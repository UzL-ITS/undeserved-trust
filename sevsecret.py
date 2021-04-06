#!/usr/bin/python3
##
# Python script to inject a secret disk password into a paused SEV VM
#  (to pause the VM start with -S option)
#
# This assumes you've already created the launch bundle using sev-tool
# from https://github.com/AMDESE/sev-tool.git
#
# sev-tool --generate_launch_blob
#
# creates several files, the only one this script needs is the TIK and TEK
# keys which are stored in tmp_tk.bin
#
# Once TIK/TEK are known, the script will probe the VM for the sev
# parameters needed to calculate the launch measure, retrieve the launch
# measure and verify against the measure calculated from the OVMF hash
# and if that matches create the secret bundle and inject it
#
# Tables and chapters refer to the amd 55766.pdf document
#
# https://www.amd.com/system/files/TechDocs/55766_SEV-KM_API_Specification.pdf
##
import sys
import os 
import base64
import hmac
import hashlib
from argparse import ArgumentParser
from uuid import UUID
from Crypto.Cipher import AES
from Crypto.Util import Counter
from qemu.python.qemu import qmp

if __name__ == "__main__":
    parser = ArgumentParser(description='Inject secret into SEV')
    parser.add_argument('--tiktek-file',
                        help='file where sev-tool stored the TIK/TEK combination, defaults to tmp_tk.bin',
                        default='tmp_tk.bin')
    parser.add_argument('--passwd',
                        help='Disk Password',
                        required=True)
    parser.add_argument('--ovmf-hash',
                        help='hash of OVMF firmware blob in hex')
    parser.add_argument('--ovmf-file',
                        help='location of OVMF file to calculate hash from')
    parser.add_argument('--socket',
                        help='Socket to connect to QMP on, defaults to localhost:6550',
                        default='localhost:6550')
    args = parser.parse_args()

    if (args.ovmf_file):
        fh = open (args.ovmf_file, 'rb')
        h = hashlib.sha256(fh.read())
        ovmf_hash = h.digest()
    elif (args.ovmf_hash):
        ovmf_hash = bytearray.fromhex(args.ovmf_hash)
    else:
        parser.error('one of --ovmf-hash or -ovmf-file must be specified')

    if (args.socket[0] == '/'):
        socket = args.socket
    elif (':' in args.socket):
        s = args.socket.split(':')
        socket = (s[0], int(s[1]))
    else:
        parse.error('--socket must be <host>:<port> or /path/to/unix')

    fh=open(args.tiktek_file, 'rb')
    tiktek=bytearray(fh.read())
    fh.close()

    ##
    #  tiktek file is just two binary aes128 keys
    ##
    TEK=tiktek[0:16]
    TIK=tiktek[16:32]

    disk_secret = args.passwd

    Qmp = qmp.QEMUMonitorProtocol(address=socket);
    Qmp.connect()
    caps = Qmp.command('query-sev')
    print('SEV query found API={api-major}.{api-minor} build={build-id} policy={policy}'.format(**caps))
    h = hmac.new(TIK, digestmod='sha256');

    ##
    # calculated per section 6.5.2
    ##
    h.update(bytes([0x04]))
    h.update(caps['api-major'].to_bytes(1,byteorder='little'))
    h.update(caps['api-minor'].to_bytes(1,byteorder='little'))
    h.update(caps['build-id'].to_bytes(1,byteorder='little'))
    h.update(caps['policy'].to_bytes(4,byteorder='little'))
    h.update(ovmf_hash)

    print('\nGetting Launch Measurement')
    meas = Qmp.command('query-sev-launch-measure')
    launch_measure = base64.b64decode(meas['data'])

    ##
    # returned data per Table 52. LAUNCH_MEASURE Measurement Buffer
    ##
    nonce = launch_measure[32:48]
    h.update(nonce)
    measure = launch_measure[0:32]

    print('Measure:   ', measure.hex())
    print('should be: ', h.digest().hex())
    print('')

    if (measure != h.digest()):
        #print("measurment did not but but we deliberately do not abort")
        sys.exit('Measurement doesn\'t match')

    print('Measurement matches, Injecting Secret')

    ##
    # construct the secret table: two guids + 4 byte lengths plus string
    # and zero terminator
    #
    # Secret layout is  guid, len (4 bytes), data
    # with len being the length from start of guid to end of data
    #
    # The table header covers the entire table then each entry covers
    # only its local data
    #
    # our current table has the header guid with total table length
    # followed by the secret guid with the zero terminated secret 
    ##
    
    # total length of table: header plus one entry with trailing \0
    l = 16 + 4 + 16 + 4 + len(disk_secret) + 1
    # SEV-ES requires rounding to 16
    l = (l + 15) & ~15
    secret = bytearray(l);
    secret[0:16] = UUID('{1e74f542-71dd-4d66-963e-ef4287ff173b}').bytes_le
    secret[16:20] = len(secret).to_bytes(4, byteorder='little')
    secret[20:36] = UUID('{736869e5-84f0-4973-92ec-06879ce3da0b}').bytes_le
    secret[36:40] = (16 + 4 + len(disk_secret) + 1).to_bytes(4, byteorder='little')
    secret[40:40+len(disk_secret)] = disk_secret.encode()
    
    ##
    # encrypt the secret table with the TEK in ctr mode using a random IV
    ##
    IV=os.urandom(16)
    # -EKNUCKLEHEADS in python crypto don't understand CTR mode
    e = AES.new(bytes(TEK), AES.MODE_CTR, counter=Counter.new(128,initial_value=int.from_bytes(IV, byteorder='big')));
    encrypted_secret = e.encrypt(bytes(secret))

    ##
    # ultimately needs to be an argument, but there's only
    # compressed and no real use case
    ##
    FLAGS = 0

    ##
    # Table 55. LAUNCH_SECRET Packet Header Buffer
    ##
    header=bytearray(52);
    header[0:4]=FLAGS.to_bytes(4,byteorder='little')
    header[4:20]=IV
    h = hmac.new(TIK, digestmod='sha256');
    h.update(bytes([0x01]))
    # FLAGS || IV
    h.update(header[0:20])
    h.update(l.to_bytes(4, byteorder='little'))
    h.update(l.to_bytes(4, byteorder='little'))
    h.update(encrypted_secret)
    h.update(measure)
    header[20:52]=h.digest()

    Qmp.command('sev-inject-launch-secret',
                **{'packet-header': base64.b64encode(header).decode(),
                   'secret': base64.b64encode(encrypted_secret).decode()
                })

    print('\nSecret Injection Successful, starting VM')

    Qmp.command('cont')

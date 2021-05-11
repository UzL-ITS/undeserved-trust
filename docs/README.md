# Abstract 
The ongoing trend of moving data and computation to the cloud is met with concerns regarding privacy and protection of intellectual property. Cloud Service Providers (CSP) must be fully trusted to not tamper with or disclose processed data, hampering adoption of cloud services for many sensitive or critical applications. As a result, CSPs and CPU manufacturers are rushing to find solutions for secure and trustworthy outsourced computation in the Cloud.
While enclaves, like Intel SGX, are strongly limited in terms of throughput and size,
AMD's Secure Encrypted Virtualization (SEV) offers hardware support for transparently protecting code and data of entire VMs, thus removing the performance, memory and software adaption barriers of enclaves. Through attestation of boot code integrity and means for securely transferring secrets into an encrypted VM, CSPs are effectively removed from the list of trusted entities.
There have been several attacks on the security of SEV, by abusing I/O channels to encrypt and decrypt data, or by moving encrypted code blocks at runtime. Yet, none of these attacks have targeted the attestation protocol, the core of the secure computing environment created by SEV. 
We show that the current attestation mechanism of Zen 1 and Zen 2 architectures has a significant flaw, allowing us to manipulate the loaded code without affecting the attestation outcome.  An attacker may abuse this weakness to inject arbitrary code at startup---and thus take control over the entire VM execution, without any indication to the VM's owner. Our attack primitives allow the attacker to do extensive modifications to the bootloader and the operating system, like injecting spy code or extracting secret data. We present a full end-to-end attack, from the initial exploit to leaking the key of the encrypted disk image during boot, giving the attacker unthrottled access to all of the VM's persistent data.




# Cite
```
@article{undeservedTrust,
  author    = {Luca Wilke and
               Jan Wichelmann and
               Florian Sieck and
               Thomas Eisenbarth},
  title     = {undeSErVed trust: Exploiting Permutation-Agnostic Remote Attestation},
  year      = {2021},
  url       = {https://uzl-its.github.io/undeserved-trust/},
}
```

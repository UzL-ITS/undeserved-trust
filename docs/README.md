
# Quick Overview
We show that the measurement used in AMD SEV's attestation is block permutation-agnostic, meaning that changing the order of measured memory blocks does not affect the attestation outcome, and thus allows the attacker to modify the execution flow without detection by the VM's owner. Using this, we construct an universal attack primitive, which reorders the measured blocks of an initially loaded UEFI and sets up a ROP chain to load and execute arbitrary code, giving the attacker full control over the VM's operating system.


# Questions & Answers

## What is AMD SEV and how does it work?
AMD SEV and its incremental extensions SEV-ES and SEV-SNP aim to protect
virtual machines from a malicious hypervisor by encrypting each VM's RAM 
with a unique key. The keys are managed by the so-called *Secure Processor* and
are inaccessible to the hypervisor. To bootstrap trust, the initial code image
(usually an UEFI binary) of the VM is cryptographically attested by the Secure processor.

## Which platforms are affected?
Our attack affects all currently available AMD EPYC server processors, from the *Naples* (Zen 1) to the *Milan* microarchitecture (Zen 3) as well as the
EPYC embedded series.

## Are there countermeasures?
Right now, there are no firmware patches for Zen 1 and Zen 2 available, leaving these systems vulnerable. For Zen 3, the SEV-SNP extension reliably prevents our attack.


# Responsible Disclosure
We've responsibly  disclosed  our  findings  to  AMD  via  email on January 19th, 2021. AMD has issued **CVE-2021-26311** and stated that the problem is resolved with SEV-SNP. As this is only
available on 3rd gen EPYC CPUs, previous generations, limited to SEV-ES, will remain
vulnerable.
[AMD Security Bulletin](https://www.amd.com/en/corporate/product-security/bulletin/amd-sb-1004)


# Videos
<iframe width="560" height="315" src="https://www.youtube-nocookie.com/embed/rCUIJhUFA3U" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

# Cite
```
@INPROCEEDINGS{9474294,
  author={Wilke, Luca and Wichelmann, Jan and Sieck, Florian and Eisenbarth, Thomas},
  booktitle={2021 IEEE Security and Privacy Workshops (SPW)}, 
  title={undeSErVed trust: Exploiting Permutation-Agnostic Remote Attestation}, 
  year={2021},
  volume={},
  number={},
  pages={456-466},
  doi={10.1109/SPW53761.2021.00064}
}
```
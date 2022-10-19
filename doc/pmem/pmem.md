# pmem
http://pmem.io/

## [[pmem/init]]

[[pmem/Rust]]

# [[libpmem]]

https://manpages.debian.org/experimental/libpmem-dev/pmem_map_file.3.en.html

```c
/// detects if the entire range `[addr, addr+len)` consists of persistent memory
int pmem_is_pmem(const void *addr, size_t len);
```

see [[Rust/FFI]]

## [[pf]]

![[pmem/bib]]

[[persistent memory]] is often referred to as [[non-volatile memory]] (NVM) or storage class memory (SCM). For software developers, questions about [[NVM]] usage include:
- What is [[persistent memory]]?
- How do I use it?
- What APIs and libraries are available? -> [[libpmem]]
- What benefits can it provide for my application?
- What new programming methods do I
need to learn?
- How do I design applications to use persistent memory?
- Where can I find information, documentation, and help?

Persistent memory is not a plug-and-play technology for software applications.
Although it may look and feel like traditional [[DRAM]] memory, **applications need to be
modified to fully utilize the persistence feature** of [[persistent memory]]. That is not to say
that applications cannot run unmodified on systems with persistent memory installed,
they can, but they will not see the full potential of what persistent memory offers without
code modification.


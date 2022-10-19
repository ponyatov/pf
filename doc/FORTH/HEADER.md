# `struct HEADER {}`

```Cpp
/// memory header
struct HEADER {
    uint16_t heap;
    uint16_t entry;
    uint16_t latest;
} __attribute__((packed));

/// pointer for direct header modifications in M[]
extern HEADER* header;
```

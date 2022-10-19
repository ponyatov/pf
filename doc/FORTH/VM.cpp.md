# `/src/VM.cpp`

![[Cpp/minimal]]
![[skelex]]

- [[регистры]]
- [[M|память Форт-системы]]

```Cpp
/// main memory
uint8_t M[Msz];
/// compiler pointer
uint16_t Cp=0;
/// instruction pointer
uint16_t Ip=0;
```
```Cpp
/// return stack
uint32_t R[Rsz];
/// return stack pointer
uint8_t Rp=0;
```
```Cpp
/// data stack
int32_t D[Dsz];
/// data stack pointer
uint8_t Dp=0;
```

```Cpp
void dump() {
    printf("\n");
    for (uint16_t addr = 0; addr < Cp; addr++) {
        if (addr % 0x10 == 0) printf("%.4X: ", addr);
        printf("%.2X ", M[addr]);
    }
    printf("\n");
}
```
```Cpp
void step() {
    // command address
    assert(Ip < Cp);  // Cp < Msz
    printf("%.4X:\t", Ip);
    // opcode
    uint8_t op = M[Ip++];
    printf("%.2X\t", op);
    // command decode/run
    switch (op) {
        case NOP:
            nop();
            break;
        case BYE:
            bye();
            break;
        default:
            printf("???\n\n");
            exit(-1);
    }
}
```
```Cpp
void go() {
    while (Ip < Cp) step();
}
```
```Cpp
void nop() { printf("nop\n"); }
```
```Cpp
void bye() {
    printf("bye\n\n");
    exit(0);
}
```

#include "pf.hpp"

void arg(int argc, char argv[]) {
  cout << "argv[" << argc << "] = <" << argv << ">\n";
}

int main(int argc, char* argv[]) {
  init(argc, argv);
  arg(0, argv[0]);
  for (int i = 1; i < argc; i++) {
    arg(i, argv[i]);
    yyfile = argv[i];
    assert(yyin = fopen(yyfile, "r"));
    yyparse();
    fclose(yyin);
  }
  zync();
  dump();
  vm();
  return fini(0);
}

HEADER* header = nullptr;

void init(int argc, char* argv[]) {
  // assert(M = (ubyte*)malloc(Msz));
  size_t mapped_len;
  int is_pmem;
  assert(M = (ubyte*)pmem_map_file("tmp/pf.pmem", Msz, PMEM_FILE_CREATE, 0666,
                                   &mapped_len, &is_pmem));
  // assert(mapped_len == Msz);
  header = (HEADER*)M;
  header->halt = op::halt;
  // header->here = sizeof(HEADER);
  // header->entry = offsetof(HEADER, halt);
  // //
  // Ip = header->entry;
  // Cp = header->here;
  // assert(Ip <= Cp);
}

void zync() {
  header->entry = Ip;
  header->here = Cp;
  header->latest = lfa;
}

int fini(int exitcode) {
  zync();
  dump();
  // free(M);
  pmem_unmap(M, Msz);
  exit(exitcode);
}

#define YYERR \
  "\n\n" << yyfile << ":" << yylineno << " " << msg << " [" << yytext << "]\n\n"

void yyerror(string msg) {
  cerr << YYERR;
  fini(-1);
}

ubyte* M = nullptr;
ucell Ip = offsetof(HEADER, halt);
ucell Cp = sizeof(HEADER);
ucell lfa = 0;  // offsetof(HEADER, latest);

ucell R[Rsz];

int D[Dsz];
ucell Dp = 0;

bool compile = false;

void dump() {
  char ascii[0x11];
  ascii[0x11] = '\0';
  ubyte byte;
  ubyte ascidx;
  printf("\nM:\n");
  for (ucell addr = 0; addr < Cp; addr++) {
    if ((addr % 0x10) == 0x0) printf("\t%.4X:\t", addr);
    byte = ((uint8_t*)M)[addr];
    printf("%.2X ", byte);
    ascidx = addr % 0x10;
    if (byte > ' ')
      ascii[ascidx] = byte;
    else
      ascii[ascidx] = '.';
    ascii[ascidx + 1] = '\0';
    if ((addr % 0x10) == 0x7) printf(" ");
    if ((addr % 0x10) == 0xF) printf("\t%s\n", ascii);
  }
  if (ascidx > 0) {
    for (auto i = 0; i < 0x10 - ascidx; i++) printf("   ");
    printf("\t%s", ascii);
  }
  printf("\n\n");
}

void q() {
  cout << "\nD:";
  for (ucell i = 0; i < Dp; i++) cout << D[i] << '\t';
  cout << "\n";
}

void wc(ucell addr) {
  ((ucell*)M)[Cp] = addr;
  Cp += sizeof(addr);
}

map<string, ucell> label;

void vm() {
  while (true) {
    assert(Ip < Cp);
    printf("\n%.4X:\t", Ip);
    op opcode = ((op*)M)[Ip++];
    printf("%.2X\t", opcode);
    switch (opcode) {
      case op::nop:
        nop();
        break;
      case op::halt:
        halt();
        break;
      default:
        printf("???\n\n");
        abort();
    }
  }
}

void nop() { printf("nop"); }

void halt() {
  printf("halt\n\n");
  exit(fini(0));
}

void def(string* name) {
  // LFA
  ucell latest = lfa;
  lfa = Cp;
  C(latest);
  // @ref NFA_count
  M[Cp++] = name->length();
  for (auto c : *name) M[Cp++] = c;
  // move entry to last defined word
  Ip = Cp;
}

void C(ubyte byte) {
  assert(Cp + sizeof(byte) < Msz);
  M[Cp++] = byte;
}

void C(ucell cell) {
  assert(Cp + sizeof(cell) < Msz);
  ((ucell*)M)[Cp] = cell;
  Cp += sizeof(cell);
}

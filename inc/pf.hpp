#pragma once

#include <assert.h>

#include <iostream>
#include <map>
#include <sstream>

using namespace std;

#include <libpmem.h>

/// unsigned byte
typedef uint8_t ubyte;
/// cell mixes both *VM memory address* and **signed integer**
typedef int32_t cell;
/// cell mixes both *VM memory address* and **unsigned integer**
typedef uint32_t ucell;

/// @brief VM command opcodes
enum class op : ubyte {  //
  nop = 0xEE,
  halt = 0xFF
};

/// @defgroup wasd WASD machine
/// @brief FORTH Virtual Machine
/// @{

/// @ref M main memory size, bytes
#define Msz 0x1000
/// @ref R return stack size, cells
#define Rsz 0x100
/// @ref D data stack size, cells
#define Dsz 0x10

/// [W]ords = vocabulary [M]emory: byte-addressed, backed by `libpmem`
/// (mmap)
extern ubyte* M;
/// [I]nstruction Pointer
extern ucell Ip;
/// [C]ompilation [P]ointer
extern ucell Cp;

/// return [S]tack
extern ucell R[Rsz];
/// return stack pointer
extern ucell Rp;

/// [D]ata stack
extern int D[Dsz];
/// Data stack pointer
extern ucell Dp;

/// @defgroup compiler compiler
/// @{

/// compilation mode flag
/// * `true` compilation
/// * `false' interpretation
extern bool compile;

struct HEADER {
  /// entry point: system startup @ref Ip value
  ucell entry;
  /// last used byte address in VM memory
  ucell here;
  /// last defined word LFA
  ucell latest;
  /// minimal program: halt
  op halt;
} __attribute__((packed));
extern HEADER* header;

/// last defined word LFA
extern ucell lfa;
/// @}

/// @}

/// @defgroup cmd VM commands
/// @ingroup wasd
/// @{

/// @brief `dump ( -- )` dump whole memory until @ref Cp
extern void dump();

/// `? ( -- )` dump @ref D data stack
extern void q();

/// `nop ( -- )` do nothing
extern void nop();

/// `halt ( -- )` stop system
extern void halt();
/// @}

/// initialize
extern void init(int argc, char* argv[]);
/// finalyze system
extern int fini(int exitcode);
/// sync some data with mmap image
extern void zync();

/// @defgroup skelex lexical skeleton
/// @brief syntax parser
/// @{

/// @name lexer
/// @{
/// lexer callback
extern int yylex();
/// current line number
extern int yylineno;
/// current lexeme value
extern char* yytext;
/// current script file
extern char* yyfile;
/// lexer input stream
extern FILE* yyin;

/// @}

/// @name parser
/// @{

extern void yyerror(string msg);
#include "pf.parser.hpp"
/// @}

/// @}

extern void wc(ucell addr);

extern map<string, ucell> label;

/// run bytecode
extern void vm();

/// create new word definition in vocabulary with current @ref Cp
extern void def(string* name);

/// @defgroup forth FORTH
/// @brief Vocabulary structure
/// @{

/// @brief vocabulary: word header
struct WORD {
  /// @brief LFA: Link Field Area
  /// linked list of all defined words in a vocabulary
  ucell LFA;
  /// @brief @ref NFA byte-counted 8bit ASCII string
  /// (with high bit indicates the IMMEDIATE flag)
  ubyte NFA_count;
  /// @brief NFA: Name Field Area
  ubyte NFA;  //[];
  /// @brief CFA: Code Field Area
  op CFA[];
};

/// @brief compile byte
extern void C(ubyte byte);
/// @brief compile ucell
extern void C(ucell cell);

/// @}

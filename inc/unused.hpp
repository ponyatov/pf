/// @file
/// @brief unsused elements: for future use (Object Graph mostly)

#include <assert.h>

#include <iostream>
#include <sstream>
using namespace std;

#include "pf.hpp"

/// @defgroup graph Object Graph
/// @brief RAM-only structures for compiler (attribute grammar -like)

/// @ingroup graph
/// @brief base class for all @ref graph elements
struct Object {
  /// @name fields

  /// @brief scalar: object name, string/number value
  string value;

  /// @name garbage collector

  /// reference counter (free on zero)
  size_t ref;
  /// global objects pool
  static Object* pool;
  /// next object in @ref pool (linked list)
  Object* next;
  /// gc pass
  static void gc();

  /// @name constructor
  Object();
  virtual ~Object();

  /// @name dump/stringify

  /// dump object graph recursively
  string dump(int depth = 0, string prefix = "");
  /// `<T:V>` header
  string head(string prefix = "");
  string pad(int depth);
  virtual string tag();
  virtual string val();

  /// @ingroup compiler
  virtual void compile();
  /// @ingroup compiler
  virtual void execute();
};

/// @ingroup graph
/// @brief scalar types
struct Primitive : Object {
  Primitive();
  void execute();
};

/// @ingroup graph
/// @brief symbol
struct Sym : Primitive {
  Sym(string V);
  string val();
};

/// @ingroup graph
/// @brief integer number
struct Int : Primitive {
  cell value;
  Int(string V);
  Int(cell N);
  string val();
  void compile();
};

struct Container : Object {};

struct Stack : Container {
  Stack(string V);
};

/// @brief EDS: Executable Data Structure (c)
struct Active : Object {};

/// @brief operator
struct Op : Active {
  string value;
  Op(string V);
};

enum class op : ubyte {};

struct Cmd : Active {
  string value;
  op opcode;
  Cmd(string V, op OP);
  void compile();
};

extern ubyte* M;
extern Object* D[Dsz];
extern ucell Dp;
extern ucell Ip;
extern ucell Cp;

int fini(int exitcode) {
  // header->entry = Ip;
  header->here = Cp;
  header->latest = lfa;
  dump();
  Object::gc();
  free(M);
  exit(exitcode);
}

// skelex
/// macro returns scanned token as @ref Object -inherited instance
#define TOKEN(C, X)           \
  {                           \
    yylval.o = new C(yytext); \
    return X;                 \
  }

#define TCMD0(OP)                   \
  {                                 \
    yylval.o = new Cmd(yytext, OP); \
    return CMD0;                    \
  }

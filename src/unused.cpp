/// @file
/// @brief unsused elements: for future use (Object Graph mostly)

#include "unused.hpp"

Object* Object::pool = nullptr;

Object::Object() {
  ref = 0;
  next = pool;
  pool = this;
}

Object::~Object() {  //
  // cerr << head("del ") << endl;
}

void Object::gc() {
  Object *p = pool, *n = nullptr;
  while (p != nullptr) {
    // cerr << p->head("gc ") << endl;
    n = p->next;
    if (!p->ref) delete p;
    p = n;
  }
}

string Object::dump(int depth, string prefix) {
  ostringstream os;
  //   head
  os << pad(depth) << head(prefix);
  // subtree
  return os.str();
}

string Object::pad(int depth) {
  ostringstream os;
  os << '\n';
  for (int i = 0; i < depth; i++) os << '\t';
  return os.str();
}

string Object::head(string prefix) {
  ostringstream os;
  os << prefix << "<" << tag() << ":" << val() << "> @" << this << " #" << ref;
  return os.str();
}

#include <cxxabi.h>
string Object::tag() {
  string ret = abi::__cxa_demangle(typeid(*this).name(), 0, 0, nullptr);
  for (char& c : ret) c = tolower(c);
  return ret;
}

string Object::val() { return ""; }

Primitive::Primitive() : Object() {}

void Primitive::execute() { D[Dp++] = this; }

Sym::Sym(string V) : Primitive() { value = V; }

string Sym::val() { return value; }

Int::Int(string V) : Primitive() { value = stoi(V); }
Int::Int(cell N) : Primitive() { value = N; }

void Int::compile() {
  assert(Cp < Msz);
  // (static_cast<cell*>(M))[Cp] = 0x1234;  // value;
  // Cp += sizeof(cell);
}

string Int::val() { return to_string(value); }

Op::Op(string V) : Active() {}

Cmd::Cmd(string V, op OP) {
  value = V;
  opcode = OP;
}

void Cmd::compile() {  //
  ((ubyte*)M)[Cp++] = static_cast<ubyte>(opcode);
}

void Object::compile() {
  cerr << this->head("compile error: ") << endl;
  abort();
}
void Object::execute() {
  cout << this->head("exec error: ") << endl;
  abort();
}

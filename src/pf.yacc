%{
    #include "pf.hpp"
%}

%defines %union {
        string *s;      // [s]tring value
        int n;          // integer [n]umber
        op c;           // VM [c]ommand
}

%token<s> SYM
%token<n> INT
%token LC RC COLON
%token<c> CMD0
%token<s> ENTRY
/* CMD1 */
%type ex

%%
syntax  :
        | syntax ex

ex      : COLON SYM     { def($2); }
        | CMD0          { C(static_cast<ubyte>$1); }
        | ENTRY SYM     { cerr << "entry:" << *$2 << endl; }

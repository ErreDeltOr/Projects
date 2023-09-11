#include <iostream>
#include "LEX.h"
Lex::Lex(type_of_lex t, int v) {
    t_lex = t;
    v_lex = v;
}
type_of_lex Lex::get_type() const {return t_lex;}

int Lex::get_value() const { return v_lex;}

ostream& operator << (ostream& s, Lex l) {
    s << '(' << Lex::TL[l.t_lex] << ',' << l.v_lex << ");" ;
    return s;
}

const char * Lex:: TL [] = {

 NULL,"and","bool","string","do","else",
"if","false","int","not","or","program","read",
"then","true","var","while","write", "goto", "@",
 ";", "@", ",", ":", "=", "(", ")",
 "==","<",">","+","-","*","/","<=","!=",">=", "\"", "{", "}", "-$",
 "num", "strconst", "id", "label", "adress", "go", "fgo", " " };
 


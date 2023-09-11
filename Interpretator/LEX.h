#ifndef TASK4_LEX_H
#define TASK4_LEX_H

#include <iostream>
#include "types_of_lex.h"

using namespace std;

class Lex {
    type_of_lex t_lex;
    int         v_lex;

public:
    static const char * TL[];
    Lex(type_of_lex t = LEX_NULL, int v = 0);
    type_of_lex get_type() const;
    int get_value() const;
    friend ostream& operator << (ostream &s, Lex l);
};
        
#endif //TASK4_LEX_H

#ifndef TASK4_IDENT_H
#define TASK4_IDENT_H

#include "types_of_lex.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Ident {
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    bool label;
    int label_str_num;
    int value_int;
public:
    Ident();
    Ident(const string& n);
    bool operator== (const string& s) const;
    string& get_name();
    bool get_declare();
    void put_declare();
    type_of_lex get_type();
    void put_type(type_of_lex t);
    bool get_assign();
    void put_assign();
    int get_label_str_num();
    void put_label_str_num(int c);
    int get_value_int();
    void put_value_int(int v);
    bool get_label();
    void put_label();
    vector <int> label_gotos;
};

extern vector <Ident> TID;
int put (const string& buf);
        
#endif //TASK4_IDENT_H

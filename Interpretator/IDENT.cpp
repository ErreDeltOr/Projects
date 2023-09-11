#include <iostream>
#include "IDENT.h"
using namespace std;

Ident::Ident() { declare = false; assign = false; label = false; }

Ident::Ident(const string& n) {name=n; declare = false; assign = false; label = false; }

bool Ident::operator== (const string& s) const { return name == s; }

string& Ident::get_name() { return name; }

bool Ident::get_declare() { return declare; }

void Ident::put_declare() { declare = true; }

type_of_lex Ident::get_type() { return type; }

void Ident::put_type(type_of_lex t) { type = t; }

bool Ident::get_assign() { return assign; }

void Ident::put_assign(){ assign = true; }

int Ident::get_label_str_num() {return label_str_num;};

void Ident::put_label_str_num(int c) {label_str_num = c;};

int Ident::get_value_int() { return value_int; } 

void Ident::put_value_int(int v){ value_int= v; }  

bool Ident::get_label() { return label; }

void Ident::put_label() { label = true; }

vector <Ident> TID;
int put (const string & buf) {
    vector <Ident>::iterator k;
    if ( (k = find (TID.begin(), TID.end(), buf)) != TID.end() ) {
        return k - TID.begin();
    }
 TID.push_back ( Ident (buf) );
 return TID.size() - 1;
}
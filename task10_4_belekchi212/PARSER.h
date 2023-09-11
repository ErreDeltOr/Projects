#ifndef TASK4_PARSER1_H
#define TASK4_PARSER1_H

#include "types_of_lex.h"
#include "LEX.h"
#include "IDENT.h"
#include "SCANNER.h"
#include "STACK.h"

#include <cstdio> 

class Parser {
 Lex curr_lex;
 type_of_lex c_type;
 int c_val;
 Scanner scan;
 Stack <int, 100> st_int;
 Stack <type_of_lex, 100> st_lex;
 void PR(); void D1(); void OP1();
 void VAR(); void OP();
 void ST(); void E1(); void T(); void F();
 void gl();
 
 void dec ( type_of_lex type); // семантичиеские действия
 void check_id ();
 void check_op ();
 void check_not ();
 void check_unminus();
 void eq_type ();
 void eq_bool ();
 void check_id_in_read ();
 void set_adress(int c_val);
 public:
 vector <Lex> poliz;
 Parser (const char *program);
 void analyze();
 };

extern type_of_lex z;
extern vector <int> label_vect;

void search(int tid_num);

#endif //TASK4_PARSER1_H
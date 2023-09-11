#ifndef TASK4_SCANNER_H
#define TASK4_SCANNER_H

#include "types_of_lex.h"
#include "LEX.h"
#include "IDENT.h"
#include <cstdio> 

class Scanner {
    FILE * fp;
    char c;
    int look (const string& buf, const char * * list);
    void gc();
public:
    static const char * TW[],* TD[];
    Scanner (const char * program);
    Scanner ();
    Lex get_lex();
};

int isalpha (char c);
int isdigit (char c);
int add(const string& buf);
extern vector <string> STR;
extern int str_numb;
extern int position_numb;        
#endif //TASK4_SCANNER_H

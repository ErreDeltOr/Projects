#include <iostream>
#include "SCANNER.h"

int str_numb=1, position_numb=1;

vector <string> STR;

int add (const string & buf) {
    vector <string>::iterator k;
    if ( (k = find (STR.begin(), STR.end(), buf)) != STR.end() ) {
        return k - STR.begin();
    }
 STR.push_back (buf);
 return STR.size() - 1;
}

int isalpha (char c) {
    char alphabet [26] = {'a','b','c','d','e','f','g','h','i',
                          'j','k','l','m','n','o','p','q','r',
                          's','t','u','v','w','x','y','z'};
    for (int i = 0; i<26; i++) {
        if (c == alphabet[i]) {
            return 1;
        }
    }
    return 0;
}

int isdigit (char c) {
    char digits[10] = {'0','1','2','3','4','5','6','7','8','9'};
    for (int i = 0; i<10; i++) {
        if (c == digits[i]) {
            return 1;
        }
    }
    return 0;  
}

const char * Scanner:: TW [] = {

 NULL,"and","bool","string","do","else",
"if","false","int","not","or","program","read",
"then","true","var","while","write", "goto", " "
};

const char * Scanner:: TD [] = {
 NULL,";", "@", ",", ":", "=", "(", ")",
 "==","<",">","+","-","*","/","<=","!=",">=", "\"", "{", "}", " "
};


int Scanner::look (const string& buf, const char * * list){
    int i = 1;
    while (list[i] != string(" ")) {
        if ( buf == list[i] ) {
            return i;
        }
        i++;
    }
    return 0;
}
void Scanner::gc() { c = fgetc(fp); }

Scanner::Scanner (const char * program){
    fp = fopen ( program, "r" );
    if (fp==NULL)throw "can’t open file";
 }

Scanner::Scanner () {
    fp = fopen ( "sss.txt", "r" );
    if (fp==NULL)throw "can’t open file";
}
Lex Scanner:: get_lex() {
    enum state{H,IDENT, NUMB, STRING, CHECKCOM1, CHECKCOM2, COM, WRONGCOM, ALE, NEQ };
    state CS = H; string buf; int d, j;
    do { gc();
         position_numb++;
        switch(CS) {
        case H:
            if(c ==' ' || c =='\n' || c=='\r' || c =='\t') {
                if (c == '\n') {str_numb++; position_numb = 1;}
            }
            else if (isalpha(c)) {buf.push_back(c); CS = IDENT;}
            else if ( isdigit(c) ) { d = c - '0'; CS = NUMB; }
            else if ( c == '\"' ) { CS = STRING; }
            else if ( c == '/' ) { CS = CHECKCOM1; }
            else if (c == '*') {CS = WRONGCOM; } 
            else if (c== '=' || c== '<' || c== '>') {
                     buf.push_back(c); CS = ALE; }
            else if (c == '@') return Lex(LEX_FIN);
            else if (c == '!') {buf.push_back(c); CS = NEQ; }
            else { buf.push_back(c); 
                if ( (j = look ( buf, TD)) )
                    return Lex((type_of_lex) (j+(int) LEX_FIN),j);
                else throw c;
            }
            break;
        case IDENT:
            if ( isalpha(c) || isdigit(c)) {buf.push_back(c);}
            else{ ungetc(c, fp);
                if ( (j = look (buf, TW)) )
                    return Lex ((type_of_lex) j, j);
                else {
                    j = put(buf); 
                    return Lex (LEX_ID, j);
                }
            }
            break;
        case NUMB:
            if ( isdigit(c) ) {d = d * 10 + (c - '0');}
            else { ungetc(c,fp); return Lex (LEX_NUM, d);}
            break;
        case STRING:
            if ( (c != '\"') && (c != '@') ) {buf.push_back(c);}
            else if (c == '@') {throw c;}
            else {j = add(buf); return Lex (LEX_STRINGCONST, j);}
            break;
        case CHECKCOM1:
            if ( c == '*' ) {CS = COM;}
            else {
                ungetc(c, fp);
                j = look("/", TD); 
                return Lex((type_of_lex) (j+(int) LEX_FIN),j);
            } 
            break;
        case COM:
            if ( c == '*' ) {CS = CHECKCOM2; }
            else
            if (c == '@') throw c;
            break;
        case CHECKCOM2:
            if ( c == '/' ) {CS = H; }
            else {ungetc(c, fp); CS = COM;};
            break;
        case WRONGCOM:
            if ( c == '/' ) {throw c; }
            else {
                ungetc(c, fp);
                j = look("*", TD); 
                return Lex((type_of_lex) (j+(int) LEX_FIN),j);
            } 
            break;
        case ALE:
            if (c=='='){ buf.push_back(c); j = look( buf, TD);
                return Lex ((type_of_lex)(j+(int) LEX_FIN),j);
            }
            else {ungetc(c, fp); j = look (buf, TD);
                return Lex((type_of_lex) (j+(int) LEX_FIN),j); 
            }
            break;
        case NEQ: 
            if (c == '=') {
                buf.push_back(c); j = look ( buf, TD );
                return Lex ( LEX_NEQ, j ); 
            }
            else throw '!';
            break;
        } //end of switch
    } while (true);
}
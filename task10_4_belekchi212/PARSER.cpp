#include <iostream>
#include "EXECUTER.h"

type_of_lex z;

vector <int> label_vect;

void search(int tid_num) {
    for (int i = 0; i < label_vect.size(); i++) {
        if (label_vect[i] == tid_num) {
            label_vect.erase (label_vect.begin() + i);
            i--;
        }
    }
}

void Parser::set_adress(int c_val) {
    int amount = (TID[c_val].label_gotos).size();
    for (int i = 0; i<amount; i++) {
        poliz[(TID[c_val].label_gotos)[i]] = 
            Lex(POLIZ_LABEL, TID[c_val].get_label_str_num());
    }

}
    
    
    

Parser::Parser (const char *program){}

void Parser::gl(){
    curr_lex = scan.get_lex();
    c_type = curr_lex.get_type();
    c_val = curr_lex.get_value();
}

void Parser::analyze() {
    gl();
    PR();
    if (!label_vect.empty()) {throw "ERROR:TRANSITION ON AN UNDECLARED LABEL!";}
    if (c_type != LEX_FIN) {
        throw "ERROR:NO END SIGN!";
    }

    //int amount = poliz.size();
    //for(int h = 0; h<amount; h++) {
       // cout << poliz[h] << "  [" << h << "]" << endl;
    //}
    //cout << "ALL ARITHM OPERATIONS IN POLIZ:" << endl;


    //amount = poliz.size();
    //for(int h = 0; h<amount; h++) {
        //if(((poliz[h]).get_type() == LEX_UNMIN) || ((poliz[h]).get_type()==LEX_PLUS) ||
            //((poliz[h]).get_type() == LEX_MINUS) || ((poliz[h]).get_type()==LEX_DIV) ||
            //((poliz[h]).get_type() == LEX_MULT) )
        //cout << poliz[h] << "  [" << h << "]" << endl;
    //}
}

void Parser::dec ( type_of_lex type )
{
    int i;
    while ( !st_int.is_empty())
    {
         i = st_int.pop();
         if ( TID[i].get_declare() )
             throw "ERROR:VARIEBLE DECLARED TWICE!";
         else
         {
             TID[i].put_declare();
             TID[i].put_type(type);
         }
    }
}

void Parser::check_id()
{
    if ( TID[c_val].get_declare() )
        st_lex.push(TID[c_val].get_type());
    else
        throw "ERROR:VARIABLE IN STATEMENT IS NOT DECLARED!";
}


void Parser::check_op ()
{
    type_of_lex t1, t2, op;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    if (t1!=t2) { throw "ERROR:WRONG TYPES IN OPERATION!"; }
    if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_MULT || op == LEX_DIV) {
        if (t1 == LEX_BOOL) {
            throw "ERROR:WRONG TYPES IN OPERATION!";
        }
        if (t1 == LEX_STRING) {
            if (op != LEX_PLUS) {
                throw "ERROR:WRONG TYPES IN OPERATION!";
            }
        }
        st_lex.push(t1);
    }
    else if (op == LEX_L || op == LEX_G || op == LEX_LEQ || op == LEX_GEQ ||
             op == LEX_EQ || op == LEX_NEQ) {
        if (t1 == LEX_BOOL) {
            throw "ERROR:WRONG TYPES IN OPERATION!";
        }
        if (t1 == LEX_STRING) {
            if (op != LEX_L && op != LEX_G && op != LEX_EQ && op != LEX_NEQ) {
                throw "ERROR:WRONG TYPES IN OPERATION!";
            }
        }
        st_lex.push(LEX_BOOL);
    }
    else {                         // AND, OR //
        if (t1 != LEX_BOOL) { 
            throw "ERROR:WRONG TYPES IN OPERATION!";
        }
        else {st_lex.push(LEX_BOOL);}
    }
    poliz.push_back(Lex(op));
}

void Parser::check_not ()
{
    if (st_lex.pop() != LEX_BOOL)
        throw "ERROR:WRONG TYPES IN OPERATION \"not\" !";
    else
    {
        st_lex.push (LEX_BOOL);
        poliz.push_back (Lex (LEX_NOT));
    }
}

void Parser::check_unminus ()
{
    if (st_lex.pop() != LEX_INT)
        throw "ERROR:WRONG TYPES IN OPERATION \"unar -\" !";
    else
    {
        st_lex.push (LEX_INT);
        poliz.push_back (Lex (LEX_UNMIN));
    }
}

void Parser::eq_type ()
{
    if ( st_lex.pop() != st_lex.pop() ) throw "ERROR:WRONG TYPES IN OPERATION \"=\" !";
}

void Parser::eq_bool ()
{
    if ( st_lex.pop() != LEX_BOOL )
        throw "ERROR: EXPRESSION IS NOT OF TYPE \"bool\" !";
}

void Parser::check_id_in_read ()
{
    if ( !TID [c_val].get_declare() )
        throw "VARIABLE IN OPERATOR \"read\" IS NOT DECLARED";
}




void Parser::PR() {
    if (c_type == LEX_PROGRAM) { gl();}
    else {throw "ERROR:NO KEYWORD \"program\" !";}
    if (c_type == LEX_FIGOP) {gl();}
    else {throw "ERROR:NO \"{\" AFTER \"program\" !";}
    D1();
    OP1();
    if (c_type == LEX_FIGCL) {gl();}
    else {throw "ERROR:NO \"}\" AFTER \"program\" !";}
    
}

void Parser::D1() {
    while ((c_type == LEX_INT) || (c_type == LEX_STRING || c_type == LEX_BOOL)) {
        z = c_type;
        st_int.reset();
        gl();
        VAR();
        while (c_type == LEX_COMMA){ gl(); VAR();}
        if (c_type != LEX_SEMICOLON){ throw "ERROR:NO \";\" AFTER VARIABLE DECLARATION!";}
        dec(z);
        gl();
    }

}

void Parser::VAR() {
    int c_val0 = c_val;
    Lex curr_lex2;
    if (c_type==LEX_ID){st_int.push(c_val);gl();}
    else {throw "ERROR:DECLARATION OF NON-VARIABLE!";}
    if (c_type == LEX_ASSIGN) {
        gl();
        curr_lex2 = curr_lex;
        if ((c_type == LEX_NUM)||(c_type == LEX_STRINGCONST) ||
             c_type == LEX_TRUE || c_type == LEX_FALSE) {
                 if ((z == LEX_INT) && (c_type != LEX_NUM)) {
                     throw "ERROR:ASSIGNING WRONG CONST TYPE!";
                 }
                 else if ((z == LEX_STRING) && (c_type != LEX_STRINGCONST)) {
                     throw "ERROR:ASSIGNING WRONG CONST TYPE!";
                 }
                 else if ((z == LEX_BOOL) && (c_type != LEX_TRUE) &&
                          (c_type != LEX_FALSE)) {
                     throw "ERROR:ASSIGNING WRONG CONST TYPE!";
                 }
                 else {
                     poliz.push_back(Lex(POLIZ_ADDRESS, c_val0));
                     poliz.push_back(curr_lex2);
                     poliz.push_back(Lex(LEX_ASSIGN));
                     gl();
                 }
        }
        else {throw "ERROR:ASSIGNING NON CONST VALUE IN VARIABLE DECLARATION!";}
    }
}

void Parser::OP1() {
     if (c_type != LEX_FIGCL) {OP();}
     while ((c_type != LEX_FIN) && (c_type != LEX_FIGCL)) { OP();}
     if (c_type == LEX_FIN) {throw "ERROR:SYNTAX ERROR!";}
}





void Parser::OP() {
    if (c_type == LEX_ID) {
        if (TID[c_val].get_declare()) {
            check_id();
            poliz.push_back(Lex (POLIZ_ADDRESS, c_val));
            gl();
            if (c_type != LEX_ASSIGN) { throw "ERROR:SYNTAX ERROR WHILE ASSIGNING!";}
            gl();
            ST();
            eq_type();
            poliz.push_back(Lex (LEX_ASSIGN));
            if (c_type != LEX_SEMICOLON) { throw "ERROR:NO \";\" AFTER ASSIGNING!";}
            gl();
        }
        else {
            if (TID[c_val].get_label()) {throw "ERROR: LABEL HAS ALREADY BEEN DECLARED!";}
            TID[c_val].put_label();
            TID[c_val].put_label_str_num(poliz.size());
            search(c_val);
            set_adress(c_val);
            gl();
            if (c_type != LEX_COLON) { throw "ERROR: NO \":\" AFTER LABEL!";}
            gl();
            OP();
        }
    }
    else if (c_type == LEX_GOTO) { 
        gl();
        if (c_type != LEX_ID) {throw "ERROR: NO LABEL AFTER \"goto\"!";}
        if (TID[c_val].get_declare()){throw "ERROR: VARIABLE HAS ALREADY BEEN DECLARED!";}

        if (!TID[c_val].get_label()) {
            (TID[c_val].label_gotos).push_back(poliz.size());
            poliz.push_back (Lex ());
            label_vect.push_back(c_val);
        }
        else { poliz.push_back(Lex(POLIZ_LABEL, TID[c_val].get_label_str_num()));}
        poliz.push_back(Lex (POLIZ_GO));
        gl();
        if (c_type !=LEX_SEMICOLON) {throw "ERROR: NO \";\" AFTER \"goto\"!";}
        gl();
    }
    else if (c_type == LEX_IF) {
        int pl2,pl3;
        gl();
        if (c_type == LEX_OPPAR) { gl(); ST();eq_bool();}
        else {throw "ERROR:NO \"(\" AFTER \"if\"!";}

        pl2= poliz.size(); poliz.push_back (Lex( ));
        poliz.push_back (Lex (POLIZ_FGO)); 

        if (c_type == LEX_CLPAR) { gl(); OP();}
        else {throw "ERROR:NO \")\" AFTER \"if\"!";}

        pl3 = poliz.size(); poliz.push_back (Lex( ));
        poliz.push_back (Lex (POLIZ_GO));
        poliz[pl2] = Lex (POLIZ_LABEL, poliz.size( ));

        if (c_type == LEX_ELSE) { gl(); OP();}
        poliz[pl3] = Lex (POLIZ_LABEL, poliz.size( ));
    }

    else if (c_type == LEX_WHILE) {
        int pl0, pl1;

        pl0 = poliz.size();

        gl();
        if (c_type == LEX_OPPAR) { gl(); ST();eq_bool();}
        else {throw "ERROR:NO \"(\" AFTER \"while\"!";}
        
        pl1 = poliz.size( ); poliz.push_back (Lex( ));
        poliz.push_back (Lex (POLIZ_FGO)); 

        if (c_type == LEX_CLPAR) { gl();}
        else {throw "ERROR:NO \")\" AFTER \"while\"!";}
        OP();

        poliz.push_back (Lex(POLIZ_LABEL, pl0));
        poliz.push_back (Lex (POLIZ_GO));
        poliz[pl1] = Lex (POLIZ_LABEL, poliz.size( ));

    }
    else if (c_type == LEX_DO) {
        int pl0,pl1;

        pl0 = poliz.size();

        gl();
        OP();
        if (c_type != LEX_WHILE) {throw "ERROR:NO \"while\" AFTER \"do\"!";}
        gl();
        if (c_type == LEX_OPPAR) { gl(); ST();eq_bool();}
        else {throw "ERROR:NO \"(\" AFTER \"while\"!";}
        
        pl1 = poliz.size( ); poliz.push_back (Lex( ));
        poliz.push_back(Lex (POLIZ_FGO));
        poliz.push_back(Lex(POLIZ_LABEL, pl0));
        poliz.push_back(Lex (POLIZ_GO));
        poliz[pl1] = Lex (POLIZ_LABEL, poliz.size( ));

        if (c_type != LEX_CLPAR) {throw "ERROR:NO \")\" AFTER \"while\"!"; }
        gl();
    }
    else if (c_type == LEX_READ) {
        gl();
        if (c_type == LEX_OPPAR) {
            gl();
            if (c_type != LEX_ID) {throw "ERROR:READING NON-VARIABLE!";}
            check_id_in_read();

            poliz.push_back (Lex (POLIZ_ADDRESS, c_val));

            gl();
        }
        else {throw "NO \"(\" AFTER \"read\"";}

        poliz.push_back (Lex (LEX_READ));

        if (c_type != LEX_CLPAR) {throw "ERROR:NO \")\" AFTER \"read\"!";}
        gl();
        if (c_type != LEX_SEMICOLON) {throw "ERROR:NO \";\" AFTER \"read\"!";}
        gl();
    }
    else if (c_type == LEX_WRITE) {
        gl();
        if (c_type == LEX_OPPAR) {
            gl();
            ST();
            poliz.push_back (Lex (LEX_WRITE));
        }
        else {throw "ERROR:NO \"(\" AFTER \"write\"!";}
        while (c_type == LEX_COMMA){ gl(); ST(); poliz.push_back (Lex (LEX_WRITE));}
        if (c_type != LEX_CLPAR) {throw "ERROR:NO \")\" AFTER \"while\"!";}
        gl();
        if (c_type != LEX_SEMICOLON) {throw "ERROR:NO \";\" AFTER \"while\"!";}
        gl();
    }
    else if (c_type == LEX_FIGOP) {
        gl();
        OP1();
        if (c_type != LEX_FIGCL) { throw "ERROR:NO \")\" AFTER OPERATOR!";}
        gl();
    }
    else {
        ST();
        if (c_type != LEX_SEMICOLON) { throw "ERROR:NO \";\" AFTER OPERATOR!";}
        gl();
    }

}

void Parser::ST() {
    E1();
    if ( c_type == LEX_EQ || c_type == LEX_L || c_type == LEX_G ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ )
    {
        st_lex.push(c_type);
        gl();
        E1();
        check_op();
    }
}

void Parser::E1 () {
    T();
    while ( c_type==LEX_PLUS || c_type==LEX_MINUS || c_type==LEX_OR )
    {
        st_lex.push (c_type);
        gl();
        T();
        check_op();

     }
}

void Parser::T () {
    F();
    while ( c_type==LEX_MULT || c_type==LEX_DIV || c_type==LEX_AND )
    {
        st_lex.push (c_type);
        gl();
        F();
        check_op();
    }
}

void Parser::F () {
    if ( c_type == LEX_ID )
    {
        check_id();
        poliz.push_back (curr_lex);
        gl();
    }
    else
    if ( c_type == LEX_NUM )
    {
        st_lex.push ( LEX_INT );
        poliz.push_back (curr_lex);
        gl();
    }
    else
    if ( c_type == LEX_STRINGCONST )
    {
        st_lex.push ( LEX_STRING );
        poliz.push_back (curr_lex);
        gl();
    } 
    else
    if ( c_type == LEX_TRUE )
    {
        st_lex.push ( LEX_BOOL );
        poliz.push_back (curr_lex);
        gl();
    }
    else
    if ( c_type == LEX_FALSE )
    {
        st_lex.push ( LEX_BOOL );
        poliz.push_back (curr_lex);
        gl();
    }
    else
    if ( c_type == LEX_NOT )
    {
        gl();
        F();
        check_not();
    }
    else
    if ( c_type == LEX_MINUS )
    {
        gl();
        F();
        check_unminus();
    }
    else
    if ( c_type == LEX_OPPAR )
    {
        gl();
        ST();
        if ( c_type == LEX_CLPAR)
            gl();
        else
            throw "ERROR:NO \")\" AFTER STATEMENT!";
    }
    else
        throw "ERROR:SYNTAX ERROR IN STATEMENT!";
}








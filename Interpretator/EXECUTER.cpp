#include "EXECUTER.h"

template <class T, class T_EL>
void from_st (T& t, T_EL & x) { x = t.top(); t.pop(); }


void Executer::execute (vector <Lex> & poliz ) {
    Lex pc_el;
    stack < Lex > args;
    Lex i, j; int index = 0, size = poliz.size ( );
    int val;
    int k;
    string val_str;
    while ( index < size ) {
        pc_el = poliz [ index ];
        switch ( pc_el.get_type () ) {
            case LEX_NUM: args.push ( Lex(LEX_INT, pc_el.get_value()) ); break;
            case POLIZ_ADDRESS: case POLIZ_LABEL: case LEX_STRINGCONST:
                args.push ( poliz[index] );
            break;
            case LEX_TRUE:
                args.push (Lex(LEX_BOOL, 1));
                break;
            case LEX_FALSE:
                args.push (Lex(LEX_BOOL, 0));
                break;
            case LEX_ID:
                val = pc_el.get_value ( );
                if ( TID [ val ].get_assign ( ) ) {
                    if ( TID [ val ].get_type ( ) == LEX_STRING ) {
                        args.push(Lex(LEX_STRINGCONST,TID[val].get_value_int() )); break;
                    }
                    else if ( TID [ val ].get_type ( ) == LEX_INT ) {
                        args.push(Lex(LEX_INT,TID[val].get_value_int() )); break;
                    }
                    else {
                        args.push(Lex(LEX_BOOL,TID[val].get_value_int() )); break;
                    }
                }
                else
                    throw "POLIZ: indefinite identifier";

            case LEX_NOT:
                from_st(args, i);
                args.push(Lex(LEX_BOOL, !(i.get_value()))); break;
            case LEX_UNMIN:
                from_st(args, i);
                args.push(Lex(LEX_INT, (-1)*i.get_value())); break;
            case LEX_OR:
                from_st(args,j);
                from_st(args,i);
                args.push(Lex(LEX_BOOL, i.get_value() || j.get_value())); break;
            case LEX_AND:
                from_st(args,j);
                from_st(args,i);
                args.push(Lex(LEX_BOOL, i.get_value() && j.get_value())); break;
            case POLIZ_GO:
                from_st(args, i);
                index = i.get_value() - 1; break;
            case POLIZ_FGO:
                from_st(args, i);
                from_st(args, j);
                if( ! j.get_value()) index = i.get_value() -1; break;
            case LEX_WRITE:
                from_st(args, i);
                if (i.get_type() == LEX_INT ) {
                    cout <<i.get_value() << endl; break;
                }
                else if (i.get_type() == LEX_STRINGCONST) {
                    cout <<STR[i.get_value()] << endl; break;
                }
                else {
                    if (i.get_value() == 1) {cout << "TRUE" << endl;}
                    else {cout << "FALSE" << endl;}
                    break;
                }
            case LEX_READ:
                from_st (args, i);
                if ( TID [ i.get_value() ].get_type () == LEX_INT ) {
                    cout << "Input int value for ";
                    cout << TID[i.get_value()].get_name () << " = "; 
                    cin >> k;
                }
                else if (TID [ i.get_value() ].get_type () == LEX_BOOL) { string j;
                        while(1) {
                            cout << "Input boolean value (TRUE or FALSE) for ";
                            cout << TID [i.get_value()].get_name ( ) << " = "; 
                            cin >> j;
                            if ( j != "true" && j != "false" ) {
                                cout << "Error in input: true/false" << endl;
                                continue;
                            }
                            k = (j == "true") ? 1 : 0; break;
                        }
                }
                else {
                    cout << "Input string value for ";
                    cout << TID [i.get_value()].get_name ( ) << " = ";
                    cin >> val_str;
                    k = add(val_str);
                }
                TID [ i.get_value() ].put_value_int (k);
                TID [ i.get_value() ].put_assign ();
                break;

            case LEX_PLUS:
                from_st(args,j);
                from_st(args,i);
                if (i.get_type() == LEX_INT ) {
                    args.push(Lex(LEX_INT, i.get_value() + j.get_value())); break;
                }
                else {
                    val_str = STR[i.get_value()] + STR[j.get_value()];
                    k = add(val_str);
                    args.push(Lex(LEX_STRINGCONST, k)); break;
                }
            case LEX_MULT:
                from_st(args,j);
                from_st(args,i);
                args.push(Lex(LEX_INT, i.get_value() * j.get_value())); break;
            case LEX_MINUS:
                from_st(args,j);
                from_st(args,i);
                args.push(Lex(LEX_INT, i.get_value() - j.get_value())); break;
            case LEX_DIV:
                from_st(args,j);
                from_st(args,i);
                if(j.get_value() != 0) {
                    args.push(Lex(LEX_INT, i.get_value() / j.get_value())); break;
                }
                else throw "POLIZ:divide by zero";
            case LEX_EQ:
                from_st(args,j);
                from_st(args,i);
                if (i.get_type() == LEX_INT ) {
                    args.push(Lex(LEX_BOOL, i.get_value() == j.get_value())); break;
                }
                else {
                    k = STR[i.get_value()] == STR[j.get_value()];
                    args.push(Lex(LEX_BOOL, k)); break;
                }
            case LEX_GEQ:
                from_st(args,j);
                from_st(args,i);
                args.push(Lex(LEX_INT, i.get_value() >= j.get_value())); break;
            case LEX_LEQ:
                from_st(args,j);
                from_st(args,i);
                args.push(Lex(LEX_INT, i.get_value() <= j.get_value())); break;
            case LEX_G:
                from_st(args,j);
                from_st(args,i);
                if (i.get_type() == LEX_INT ) {
                    args.push(Lex(LEX_BOOL, i.get_value()  > j.get_value())); break;
                }
                else {
                    k = STR[i.get_value()] > STR[j.get_value()];
                    args.push(Lex(LEX_BOOL, k)); break;
                }
            case LEX_L:
                from_st(args,j);
                from_st(args,i);
                if (i.get_type() == LEX_INT ) {
                    args.push(Lex(LEX_BOOL, i.get_value() < j.get_value())); break;
                }
                else {
                    k = STR[i.get_value()] < STR[j.get_value()];
                    args.push(Lex(LEX_BOOL, k)); break;
                }
            case LEX_NEQ:
                from_st(args,j);
                from_st(args,i);
                if (i.get_type() == LEX_INT ) {
                    args.push(Lex(LEX_BOOL, i.get_value() != j.get_value())); break;
                }
                else {
                    k = STR[i.get_value()] != STR[j.get_value()];
                    args.push(Lex(LEX_BOOL, k)); break;
                }
            case LEX_ASSIGN:
                from_st(args,j);
                from_st(args,i);
                TID[i.get_value()].put_value_int(j.get_value());
                TID[i.get_value()].put_assign( ); break;
            default:
                throw "POLIZ: unexpected elem";

        } //end of switch
        index++;

    } //end of while
    cout << "Finish of executing!!!" << endl;

} // end of execute
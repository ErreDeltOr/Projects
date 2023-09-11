#include "EXECUTER.h"

int main() {
    try {
    Parser parser("sss.txt");
    parser.analyze();
    Executer exec;
    exec.execute(parser.poliz);
   }
    catch(Lex lex) { cout << lex <<endl;}
    catch (char c) { cout << c << endl;}
    catch (const char * l){cout << str_numb << ":" << position_numb << " " << l<<endl;}
    catch (...){cout << "???" <<endl;}
    return 0;
} 
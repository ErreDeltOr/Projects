/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/
types_of_lex - Содержит в себе перечислимый тип для опознавания лексем
/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/



/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/
Lex:
    type_of_lex t_lex; - тип лексемы
    int         v_lex; - значение лексемы


    Lex(type_of_lex t = LEX_NULL, int v = 0); - конструктор
    type_of_lex get_type() const; - получение типа лексемы
    int get_value() const; - получение значения лексемы (для переменных это номер в
                                                         Таблице TID)
    friend ostream& operator << (ostream &s, Lex l); - перегруженная операция вывода
/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/



/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/
Ident:
    string name; - название переменной
    bool declare; - флаг, говорящий о том, объявлена ли переменная или нет
    type_of_lex type; - тип переменной
    bool assign; - флаг, говорящий о том, присвоено ли какое-то значение переменной
    bool label; - флаг метки
    int label_str_num; - номер элемента в ПОЛИЗе, куда надо переходить по метке
    int value_int; - значение переменной
/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/


/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/
Scanner:
    FILE * fp; - файловый дескриптор файла с программой
    char c; - 
    int look (const string& buf, const char * * list); - поиск строки в списке строк
    void gc(); - получение очередного символа

    static const char * TW[],* TD[]; - константные таблицы лексем
    Scanner (const char * program); - конструктор
    Scanner (); 
    Lex get_lex(); - метод , выдающий синтаксическому анализатору очередную лексему

    int isalpha (char c); - функция, определяющая, является ли символ буквой
    int isdigit (char c); - функция, определяющая, является ли символ цифрой
    int add(const string& buf); - добавляет в таблицу константных строк очередную строку
    extern vector <string> STR; - таблица константных строк программы
    extern int str_numb; - номер строки в программе
    extern int position_numb; - позиция в строке в программе
/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/



/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/
 Parser:
 Lex curr_lex; - текущая лексема в программе
 type_of_lex c_type; - тип текущей лексемы
 int c_val; - значение текущей лексемы
 Scanner scan; - член класса Scanner, выдающий очередную лексему
 Stack <int, 100> st_int; - семантический стек для объявления переменных
 Stack <type_of_lex, 100> st_lex; - семантический стек для контроля типов операций
 void PR(); void D1(); void OP1();
 void VAR(); void OP();
 void ST(); void E1(); void T(); void F();
 void gl();
 
 void dec ( type_of_lex type); - объявляет переменные и не дает этого сделать дважды
 void check_id (); - проверят,  описана ли переменная
 void check_op (); - проверяет соответствие типов операндов
 void check_not (); - проверяет соответствие типов для операции not
 void check_unminus(); - проверяет соответствие типов для операции унарного -
 void eq_type ();
 void eq_bool ();
 void check_id_in_read (); - проверяет описана ли переменная при чтении из вх. Потока
 void set_adress(int c_val); - ставит адреса в полизе возле встретившихся goto

/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/




/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/
Stack:
    T s[max_size]; -  стек в виде массива
    int top; -вершина стека

    Stack(); -  конструктор
    void reset(); - очистка стека
    void push(T i); - добавление элемента в стек
    T pop(); - удаление элемента из стека
    bool is_empty(); - проверка на пустоту стека
    bool is_full(); - проверка на переполнение стека
/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/



/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/
Executer:  
    void execute (vector <Lex> & poliz); - функция, интерпретирующая ПОЛИЗ
/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/



/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/
main:  
    Запускает Parser и Executer и отлавливает все исключения
/----------------------------------------------------------------------------------------/
/----------------------------------------------------------------------------------------/











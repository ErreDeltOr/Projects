#ifndef TASK4_STACK_H
#define TASK4_STACK_H

#include "LEX.h"
#include <cstdio> 

template <class T, int max_size > 
class Stack
{
    T s[max_size];
    int top;
public:
    Stack();
    void reset();
    void push(T i);
    T pop();
    bool is_empty();
    bool is_full();
};
       
#endif //TASK4_STACK_H

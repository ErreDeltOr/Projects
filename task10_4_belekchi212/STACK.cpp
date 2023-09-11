#include "STACK.h"

template <class T, int max_size >
Stack <T, max_size >::Stack(){top = 0;}

template <class T, int max_size >
void Stack <T, max_size >::reset() { top = 0; }

template <class T, int max_size >
bool Stack <T, max_size >::is_empty(){ return top == 0; }

template <class T, int max_size >
bool Stack <T, max_size >::is_full() { return top == max_size; }


template <class T, int max_size >

void Stack <T, max_size >::push(T i)
{
    if ( !is_full() )
    {
        s[top] = i;
        ++top;
    }
    else
        throw "Stack_is_full";

}

template <class T, int max_size >

T Stack <T, max_size >::pop()
{
    if ( !is_empty() )
    {
        --top;
        return s[top];
    }
    else
        throw "Stack_is_empty";
}
        
template class Stack<int, 100>;
template class Stack<type_of_lex, 100>;

#include "stack.h"
#include "print_stack.h"


void print_stack_data(IntStack *s)
{
    int i = 0;

    while(s->top - i >= 0)
    {
        printf(" %c ", int_to_operator(s->data[i]) );
        i++;
    }
    printf("\n");
}

char int_to_operator(int i)
{
  switch(i)
  {
    case 0 : return'-';    // / Unary minus
    case  1: return'=';    // Assignment
    case  2: return'+';    // Addition
    case  3: return'-';    // Subtraction
    case  4: return'*';   // Multiplication
    case  5: return'/';    // Division
    case  6: return'%';   // Modulo

    case  7: return'I';    //  Increment
    case  8: return'D';   //  Decrement

    case  9: return'E';    //  Equal
    case 10: return'Q';   // // Not equal
    case 11: return'>';   // / Greater
    case 12 : return'<';    //  Less
    case 13 : return'G';    // / Greater or equal
    case 14 : return'L';    // / Less or equal

    case 15: return'O';   // // Or
    case 16 : return'A';    // / And
    case 17 : return'N';    //  Not

    case 18 : return'(';    //  Parenthesis left
    case 19 : return')';    //  Parenthesis right

    case 20: return'i';      // / ID
    case 21 : return'f';      //  function
    case 22 : return'$';    //  Dollar
    case 23: return'n';      // /
    case 24: return'=';
    case 25: return'<';
  }
}

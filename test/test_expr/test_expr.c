
#include "expr.h"


#include <stdio.h>
#include <stdlib.h>
#include "print_stack.h"

int main ()
{
    sourceFile= fopen("test.txt","r");

    if(sourceFile == NULL)
    {
        printf("neotvoreny subor\n");
        return -1;
    }


    int result=fn_expresion(sourceFile);

    return result;
}

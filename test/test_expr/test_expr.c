
#include "expr.h"
#include "struct.h"

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

    token= malloc(sizeof(T_token));
    Init_token(token);
    int result=fn_expression(sourceFile,in_assign);

    free_token(token);
    free(token);
    fclose(sourceFile);

    return result;
}

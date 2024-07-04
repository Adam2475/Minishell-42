#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

int main()
{
    char *input;

    input = readline("Enter your name: ");

    printf("Hello, %s!\n", input);

    free(input);

    return 0;
}

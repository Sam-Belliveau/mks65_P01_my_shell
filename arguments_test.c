/**
 * This program was made to test the ways in which
 * the arguments of the shell are being passed into 
 * the program being run.
 */

#include <stdio.h>

int main(int argc, char** argv)
{
    int i;

    printf("# of Args: %d\n", argc);
    printf("Arguments:\n");
    
    for(i = 0; i < argc; ++i)
    { printf("\t- \"%s\"\n", argv[i]); }
}
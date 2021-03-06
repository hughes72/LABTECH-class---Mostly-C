/*
    Kyle Hughes 2/15/14
    This program loops through the data in the file and prints
    the initials and numbers of the Presidents out one at a time.

 */
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {

    char inits[4];
    unsigned char num;
} prezinfo;

int main()
{
    prezinfo prez;
    FILE *saveit;
    int howmany;
    int i;
    if( ( saveit = fopen("prez.dat", "r") ) == NULL ) {
        perror("prez.dat");
        exit(1);
    }

    for(i = 0; i < sizeof(prezinfo)-1; i++) {
        if( (howmany = fread(&prez, sizeof(prezinfo), 1, saveit)) !=1) {
            if ( feof(saveit) != 0) {
                fprintf(stderr, "EOF on file. \n");
            } else {
                perror("prez.dat");
            }
            exit(1);
        }

        printf("File data: initials: %s, num: %d.\n", prez.inits, prez.num);
    }
    fclose(saveit);
    return 0;
}

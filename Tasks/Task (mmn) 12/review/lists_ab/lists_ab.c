/* This program gets chars. print them, and count how many chars the user entered, and count how many from them are alphanumeric */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    int i;
    char c; /* represents char the user entered */
    int lenAll = 0; /* the amount of the chars*/
    int lenAlphanumeric = 0; /* the amount of the alphanumeric chars*/
    int size = 16 * sizeof(char); /* the length of the var 'txt' */
    char *txt = (char *)(malloc(size)); /* save the chars that the user entered */
    if (txt == NULL) {
        printf("Not enough memory!");
        return -1;
    }

    /* gets the chars from the user */
    printf("Enter chars:\n");
    while ((c = getchar()) != EOF) { 
        if (lenAll == size) { /* if the var 'txt' went full */
/**********************************************
* Required to avoid 'magic number'! 
* Numbers need to define as constant (-5)
***********************************************/           
			txt = (char *)(realloc(txt, size + (16 * sizeof(char)))); /* resize the var 'txt' by 16 new cells */
            if (txt == NULL) {
                printf("Not enough memory!");
                return -1;
            }
            size += 16 * sizeof(char); /* change size to the current size */
        }
        txt[lenAll] = c;
        if (isalnum(c)) /* if the char is alphanumeric */
            lenAlphanumeric++;
        lenAll++;
    }

    /* print the user's chars */
    printf("\n\nThe chars are (separate by vertical bar):");
    for (i = 0; i < lenAll; i++) {
        if (i % 5 == 0)
            printf("\n|");
        if (txt[i] == '\n')
            printf("\\n|");
        else if (txt[i] == '\t')
            printf("\\t|");
        else
            printf("%c|", txt[i]);
    }
    printf("\n\n");

    printf("There are: %d alphanumeric characters.\n", lenAlphanumeric);
    printf("There are: %d characters.\n", lenAll);
    free(txt);
    return 0;
}

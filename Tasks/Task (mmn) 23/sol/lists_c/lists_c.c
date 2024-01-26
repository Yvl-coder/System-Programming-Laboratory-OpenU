/* this program received a file name, store the characters from it in linked list, and print the characters. */
#include "lists_c.h"


int main(int argc, char *argv[]) {
    FILE *fptr; /* pointer to the file */
    int i;
    node head; /* the head of the linked list */
    node *curr; /* a pointer that moves in the linked list */
    char c; /* represents char from the file */

    if (argc == 1) {
        printf("No file name argument received!\n");
        return FAILED;
    }
    if (argc > 2)
        printf("Received more then one argument! Taking only the first!\n");
	
	/* open the file */
    fptr = fopen(argv[1], "r");
    i = 0;
	
	/* initial the head of the linked list values */
    strcpy(head.txt, "\0\0\0\0\0");
    head.next = NULL;
	
	/* curr point to the head */
    curr = &head;

    if (fptr == NULL) {
        printf("Error occurred! Cannot open the file name received!\n");
        return FAILED;
    }

	/* save the characters from the file in the linked list */
    while ((c = (char)fgetc(fptr)) != EOF) {
        if (i == LEN_TXT) { /* if 'txt' attribute in the element in the linked list is full, create a new element */
            curr->next = (node *)malloc(sizeof(node));
            if (curr->next == NULL) {
                printf("Error occurred! Restart the program.\n");
                return FAILED;
            }
            curr = curr->next; /* curr point to the next element (the element that just created) */
            strcpy(curr->txt, "\0\0\0\0\0"); /* initial the txt attribute in the new element*/
            i = 0;
        }
        curr->txt[i] = c;
        i++;
    }

    printf("The chars are (seperated by vertical bar):\n");
    curr = &head;
    while (curr != NULL) { /* print the characters */
        for (i = 0; i < LEN_TXT-1 && curr->txt[i] != '\0'; i++)
            printf("%c|", curr->txt[i]);
        if (curr->txt[i] != '\0') printf("%c\n", curr->txt[i]);
        curr = curr->next;
    }

	/* free the linked list elements created with 'malloc' */
    FREE(head.next);
    return 0;
}

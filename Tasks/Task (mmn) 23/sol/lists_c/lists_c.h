#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_TXT 5 /* represents the len of the 'txt' attribute in the struct 'node' */
#define FAILED -1 /* return this value in 'main' function, if error occurred */

/* macro to free all the 'malloc' elements in the linked list */
#define FREE(start) {       \
    curr = start;           \
    while (curr != NULL) {  \
        node *tmp = curr;   \
        curr = curr->next;  \
        free(tmp);          \
    }                       \
}

/* the linked list */
typedef struct node {
    char txt[LEN_TXT]; /* store characters */
    struct node *next; /* the next element in the linked list */
} node;

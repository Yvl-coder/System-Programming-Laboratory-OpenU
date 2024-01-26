#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* boolean */
typedef int boolean;
#define TRUE 1
#define FALSE 0

/* maximum line length: 80, and plus 1 for the character '\n' */
#define MAX_LINE_LENGTH 81

/* the maximum length of label / macro name */
#define MAX_LABEL_MACRO_LENGTH 30

/* buffer for the data and the instruction arrays */
#define BUFF 2000

/* the load location of the program at the memory */
#define LOAD_LOCATION 100

/* the length of a word in the memory */
#define WORD_LENGTH 10

/* the minimum value that number can be in the code, calculate in this way: there are 10 bits and the most left bit is only for positive/negetive represention.
 * then we will take the most left bit as 1, and the rest will be 0's, we got the number 1000000000 in binary, and this equal to -512 in decimal */
#define MIN_VALUE (-512)

/* the maximum value that number can be in the code, calculate in this way: there are 10 bits and the most left bit is only for positive/negetive represention.
 * then we will take the most left bit as 0, and the rest will be 1's, we got the number 0111111111 in binary, and this equal to 511 in decimal */
#define MAX_VALUE 511

/* constant that store the case of return to string that is not register name in the function "register_index" in "general.c". */
#define NOT_REGISTER 8

/* check if dynamic memory allocated succeeded, if not - print error message and exit the program */
#define IS_ALLOCATE_MEMORY_OK(ptr) {                 \
    if ((ptr) == NULL) {                             \
        printf("Memory error! Exit the program.\n"); \
        exit(EXIT_FAILURE);                          \
    }                                                \
}

/* struct that store a name of the file name and its length */
typedef struct {
    char *name; /* the file name */
    int len; /* the length of the file name */
} file_name_values;

/* struct that represents a word at the memory - 10 bits */
typedef struct {
    unsigned int val: WORD_LENGTH; /* the value of the word - binary */
} word_data;

/* struct that represents a word at the memory - 10 bits */
typedef struct {
    unsigned int val: WORD_LENGTH; /* the value of the word - binary */
    boolean is_register; /* if the current word is register addressing */
    char label_name[MAX_LABEL_MACRO_LENGTH]; /* if the current word require an address of label, this field save the name of the label. else, the field will be "". */
    int index_line; /* the index of the line that the current word created from */
} word_instruction;

/* linked list of labels */
typedef struct label {
    char name[MAX_LABEL_MACRO_LENGTH]; /* the name of the label, according to the maman - the maximum length is 30 */
    int value; /* the value of the label - at the memory */
    boolean isData; /* TRUE if the sentence is directive sentence, FALSE otherwise */
    boolean isExtern; /* TRUE if the label is with extern guidance, FALSE otherwise */
    boolean isEntry; /* TRUE if the label is with entry guidance, FALSE otherwise */
    boolean isCode; /* TRUE if the label is including opcode (in other words, instructional sentence), FALSE otherwise */
    struct label *next; /* the next node in the linked list */
} label;


/* this function remove the blank characters from the start of the string  of the parameter line until first non-blank character.
 *
 * parameter:
 * ----------
 * line: the string that the blank characters deleted from. the function assumes that the parameter includes the '\n' character at the end of the string.
 *
 * return:
 * -------
 * the parameter line without the blank characters from the start until non-blank character.
 */
char *ignore_white_characters(char *line);

/* this function fill the parameter content with the non-blank characters from the start of the parameter line until first blank character.
 *
 * parameter:
 * ----------
 * line: the string that the non-blank characters taken. the function assumes that the parameter includes the '\n' character at the end of the string.
 * content: the string that fill with the non-blank characters that taken from the parameter line.
 *
 * return:
 * -------
 * the parameter line without the non-blank characters from the start until blank character.
 */
char *get_until_white_characters(char *line,  char *content);

/* this function check if label / macro name is valid according to the maman.
 *
 * parameter:
 * ----------
 * name: the name of the label / macro.
 *
 * return:
 * -------
 * if label / macro name is valid according to the maman.
 */
boolean is_valid_label_macro_name(char *name);

/* this function return array of strings that each string is opcode name.
 *
 * parameter:
 * ----------
 * there are no parameters.
 *
 * return:
 * -------
 * array of strings that each string is opcode name.
 */
char **get_opcode_names();

/* this function check if string is a register name.
 *
 * parameter:
 * ----------
 * str: the string that checked if it is register name.
 *
 * return:
 * -------
 * the index of the register. return 8 if it is not a register name.
 */
int register_index(char *str);

#endif
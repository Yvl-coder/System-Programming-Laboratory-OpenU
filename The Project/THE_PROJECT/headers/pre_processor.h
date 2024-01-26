#ifndef PRE_PROCESSOR_H
#define PRE_PROCESSOR_H

#include "general.h"

#define MAX_LINES_MACRO 10 /* each macro can contain maximum of 10 lines */
#define MACRO_DEFINITION "macro" /* start definition of macro */
#define END_MACRO_DEFINITION "endmacro" /* end definition of macro */

/* linked list of macros */
typedef struct macro {
    char name[MAX_LABEL_MACRO_LENGTH]; /* the name of the macro, according to the maman - the maximum length is 30 */
    char content[MAX_LINES_MACRO * MAX_LINE_LENGTH]; /* the content of the macro */
    struct macro *next; /* the next node in the linked list */
} macro;

/* this function execute the pre-processor process.
 *
 * parameter:
 * ----------
 * fptr: pointer to the ".as" file.
 * file_name: the name of the file (the ".as" file).
 *
 * return:
 * -------
 * TRUE if error occurred while the pre-processor process, FALSE otherwise.
 */
boolean pre_processor(FILE *fptr, file_name_values file_name);

/* this function handle single line from the ".as" file and fill the ".am" file according to the pre-processor in the maman.
 *
 * parameter:
 * ----------
 * head: the head of the linked list of macros.
 * line: single line from the ".as" file.
 * index_line: the index of the parameter "line" at the ".as" file.
 * to_write: pointer to the ".am" file.
 * file_name: the name of the ".as" file.
 *
 * return:
 * -------
 * void function.
 */
void line_analysis_macros(macro *head, char *line, int index_line, FILE *to_write, file_name_values file_name);

/* this function read a line from file (used only at the pre processor).
 *
 * parameter:
 * ----------
 * fptr: pointer to file.
 * line: will store the line content.
 * index_line: the index of the specific line in the file.
 * file_name: the name of the file.
 *
 *
 * return:
 * -------
 * NULL if the file end, else the line content.
 */
char *read_line_from_file(FILE *fptr, char *line, int index_line, file_name_values file_name);

/* this function return a pointer to a macro from the linked list of macros with the same name as the parameter macro_name.
 * if there is not a macro with the same name as the parameter macro_name, the function returns NULL.
 *
 * parameter:
 * ----------
 * head: pointer to the head of the linked list of macros.
 * macro_name: the name of the macro that searched in the linked list of macros.
 *
 * return:
 * -------
 * pointer to the macro if it existed in the linked list of macros, NULL otherwise.
 */
macro *search_macro(macro *head, char *macro_name);

/* this function add a new macro (node) to the linked list of macros and check if it is a valid macro.
 *
 * parameter:
 * ----------
 * head: pointer to the head of the linked list of macros.
 * line: line from the ".as" file.
 * index_line: the index of the parameter line at the ".as" file.
 * file_name: the name of the ".as" file.
 *
 * return:
 * -------
 * void function.
 */
void add_macro_to_list(macro *head, char *line, int index_line, file_name_values file_name);

/* this function return the last **defined** macro at the linked list of macros.
 *
 * parameter:
 * ----------
 * head: pointer to the head of the linked list of macros.
 *
 * return:
 * -------
 * the last **defined** macro at the linked list of macros.
 */
macro *find_last_macro(macro *head);

/* this function check if the line is a close to macro definition.
 *
 * parameter:
 * ----------
 * line: line from the ".as" file.
 *
 * return:
 * -------
 * TRUE if the line is a close to macro definition, FALSE otherwise.
 */
boolean is_end_of_macro(char *line);

/* this function return if error occurred while the pre-processor process.
 *
 * parameter:
 * ----------
 * is_error: when the function called with a TRUE value to this parameter, then the function "know" that there was an error.
 *           when the function called with a FALSE value to this parameter, then the function just return if there was an error or not.
 * reset: when the function called with a TRUE value to this parameter, then the function reset the flag of error to FALSE.
 *        when the function called with a FALSE value to this parameter, do nothing.
 *
 * return:
 * -------
 * TRUE if error occurred while the pre-processor process, FALSE otherwise.
 */
boolean is_error_occurred_macro(boolean is_error, boolean reset);

/* this function free the allocated memory at the "pre_processor.c" and the pointers to the ".as" and the ".am" files.
 *
 * parameter:
 * ----------
 * fptr: pointer to the ".as" file.
 * to_write: pointer to the ".am" file.
 * head: the head of the linked list of macros.
 *
 * return:
 * -------
 * void function.
 */
void free_allocated_memory_pre_processor(FILE *fptr, FILE *to_write, macro *head);

#endif

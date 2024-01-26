#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "pre_processor.h"
#include "first_pass.h"
#include "second_pass.h"

/* this constant is used to reset the error flag (at the: pre-processor, first pass and second pass) in the program to FALSE */
#define RESET_ERROR_FUNCTIONS {                 \
    is_error_occurred_macro(FALSE, TRUE);       \
    is_error_occurred_first_pass(FALSE, TRUE);  \
    is_error_occurred_second_pass(FALSE, TRUE); \
}

/* this function handle the parameters that the program got from the command line.
 *
 * parameter:
 * ----------
 * argc: the amount of the parameters from the command line.
 * argv: the value of the parameters from the command line.
 *
 * return:
 * -------
 * void function.
 */
void extract_files_from_command_line(int argc, char *argv[]);

/* this function handle (compile) a signal file from the command line.
 *
 * parameter:
 * ----------
 * fptr: pointer to the ".as" file.
 * file_name: the name of the file.
 *
 * return:
 * -------
 * void function.
 */
void compile(FILE *fptr, file_name_values *file_name);

/* this function free all the allocated memory that didn't free in the first and the second pass.
 *
 * parameter:
 * ----------
 * file_name: pointer to the ".as" file.
 * IC: the IC.
 * DC: the DC.
 * head: the head of the linked list.
 * after_pre_processor: pointer to the ".am" file (in read mode).
 *
 * return:
 * -------
 * void function.
 */
void free_allocated_memory(file_name_values *file_name, int *IC, int *DC, label *head, FILE *after_pre_processor);

#endif
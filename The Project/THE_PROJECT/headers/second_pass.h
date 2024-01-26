#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "general.h"

#define E 1 /* the value of the E bit from the A,R,E bits */
#define R 2 /* the value of the R bit from the A,R,E bits */

/* the max length that the representation of number in binary of max of 10 bits (unsigned), can be in base 32.
 * the max number in binary of 10 bits (unsigned) is 1111111111 which is "vv" (without the quotation marks) in base 32.
 * that mean the max length of base 32 representation in this program is 2.
 */
#define MAX_BASE_32_LENGTH 2

/* this macro reverse a string */
#define REVERSE_STRING(str) {                     \
    int index;                                    \
    int length;                                   \
    char temp;                                    \
                                                  \
    length = strlen((str));                       \
    for (index = 0; index < length/2; index++) {  \
        temp = (str)[index];                      \
        (str)[index] = (str)[length-index-1];     \
        (str)[length-index-1] = temp;             \
    }                                             \
}

/* this function execute the second pass process (with level 17 from the first process algorithm from the maman).
 *
 * parameter:
 * ----------
 * after_pre_processor: pointer to the ".am" file.
 * file_name: the name of the file with ".am" extension.
 * head: pointer to the head of the linked list of labels.
 * IC: instruction counter.
 * DC: data counter.
 * instruction: array of the instructions in the ".am" file.
 * data: array of the data in the ".am" file.
 *
 * return:
 * -------
 * TRUE if error occurred while the second pass process, FALSE otherwise.
 */
boolean second_pass(FILE *after_pre_processor, file_name_values file_name, label *head, int *IC, int *DC, word_instruction instruction[], word_data data[]);

/* this function update the value of the labels with the data sign on by IC.
 *
 * parameter:
 * ----------
 * head: pointer to the head of the linked list of labels.
 * IC: instruction counter.
 *
 * return:
 * -------
 * void function.
 */
void update_data_value(label *head, int *IC);

/* this function handle single line from the ".am" file according to the second pass algorithm in the maman.
 *
 * parameter:
 * ----------
 * head: the head of the linked list of labels.
 * line: single line from the ".am" file.
 * index_line: the index of the parameter "line" at the ".am" file.
 * file_name: the name of the file with ".am" extension.
 *
 * return:
 * -------
 * void function.
 */
void line_analysis_second_pass(label *head, char *line, int index_line, file_name_values file_name);

/* this function handle ".entry" definition.
 *
 * parameter:
 * ----------
 * head: pointer to the head of the linked list of labels.
 * line: line from the ".am" file.
 * index_line: the index of the line of the parameter line.
 * file_name: the name of the file with ".am" extension.
 *
 * return:
 * -------
 * void function.
 */
void handle_entry_definition(label *head, char *line, int index_line, file_name_values file_name);

/* this function search label in the linked list of labels.
 *
 * parameter:
 * ----------
 * head: pointer to the head of the linked list of labels.
 * label_name: the name of the label that the function search.
 *
 * return:
 * -------
 * pointer to the node in the linked list that represents the parameter label_name, if it exists. NULL otherwise.
 */
label *search_label(label *head, char *label_name);


/* this function complete the machine code that didnt done in the first pass - write the addresses of the labels.
 *
 * parameter:
 * ----------
 * head: pointer to the head of the linked list of labels.
 * IC: instruction counter.
 * instruction: array of the instructions in the ".am" file.
 * file_name: the name of the file with ".am" extension.
 *
 * return:
 * -------
 * void function.
 */
void complete_machine_code_from_first_pass(label *head, int *IC, word_instruction instruction[], file_name_values file_name);

/* this function create and fill the ".ob" file.
 *
 * parameter:
 * ----------
 * file_name: the name of the file with ".am" extension.
 * IC: instruction counter.
 * DC: data counter.
 * instruction: array of the instructions in the ".am" file.
 * data: array of the data in the ".am" file.
 *
 * return:
 * -------
 * void function.
 */
void create_object_file(file_name_values file_name, int *IC, int *DC, word_instruction instruction[], word_data data[]);

/* this function create and fill the ".ent" file.
 *
 * parameter:
 * ----------
 * file_name: the name of the file with ".am" extension.
 * head: pointer to the head of the linked list of labels.
 *
 * return:
 * -------
 * void function.
 */
void create_entries_file(file_name_values *file_name, label *head);

/* this function create and fill the ".ob" file.
 *
 * parameter:
 * ----------
 * file_name: the name of the file with ".ent" extension.
 * IC: instruction counter.
 * instruction: array of the instructions in the ".am" file.
 *
 * return:
 * -------
 * void function.
 */
void create_externals_file(file_name_values file_name, int *IC, word_instruction instruction[]);

/* this function convert a number in decimal base to the special 32 base.
 *
 * parameter:
 * ----------
 * x: the number that will be converted to the special base 32.
 * x_in_special_base_32: the parameter x in the special base 32 representation.
 * return:
 * -------
 * the representation of the parameter x in the special base 32 representation.
 */
char *convert_to_special_base_32(unsigned int x, char x_in_special_base_32[]);

/* this function return if error occurred while the second pass process.
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
 * TRUE if error occurred while the second pass process, FALSE otherwise.
 */
boolean is_error_occurred_second_pass(boolean is_error, boolean reset);

#endif
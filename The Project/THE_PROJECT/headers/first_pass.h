#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "general.h"

#define SOURCE_OPERAND_SHIFT 4 /* how much to shift if the operand is source operand */
#define TARGET_OPERAND_SHIFT 2 /* how much to shift if the operand is target operand */

#define DIRECT_ADDRESSING 1 /* the value of the direct addressing */
#define ACCESS_TO_STRUCT_ADDRESSING 2 /* the value of the access to struct addressing */
#define DIRECT_REGISTER_ADDRESSING 3 /* the value of the direst register addressing */

#define IMMEDIATE_ADDRESSING_MIN_VALUE (-128) /* the min value that the code can use when using immediate addressing */
#define IMMEDIATE_ADDRESSING_MAX_VALUE 127 /* the max value that the code can use when using immediate addressing */

/* this function execute the first pass process (with level 17 from the first process algorithm from the maman).
 *
 * parameter:
 * ----------
 * after_pre_processor: pointer to the ".am" file.
 * file_name: the name of the file with ".am" extension.
 * IC: instruction counter.
 * DC: data counter.
 * instruction: array of the instructions in the ".am" file.
 * data: array of the data in the ".am" file.
 * head: pointer to the head of the linked list of labels.
 *
 * return:
 * -------
 * TRUE if error occurred while the first pass process, FALSE otherwise.
 */
boolean first_pass(FILE *after_pre_processor, file_name_values file_name, int *IC, int *DC, word_instruction instruction[], word_data data[], label *head);

/* this function handle single line from the ".am" file according to the second pass algorithm in the maman.
 *
 * parameter:
 * ----------
 * head: the head of the linked list of labels.
 * line: single line from the ".am" file.
 * index_line: the index of the parameter "line" at the ".am" file.
 * file_name: the name of the file with ".am" extension.
 * IC: instruction counter.
 * DC: data counter.
 * instruction: array of the instructions words in the ".am" file.
 * data: array of the data words in the ".am" file.
 *
 * return:
 * -------
 * void function.
 */
void line_analysis_first_pass(label *head, char *line, int index_line, file_name_values file_name, int *IC, int *DC, word_instruction instruction[], word_data data[]);

/* this function add a new label (node) to the linked list of labels and check if it is a valid label.
 *
 * parameter:
 * ----------
 * head: pointer to the head of the linked list of labels.
 * name: the name of the label.
 * value: the value of the label.
 * isData: if the label is data sentence.
 * isExtern: if the label is extern.
 * isEntry: if the label is entry.
 * isCode: if the label is command sentence.
 * index_line: the index of the parameter line at the ".am" file.
 * file_name: the name of the file with ".am" extension.
 *
 * return:
 * -------
 * void function.
 */
void add_label_to_list(label *head, char *name, int value, boolean isData, boolean isExtern, boolean isEntry, boolean isCode, int index_line, file_name_values file_name);

/* this function handle ".data" definition.
 *
 * parameter:
 * ----------
 * line: line from the ".am" file.
 * index_line: the index of the line of the parameter line.
 * file_name: the name of the file with ".am" extension.
 * DC: data counter.
 * data: array of the data in the ".am" file.
 *
 * return:
 * -------
 * void function.
 */
void handle_data_definition(char *line, int index_line, file_name_values file_name, int *DC, word_data data[]);

/* this function handle ".string" definition.
 *
 * parameter:
 * ----------
 * line: line from the ".am" file.
 * index_line: the index of the line of the parameter line.
 * file_name: the name of the file with ".am" extension.
 * DC: data counter.
 * data: array of the data in the ".am" file.
 * called_from_struct: if the function called from the function handle_struct_definition.
 *
 * return:
 * -------
 * void function.
 */
void handle_string_definition(char *line, int index_line, file_name_values file_name, int *DC, word_data data[], boolean called_from_struct);

/* this function handle ".struct" definition.
 *
 * parameter:
 * ----------
 * line: line from the ".am" file.
 * index_line: the index of the line of the parameter line.
 * file_name: the name of the file with ".am" extension.
 * DC: data counter.
 * data: array of the data in the ".am" file.
 *
 * return:
 * -------
 * void function.
 */
void handle_struct_definition(char *line, int index_line, file_name_values file_name, int *DC, word_data data[]);

/* this function handle ".extern" declaration.
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
void handle_extern_declaration(label *head, char *line, int index_line, file_name_values file_name);

/* this function handle command sentence.
 *
 * parameter:
 * ----------
 * opcode_name: the name of the opcode.
 * line: line from the ".am" file.
 * index_line: the index of the line of the parameter line.
 * file_name: the name of the file with ".am" extension.
 * IC: instruction counter.
 * data: array of the instructions in the ".am" file.
 *
 * return:
 * -------
 * void function.
 */
void handle_opcode_declaration(char *opcode_name, char *line, int index_line, file_name_values file_name, int *IC, word_instruction instruction[]);

/* this function handle command sentence with opcode that get a single operand.
 *
 * parameter:
 * ----------
 * opcode_word_original: the original word from which the other words are derived.
 * opcode: the index of the opcode.
 * line: line from the ".am" file.
 * index_line: the index of the line of the parameter line.
 * file_name: the name of the file with ".am" extension.
 * IC: instruction counter.
 * data: array of the instructions in the ".am" file.
 * shift: how much to shift (<<), the value determined if the operand is source / target
 *
 * return:
 * -------
 * void function.
 */
void single_operand(word_instruction *opcode_word_original, int opcode, char *line, int index_line, file_name_values file_name, int *IC, word_instruction instruction[], int shift);

/* this function handle command sentence with opcode that get a two operands.
 *
 * parameter:
 * ----------
 * opcode_word_original: the original word from which the other words are derived.
 * opcode: the index of the opcode.
 * line: line from the ".am" file.
 * index_line: the index of the line of the parameter line.
 * file_name: the name of the file with ".am" extension.
 * IC: instruction counter.
 * data: array of the instructions in the ".am" file.
 *
 * return:
 * -------
 * void function.
 */
void two_operands(word_instruction *opcode_word_original, int opcode, char *line, int index_line, file_name_values file_name, int *IC, word_instruction instruction[]);

/* this function return if error occurred while the first pass process.
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
 * TRUE if error occurred while the first pass process, FALSE otherwise.
 */
boolean is_error_occurred_first_pass(boolean is_error, boolean reset);

#endif
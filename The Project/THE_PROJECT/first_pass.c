#include "headers/first_pass.h"

/* this constant is created specific to the file "first_pass.c", and called if error detected */
#define ERROR_OCCURRED_FIRST_PASS  {           \
    is_error_occurred_first_pass(TRUE, FALSE); \
    return;                                    \
}

boolean first_pass(FILE *after_pre_processor, file_name_values file_name, int *IC, int *DC, word_instruction instruction[], word_data data[], label *head) {
    char line[MAX_LINE_LENGTH]; /* string to store the line chars from the file */
    int index_line = 1; /* the current index of line in the file */

    memset(line, '\0', sizeof(line)); /* reset the var line array values to '\0' */
    while (fgets(line, MAX_LINE_LENGTH, after_pre_processor) != NULL) { /* go across the file */
        line_analysis_first_pass(head, line, index_line, file_name, IC, DC, instruction, data); /* analyze specific line in the file */
        memset(line, '\0', sizeof(line)); /* reset the var line array values to '\0' */
        index_line++; /* increase the line index by 1 */
    }

    /* return if error occurred at the first pass */
    return is_error_occurred_first_pass(FALSE, FALSE);
}


void line_analysis_first_pass(label *head, char *line, int index_line, file_name_values file_name, int *IC, int *DC, word_instruction instruction[], word_data data[]) {
    boolean label_definition = FALSE; /* if the line is include label definition */
    char content[MAX_LINE_LENGTH]; /* the content after blank characters at the line */
    char label_name[MAX_LABEL_MACRO_LENGTH]; /* if there is a label definition - this var store the label name */

    line = ignore_white_characters(line);
    if (line[0] == ';' || line[0] == '\n') { /* check if the line is a comment line or only white characters */
        return;
    }

    memset(content, '\0', MAX_LINE_LENGTH);
    line = get_until_white_characters(line, content); /* fill the array content with the non-blank characters at the line */
    if (content[strlen(content)-1] == ':') {/* check if there is label definition */
        label_definition = TRUE;

        content[strlen(content)-1] = '\0'; /* delete ':' from the label name */
        if (!is_valid_label_macro_name(content)) { /* check if the label has invalid name */
            printf("Error in line %d at \"%s\", the label \"%s\" has invalid name!\n", index_line, file_name.name, content);
            ERROR_OCCURRED_FIRST_PASS
        }

        strcpy(label_name, content); /* save the label name in the var label_name */
        if (label_name[0] == '\0') { /* if the label is empty */
            printf("Error in line %d at \"%s\", empty label!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }

        line = ignore_white_characters(line);
        memset(content, '\0', MAX_LINE_LENGTH);
        line = get_until_white_characters(line, content);

        if (strlen(content) == 0) { /* if there is **only** label definition */
            printf("Error in line %d at \"%s\", the label \"%s\" dont associated with directive sentence or command sentence!\n", index_line, file_name.name, label_name);
            ERROR_OCCURRED_FIRST_PASS
        }
    }

    if (content[strlen(content)-1] == ',') {
        printf("Error in line %d at \"%s\", extra comma!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    if (!strcmp(content, ".data")) { /* if this is .data sentence */
        if (label_definition) /* if there was label definition */
            add_label_to_list(head, label_name, *DC, TRUE, FALSE, FALSE, FALSE, index_line, file_name); /* add the label to the linked list with data sign */

        handle_data_definition(line, index_line, file_name, DC, data);
    } else if (!strcmp(content, ".string")) { /* if this is .string sentence */
        if (label_definition) /* if there was label definition */
            add_label_to_list(head, label_name, *DC, TRUE, FALSE, FALSE, FALSE, index_line, file_name); /* add the label to the linked list with data sign */

        handle_string_definition(line, index_line, file_name, DC, data, FALSE);
    } else if (!strcmp(content, ".struct")) { /* if this is .struct sentence */
        if (label_definition) /* if there was label definition */
            add_label_to_list(head, label_name, *DC, TRUE, FALSE, FALSE, FALSE, index_line, file_name); /* add the label to the linked list with data sign */

        handle_struct_definition(line, index_line, file_name, DC, data);
    } else if (!strcmp(content, ".entry")) { /* if this is .entry sentence */
        if (label_definition) { /* if there was label definition */
            if (!is_valid_label_macro_name(label_name)) { /* if the label name is invalid */
                printf("Error in line %d at \"%s\", the label \"%s\" has invalid name!\n", index_line, file_name.name, label_name);
                ERROR_OCCURRED_FIRST_PASS
            }
            printf("Warning in line %d at \"%s\", the label \"%s\" is meaningless!\n", index_line, file_name.name, label_name);
        }
    } else if (!strcmp(content, ".extern")) { /* if this is .extern sentence */
        if (label_definition) { /* if there was label definition */
            if (!is_valid_label_macro_name(label_name)) { /* if there was label definition */
                printf("Error in line %d at \"%s\", the label \"%s\" has invalid name!\n", index_line, file_name.name, label_name);
                ERROR_OCCURRED_FIRST_PASS
            }
            printf("Warning in line %d at \"%s\", the label \"%s\" is meaningless!\n", index_line, file_name.name, label_name);
        }
        handle_extern_declaration(head, line, index_line, file_name);
    } else { /* if this is command sentence */
        if (label_definition) /* if there was label definition */
            add_label_to_list(head, label_name, *IC, FALSE, FALSE, FALSE, TRUE, index_line, file_name); /* add the label to the linked list with code sign */

        handle_opcode_declaration(content, line, index_line, file_name, IC, instruction);
    }
}

void add_label_to_list(label *head, char *name, int value, boolean isData, boolean isExtern, boolean isEntry, boolean isCode, int index_line, file_name_values file_name) {
    label *curr = head;

    /* get the last node */
    while (curr->next != NULL) {
        if (!strcmp(curr->name, name)) { /* if the label is already defined */
            printf("Error in line %d at \"%s\", the label \"%s\" already defined!\n", index_line, file_name.name, name);
            ERROR_OCCURRED_FIRST_PASS
        }
        curr = curr->next;
    }

    /* fill the fields of the node with the label info */
    strcpy(curr->name, name);
    curr->value = value + LOAD_LOCATION;
    curr->isData = isData;
    curr->isExtern = isExtern;
    curr->isEntry = isEntry;
    curr->isCode = isCode;

    /* create a new node */
    curr->next = (label *)malloc(sizeof(label));
    IS_ALLOCATE_MEMORY_OK(curr->next)
    curr = curr->next;
    curr->value = 0;
    curr->isData = FALSE;
    curr->isExtern = FALSE;
    curr->isEntry = FALSE;
    curr->isCode = FALSE;
    curr->next = NULL;
}

void handle_data_definition(char *line, int index_line, file_name_values file_name, int *DC, word_data data[]) {
    int tmp; /* store the value of the numbers in the .data definition */
    boolean came_to_end_from_comma = TRUE; /* if reach the end of the line and there was a comma before, example: line=" 3, 4, \n" */
    int len; /* the length of the var line */

    line = ignore_white_characters(line);
    if (line[0] == '\n') { /* if there are not arguments in the definition */
        printf("Error in line %d at \"%s\", \".data\" definition didn't get arguments!\n", index_line,file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }
    if (line[0] == ',') { /* if there is a comma before the first argument */
        printf("Error in line %d at \"%s\", extra comma!\n", index_line,file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    while (line[0] != '\n') { /* go over the line */
        line = ignore_white_characters(line);
        len = (int)strlen(line);
        tmp = (int)strtol(line, &line, 10); /* extract the number value from the definition */

        if (line[0] == ',' && len == strlen(line)) { /* if double commas written */
            printf("Error in line %d at \"%s\", double commas writen between the arguments!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }
        if (line[0] != ' ' && line[0] != '\t' && line[0] != ',' && line[0] != '\n') { /* if there is not a number in the .data definition */
            printf("Error in line %d at \"%s\", illegal argument got! \".data\" definition gets only numbers (integers)!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }

        if (MIN_VALUE <= tmp && tmp <= MAX_VALUE) { /* if number in the definition is in the limits of number can be */
            data[(*DC)].val = tmp; /* add the value of the data with the current number from the definition */
            (*DC)++;
        } else { /* if the number exceeds the limits */
            printf("Error in line %d at \"%s\", the number: %d exceeds the limits! the limits are: -512 to 511.\n", index_line, file_name.name, tmp);
            ERROR_OCCURRED_FIRST_PASS
        }

        line = ignore_white_characters(line);
        if (line[0] != ',' && line[0] != '\n' && isdigit(line[0])) { /* if comma is missing */
            printf("Error in line %d at \"%s\", missing comma!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        } else if (!isdigit(line[0]) && line[0] != '+' && line[0] != '-' && line[0] != ',' && line[0] != '\n') { /* if there is an extraneous text */
            printf("Error in line %d at \"%s\", extraneous text!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        } else if (line[0] != '\n') { /* if its comma and the line didnt end */
            line++;
        } else if (line[0] == '\n') { /* if the line ended */
            came_to_end_from_comma = FALSE; /* there must be a comma because the if's above */
        }
    }

    if (came_to_end_from_comma) { /* if there was a situation like this (only an example): "2,3,4 , ". there is an extra comma in the end */
        printf("Error in line %d at \"%s\", extra comma!\n", index_line,file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }
}

void handle_string_definition(char *line, int index_line, file_name_values file_name, int *DC, word_data data[], boolean called_from_struct) {
    boolean close_quotation_marks = FALSE; /* if there are a close quotation marks, example: line="\"qwe\"" */

    line = ignore_white_characters(line);
    if (line[0] == '\n') { /* if there are no arguments */
        if (called_from_struct) {
            printf("Error in line %d at \"%s\", \".struct\" definition didn't get a string argument!\n", index_line,file_name.name);
        } else {
            printf("Error in line %d at \"%s\", \".string\" definition didn't get argument!\n", index_line, file_name.name);
        }
        ERROR_OCCURRED_FIRST_PASS
    }
    if (line[0] != '"') { /* if there are not opening quotation marks */
        if (called_from_struct) {
            printf("Error in line %d at \"%s\", the string field in \".struct\" definition must open with quotation marks!\n", index_line,file_name.name);
        } else {
            printf("Error in line %d at \"%s\", definition of \".string\" must open with quotation marks!\n", index_line, file_name.name);
        }
        ERROR_OCCURRED_FIRST_PASS
    }
    line++; /* skip the opening quotation marks */

    while (line[0] != '\n') { /* go over the line */
        if (line[0] == '"') { /* if reach the closing quotation marks */
            close_quotation_marks = TRUE;

            line++; /* skip the closing quotation marks */
            line = ignore_white_characters(line);
            if (line[0] != '\n') { /* if there is extraneous text */
                printf("Error in line %d at \"%s\", extraneous text!\n", index_line, file_name.name);
                ERROR_OCCURRED_FIRST_PASS
            }

            data[(*DC)].val = '\0'; /* add zero to the data */
            (*DC)++;
        } else {
            data[(*DC)].val = (int)line[0]; /* add the value of the current char at the string definition */
            (*DC)++;

            line++; /* move to the next char */
        }
    }

    if (!close_quotation_marks) { /* if there were not a closing quotation marks */
        if (called_from_struct) {
            printf("Error in line %d at \"%s\", the string field in \".struct\" definition must close with quotation marks!\n", index_line,file_name.name);
        } else {
            printf("Error in line %d at \"%s\", definition of \".string\" must close with quotation marks!\n", index_line, file_name.name);
        }
        ERROR_OCCURRED_FIRST_PASS
    }
}

void handle_struct_definition(char *line, int index_line, file_name_values file_name, int *DC, word_data data[]) {
    int tmp; /* store the first field value (the number value) */
    int len; /* the len of the line */

    line = ignore_white_characters(line);
    if (line[0] == '\n') { /* if there is no arguments */
        printf("Error in line %d at \"%s\", \".struct\" definition didn't get arguments!\n", index_line,file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    len = (int)strlen(line);
    tmp = (int)strtol(line, &line, 10); /* extract the number value from the definition */

    if (len == strlen(line)) { /* if there is not a number in the definition */
        printf("Error in line %d at \"%s\", \".struct\" definition didn't get a number argument!\n", index_line,file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }
    if (line[0] != ' ' && line[0] != '\t' && line[0] != ',' && line[0] != '\n') { /* if not a number written */
        printf("Error in line %d at \"%s\", illegal argument got! first argument of \"struct\" must be a number (integer)!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    if (MIN_VALUE <= tmp && tmp <= MAX_VALUE) { /* if the number is in the limits */
        data[(*DC)].val = tmp;
        (*DC)++;
    } else { /* if the number exceeds the limits */
        printf("Error in line %d at \"%s\", the number: %d exceeds the limits! the limits are: -512 to 511.\n", index_line, file_name.name, tmp);
        ERROR_OCCURRED_FIRST_PASS
    }

    line = ignore_white_characters(line);
    if (line[0] == '"') {
        printf("Error in line %d at \"%s\", missing comma!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }
    if (line[0] != ',') {
        printf("Error in line %d at \"%s\", illegal text!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }
    line++; /* ignore the comma */

    handle_string_definition(line, index_line, file_name, DC, data, TRUE); /* fill the data with the second field - the string */
}

void handle_extern_declaration(label *head, char *line, int index_line, file_name_values file_name) {
    char label_name[MAX_LINE_LENGTH]; /* the name of the label that written in the definition */
    memset(label_name, '\0', MAX_LINE_LENGTH); /* reset array cells to '\0' */

    line = ignore_white_characters(line);
    if (line[0] == '\n') { /* if no argument got */
        printf("Error in line %d at \"%s\", \".extern\" must get 1 argument, actually got 0!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    line = get_until_white_characters(line, label_name);
    if (!is_valid_label_macro_name(label_name)) { /* check if the label has invalid name */
        printf("Error in line %d at \"%s\", the label \"%s\" has invalid name!\n", index_line, file_name.name, label_name);
        ERROR_OCCURRED_FIRST_PASS
    }

    line = ignore_white_characters(line);
    if (line[0] != '\n') { /* if there is an extra text after the label argument */
        printf("Error in line %d at \"%s\", extra text after label name (the argument)!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    /* add the label to the linked list */
    add_label_to_list(head, label_name, 0, FALSE, TRUE, FALSE, FALSE, index_line, file_name);
}

void handle_opcode_declaration(char *opcode_name, char *line, int index_line, file_name_values file_name, int *IC, word_instruction instruction[]) {
    int i;

    if (opcode_name[0] == ',') {
        printf("Error in line %d at \"%s\", extra comma!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    for (i = 0; i != 16 && strcmp(opcode_name, get_opcode_names()[i]) != 0; i++); /* find the opcode index */
    if (i == 16) { /* if the var opcode_name is not an opcode name */
        printf("Error in line %d at \"%s\", opcode doesnt exist!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    /* fill instruction array with the opcode */
    instruction[(*IC)].val = i << 6;
    instruction[(*IC)].is_register = FALSE;
    (instruction[(*IC)].label_name)[0] = '\0';
    instruction[(*IC)].index_line = index_line;
    (*IC)++;

    line = ignore_white_characters(line);
    if ((0 <= i && i <= 3) || i == 6) { /* if the opcode include 2 operands */
        two_operands(&instruction[(*IC)-1], i, line, index_line, file_name, IC, instruction);
    } else if (4 <= i && i <= 13) { /* if the opcode include single operand */
        single_operand(&instruction[(*IC)-1], i, line, index_line, file_name, IC, instruction, TARGET_OPERAND_SHIFT);
    } else { /* if the opcode include 0 operands */
        if (line[0] != '\n') {
            printf("Error in line %d at \"%s\", extraneous text!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }
    }
}

void single_operand(word_instruction *opcode_word_original, int opcode, char *line, int index_line, file_name_values file_name, int *IC, word_instruction instruction[], int shift) {
    char operand[MAX_LINE_LENGTH]; /* string that store the operands */
    int tmp; /* store the value of a number in immediate addressing */
    int register_i; /* store the index of register in direct register addressing */
    char *field_i; /* store the index of the field in access to struct addressing */
    char *after_number; /* store the rest of the line after the immediate addressing */


    if (line[0] == ',') {
        printf("Error in line %d at \"%s\", extra comma!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    memset(operand, '\0', MAX_LINE_LENGTH);
    line = get_until_white_characters(line, operand); /* fill content with operand */

    line = ignore_white_characters(line);
    if (line[0] == ',') {
        printf("Error in line %d at \"%s\", extra comma!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }
    if (line[0] != '\n') {
        printf("Error in line %d at \"%s\", extraneous text!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }
    if (strlen(operand) == 0) {
        printf("Error in line %d at \"%s\", \"%s\" must get a single operand, actually got 0!\n", index_line, file_name.name, get_opcode_names()[opcode]);
        ERROR_OCCURRED_FIRST_PASS
    }

    if (operand[0] == '#') { /* if the operand is an immediate addressing */
        tmp = (int)strtol(operand + 1, &after_number, 10); /* exctract the value of the number, example: "#2" - then tmp=2 */

        after_number = ignore_white_characters(after_number);
        if (strlen(after_number) != 0) { /* if the argument is not an integer number */
            printf("Error in line %d at \"%s\", immediate addressing must be a number (integer)!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }
        if (!(IMMEDIATE_ADDRESSING_MIN_VALUE <= tmp && tmp <= IMMEDIATE_ADDRESSING_MAX_VALUE)) { /* if the number exceeds the limits */
            printf("Error in line %d at \"%s\", the number exceeds the limits! the limits are: -128 to 127.!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }

        if ((shift == SOURCE_OPERAND_SHIFT && (0 <= opcode && opcode <= 3)) || (shift == TARGET_OPERAND_SHIFT && (opcode == 1 || opcode == 12))) { /* if the command can get an immediate addressing operand */
            /* opcode_word_original->val |= (0 << shift); (add to the value of the original opcode word the immediate addressing - since its 0, this line is unnecessary) */
            instruction[(*IC)].val = tmp << 2; /* create a word in the instruction that store the value of the number (and also the A,R,E - in this case its A) */
            instruction[(*IC)].is_register = FALSE;
            (instruction[(*IC)].label_name)[0] = '\0';
            instruction[(*IC)].index_line = index_line;
            (*IC)++;
        } else { /* if the command cant get an immediate addressing operand */
            printf("Error in line %d at \"%s\", illegal use of immediate addressing!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }
    } else if ((register_i = register_index(operand)) != NOT_REGISTER) { /* if the operand is a direct register addressing */
        if ((shift == SOURCE_OPERAND_SHIFT && (0 <= opcode && opcode <= 3)) || (shift == TARGET_OPERAND_SHIFT && (0 <= opcode && opcode <= 13))) { /* if the command can get a direct register addressing operand */
            if (instruction[(*IC)-1].is_register) { /* check if the previous word is a register addressing (by the way, the expression "(*IC)-1" will always be equal or bigger than 0) */
                opcode_word_original->val |= (DIRECT_REGISTER_ADDRESSING << shift); /* add to the value of the original opcode word the direct register addressing */
                instruction[(*IC)-1].val |= (register_i << 2); /* merge to the previous word the value of the current register */
            } else {
                opcode_word_original->val |= (DIRECT_REGISTER_ADDRESSING << shift); /* add to the value of the original opcode word the direct register addressing */
                if (shift == SOURCE_OPERAND_SHIFT) { /* if this is a source operand */
                    instruction[(*IC)].val = register_i << 6;
                } else {
                    instruction[(*IC)].val = register_i << 2;
                }
                instruction[(*IC)].is_register = TRUE;
                (instruction[(*IC)].label_name)[0] = '\0';
                instruction[(*IC)].index_line = index_line;
                (*IC)++;
            }
        } else { /* if the command cant get a direct register addressing operand */
            printf("Error in line %d at \"%s\", illegal use of direct register addressing!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }
    } else if (operand[strlen(operand)-2] == '.') { /* if the operand is an access to struct addressing */
        field_i = &operand[strlen(operand)-1]; /* save the text after the '.', example: content="AA.2 w" -> field_i="2 w"*/
        operand[strlen(operand)-2] = '\0'; /* change the string to be without the field_i and the dot */

        if ((shift == SOURCE_OPERAND_SHIFT && ((0 <= opcode && opcode <= 3) || opcode == 6)) || (shift == TARGET_OPERAND_SHIFT && (0 <= opcode && opcode <= 13))) { /* if the command can get an access to struct addressing operand */
            opcode_word_original->val |= (ACCESS_TO_STRUCT_ADDRESSING << shift);
            instruction[(*IC)].val = 0;
            instruction[(*IC)].is_register = FALSE;
            strcpy(instruction[(*IC)].label_name, operand);
            instruction[(*IC)].index_line = index_line;
            (*IC)++;
            if (!is_valid_label_macro_name(operand)) { /* if the label name is not valid */
                printf("Error in line %d at \"%s\", operand is illegal name of label!\n", index_line, file_name.name);
                ERROR_OCCURRED_FIRST_PASS
            }
        } else { /* if the command cant get an access to struct addressing operand */
            printf("Error in line %d at \"%s\", illegal use of access to struct addressing!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }

        if (field_i[0] == '1' || field_i[0] == '2') { /* if the field that written is 1 or 2 */
            instruction[(*IC)].val = (field_i[0] == '1'? 1 : 2) << 2;
            instruction[(*IC)].is_register = FALSE;
            (instruction[(*IC)].label_name)[0] = '\0';
            instruction[(*IC)].index_line = index_line;
            (*IC)++;
            field_i++; /* inc the pointer by 1 */
            if (field_i[0] != '\0' && field_i[0] != '\t' && field_i[0] != ' ') { /* if there is more text in the field that written */
                printf("Error in line %d at \"%s\", illegal field number (must be \"1\" or \"2\")!\n", index_line, file_name.name);
                ERROR_OCCURRED_FIRST_PASS
            }

            /* check if there is an extraneous text */
            field_i = ignore_white_characters(field_i);
            if (field_i[0] != '\0') {
                printf("Error in line %d at \"%s\", extraneous text!\n", index_line, file_name.name);
                ERROR_OCCURRED_FIRST_PASS
            }
        } else {
            printf("Error in line %d at \"%s\", illegal field number (must be \"1\" or \"2\")!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }
    } else if ((shift == SOURCE_OPERAND_SHIFT && ((0 <= opcode && opcode <= 3) || opcode == 6)) || (shift == TARGET_OPERAND_SHIFT && (0 <= opcode && opcode <= 13))) { /* if the operand is a direct addressing */
        opcode_word_original->val |= (DIRECT_ADDRESSING << shift);
        instruction[(*IC)].val = 0;
        instruction[(*IC)].is_register = FALSE;
        strcpy(instruction[(*IC)].label_name, operand);
        instruction[(*IC)].index_line = index_line;
        (*IC)++;
        if (!is_valid_label_macro_name(operand)) { /* if the operand is valid name of label */
            printf("Error in line %d at \"%s\", operand is illegal name of label!\n", index_line, file_name.name);
            ERROR_OCCURRED_FIRST_PASS
        }
    } else { /* if the operand is illegal */
        printf("Error in line %d at \"%s\", \"%s\" is illegal operand for the command \"%s\"!\n", index_line, file_name.name, operand, get_opcode_names()[opcode]);
        ERROR_OCCURRED_FIRST_PASS
    }
}

void two_operands(word_instruction *opcode_word_original, int opcode, char *line, int index_line, file_name_values file_name, int *IC, word_instruction instruction[]) {
    char line_easy_to_handle[MAX_LINE_LENGTH + 3]; /* the parameter line with space before the comma and space after it, example: line="r2, r3" -> line_easy_to_handle="r2 ,  r3" */
    char *line_easy_to_handle_ptr; /* moving pointer to the var line_easy_to_handle */
    char content[MAX_LINE_LENGTH]; /* store the operand content */
    boolean there_is_comma = FALSE; /* if there is a comma in the parameter line */
    int content_len; /* the len of the var content */
    int i = 0;

    memset(line_easy_to_handle, '\0', MAX_LINE_LENGTH + 3);
    memset(content, '\0', MAX_LINE_LENGTH);

    if (line[0] == '\n') {
        printf("Error in line %d at \"%s\", \"%s\" must get 2 operands, actually got 0!\n", index_line,file_name.name, get_opcode_names()[opcode]);
        ERROR_OCCURRED_FIRST_PASS
    }
    if (line[0] == ',') {
        printf("Error in line %d at \"%s\", extra comma!\n", index_line,file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    /* fill the var line_easy_to_handle with the parameter line and add space before and after the comma in it */
    while (i < strlen(line)) {
        if (line[i] == ',') {
            if (there_is_comma) {
                printf("Error in line %d at \"%s\", extra comma!\n", index_line, file_name.name);
                ERROR_OCCURRED_FIRST_PASS
            }
            there_is_comma = TRUE;
            line_easy_to_handle[i] = ' '; line_easy_to_handle[i+1] = ','; line_easy_to_handle[i+2] = ' ';
            i++;
            if (line[i] == ',') {
                printf("Error in line %d at \"%s\", extra comma!\n", index_line, file_name.name);
                ERROR_OCCURRED_FIRST_PASS
            }
        }
        line_easy_to_handle[there_is_comma? i+2 : i] = line[i];
        i++;
    }
    if (!there_is_comma) {
        printf("Error in line %d at \"%s\", missing comma!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }

    line_easy_to_handle_ptr = line_easy_to_handle; /* make line_easy_to_handle_ptr to point the array line_easy_to_handle */

    line_easy_to_handle_ptr = get_until_white_characters(line_easy_to_handle_ptr, content); /* get the first operand to content */
    content_len = (int)strlen(content);
    content[content_len] = '\n'; content[content_len+1] = '\0'; /* add '\n' to the end (to avoid errors in the function "single operand") */

    /* handle the first operand */
    single_operand(opcode_word_original, opcode, content, index_line, file_name, IC, instruction, SOURCE_OPERAND_SHIFT);
    if (is_error_occurred_first_pass(FALSE, FALSE)) {
        return;
    }

    line_easy_to_handle_ptr = ignore_white_characters(line_easy_to_handle_ptr); /* move the pointer to the comma */
    line_easy_to_handle_ptr++; /* skip the comma */
    line_easy_to_handle_ptr = ignore_white_characters(line_easy_to_handle_ptr); /* move the pointer to the second operand */
    if (line_easy_to_handle_ptr[0] == '\n') {
        printf("Error in line %d at \"%s\", \"%s\" must get 2 operands, actually got 1!\n", index_line,file_name.name, get_opcode_names()[opcode]);
        ERROR_OCCURRED_FIRST_PASS
    }

    memset(content, '\0', MAX_LINE_LENGTH);
    line_easy_to_handle_ptr = get_until_white_characters(line_easy_to_handle_ptr, content); /* fill content with the second operand */
    content_len = (int)strlen(content);
    content[content_len] = '\n'; content[content_len+1] = '\0'; /* add '\n' to the end (to avoid errors in the function "single operand") */

    /* handle the second operand */
    single_operand(opcode_word_original, opcode, content, index_line, file_name, IC, instruction, TARGET_OPERAND_SHIFT);
    if (is_error_occurred_first_pass(FALSE, FALSE)) {
        return;
    }

    /* check if there is extraneous text */
    line_easy_to_handle_ptr = ignore_white_characters(line_easy_to_handle_ptr);
    if (line_easy_to_handle_ptr[0] != '\n') {
        printf("Error in line %d at \"%s\", extraneous text!\n", index_line, file_name.name);
        ERROR_OCCURRED_FIRST_PASS
    }
}

boolean is_error_occurred_first_pass(boolean is_error, boolean reset) {
    static boolean is_error_occurred_first_pass = FALSE; /* store the status of the compiling process of the first pass */

    if (reset) /* if reset required */
        is_error_occurred_first_pass = FALSE; /* reset to the initial value */

    if (is_error) /* if the function called because error */
        is_error_occurred_first_pass = TRUE; /* change the status to that there is an error */

    return is_error_occurred_first_pass; /* return the status of the compiling process of the first pass */
}
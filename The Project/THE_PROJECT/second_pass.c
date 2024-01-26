#include "headers/second_pass.h"

/* this constant is created specific to the file "second_pass.c", and called if error detected  */
#define ERROR_OCCURRED_SECOND_PASS  {           \
    is_error_occurred_second_pass(TRUE, FALSE); \
    return;                                     \
}

boolean second_pass(FILE *after_pre_processor, file_name_values file_name, label *head, int *IC, int *DC, word_instruction instruction[], word_data data[]) {
    char line[MAX_LINE_LENGTH]; /* string to store the line chars from the file */
    int index_line = 1; /* the current index of line in the file */

    update_data_value(head, IC); /* do level 17 in the first pass algorithm that describe at the maman */

    fseek(after_pre_processor, 0, SEEK_SET); /* move the pointer to the ".am" file to the start */
    memset(line, '\0', sizeof(line)); /* reset the var line array values to '\0' */
    while (fgets(line, MAX_LINE_LENGTH, after_pre_processor) != NULL) { /* go across the file */
        line_analysis_second_pass(head, line, index_line, file_name); /* analyze specific line in the file */
        memset(line, '\0', sizeof(line)); /* reset the var line array values to '\0' */
        index_line++; /* increase the line index by 1 */
    }

    complete_machine_code_from_first_pass(head, IC, instruction, file_name); /* complete the labels addresses in the machine code */
    
    if (is_error_occurred_second_pass(FALSE, FALSE)) /* if error occurred */ 
        return TRUE;

    create_object_file(file_name, IC, DC, instruction, data); /* create the ".ob" file */
    create_entries_file(&file_name, head); /* create the ".ent" */
    create_externals_file(file_name, IC, instruction); /* create the ".ext" file */

    /* if the program "survived" to this point, there is no chance that error occurred */
    return FALSE;
}

void update_data_value(label *head, int *IC) {
    label *curr = head;

    while (curr->next != NULL) { /* go over the linked list */
        if (curr->isData) /* if the label is with sign of data */
            curr->value += (*IC); /* update the value of the label by the IC value */
        curr = curr->next;
    }
}

void line_analysis_second_pass(label *head, char *line, int index_line, file_name_values file_name) {
    char content[MAX_LINE_LENGTH]; /* the content along the line */

    line = ignore_white_characters(line);
    if (line[0] == ';' || line[0] == '\n') { /* if a comment line or empty line */
        return;
    }

    memset(content, '\0', MAX_LINE_LENGTH);
    line = get_until_white_characters(line, content); /* fill str with the non-blank characters at the line */
    if (content[strlen(content)-1] == ':') { /* check if there is label definition */
        line = ignore_white_characters(line);
        memset(content, '\0', MAX_LINE_LENGTH);
        line = get_until_white_characters(line, content); /* skip the label content, and update the var str to the next content */
    }

    if (!strcmp(content, ".entry")) { /* if there is ".entry" definition */
        handle_entry_definition(head, line, index_line, file_name);
    }
}

void handle_entry_definition(label *head, char *line, int index_line, file_name_values file_name) {
    char label_name[MAX_LINE_LENGTH]; /* the label name received from the argument to ".entry" */
    label *argument_label; /* the node that represents the label from the var label_name */

    line = ignore_white_characters(line); /* if there are no arguments */
    if (line[0] == '\n') {
        printf("Error in line %d at \"%s\", \".entry\" must get 1 argument, actually got 0!\n", index_line, file_name.name);
        ERROR_OCCURRED_SECOND_PASS
    }

    memset(label_name, '\0', MAX_LINE_LENGTH);
    line = get_until_white_characters(line, label_name); /* fill the var label_name with the label name from ".entry" */
    if (!is_valid_label_macro_name(label_name)) { /* if the label name is invalid */
        printf("Error in line %d at \"%s\", the label \"%s\" has invalid name!\n", index_line, file_name.name, label_name);
        ERROR_OCCURRED_SECOND_PASS
    }

    line = ignore_white_characters(line);
    if (line[0] != '\n') {
        printf("Error in line %d at \"%s\", extra text after label name (the argument)!\n", index_line, file_name.name);
        ERROR_OCCURRED_SECOND_PASS
    }

    argument_label = search_label(head, label_name); /* find the label that represents the label from the var label_name */
    if (argument_label == NULL) { /* if the label didnt implemented in the code */
        printf("Error in line %d at \"%s\", the label \"%s\" is defined as entry, but not implemented in the code!\n", index_line, file_name.name, label_name);
        ERROR_OCCURRED_SECOND_PASS
    }

    argument_label->isEntry = TRUE; /* turn on the sign of entry */
}

label *search_label(label *head, char *label_name) {
    label *curr = head;

    while (curr->next != NULL && strcmp(label_name, curr->name) != 0) /* while the linked list didnt end and didnt found a node that related to the parameter label_name */
        curr = curr->next;

    return curr->next == NULL? NULL: curr; /* if reach the end, return NULL. else, return the node that related to the parameter label_name */
}

void complete_machine_code_from_first_pass(label *head, int *IC, word_instruction instruction[], file_name_values file_name) {
    int i;
    label *temp;

    for (i = 0; i < (*IC); i++) {
        if (strlen(instruction[i].label_name) != 0) { /* if it is need to write the label address in the machine code */
            temp = search_label(head, instruction[i].label_name); /* find the node in the linked list that represents the label address that the machine code need */
            if (temp == NULL) { /* if didnt found */
                printf("Error in line %d at \"%s\", the label \"%s\" doesnt exist!\n", instruction[i].index_line, file_name.name, instruction[i].label_name);
                ERROR_OCCURRED_SECOND_PASS
            }

            if (temp->isExtern) { /* if the label is extern */
                instruction[i].val = E;
            } else {
                instruction[i].val = temp->value << 2; /* write the label address at the machine code */
                instruction[i].val |= R;
            }
        }
    }
}

void create_object_file(file_name_values file_name, int *IC, int *DC, word_instruction instruction[], word_data data[]) {
    FILE *object_file; /* pointer to the ".ob" file */
    char in_special_base_32[MAX_BASE_32_LENGTH]; /* represents a number in the special base32 representation */
    int i;

    /* change the extension to ".ob" */
    file_name.name[file_name.len-2] = 'o'; file_name.name[file_name.len-1] = 'b';
    object_file = fopen(file_name.name, "w"); /* open the ".ob" file in write mode */

    if (object_file == NULL) { /* if the file didn't create */
        printf("Error! Couldn't create necessary file! abort the program.\n");
        exit(EXIT_FAILURE);
    }

    /* write IC and DC value to the ".ob" file */
    fprintf(object_file, "%s ", convert_to_special_base_32((*IC), in_special_base_32));
    fprintf(object_file, "%s\n", convert_to_special_base_32((*DC), in_special_base_32));

    /* write the instructions to the file in the special base 32 */
    for (i = 0; i < (*IC); i++) {
        fprintf(object_file, "%s ", convert_to_special_base_32(i + LOAD_LOCATION, in_special_base_32)); /* write the address */
        fprintf(object_file, "%s\n", convert_to_special_base_32(instruction[i].val, in_special_base_32)); /* write the value */
    }

    /* write the data to the file in the special base 32 */
    for (; i < (*DC) + (*IC); i++) {
        fprintf(object_file, "%s ", convert_to_special_base_32(i + LOAD_LOCATION, in_special_base_32)); /* write the address */
        fprintf(object_file, "%s\n", convert_to_special_base_32(data[i-(*IC)].val, in_special_base_32)); /* write the value */
    }

    fclose(object_file);
}

void create_entries_file(file_name_values *file_name, label *head) {
    char *file_name_ent = (char *)malloc((file_name->len + 1) * sizeof(char)); /* store the file name with the extension ".ent" */
    label *curr = head; /* moving pointer in the linked list */
    FILE *ent_file; /* pointer to the ".ent" file */
    boolean found_at_least_one_ent = FALSE; /* if there is at least one entry label */
    char in_special_base_32[MAX_BASE_32_LENGTH]; /* represents a number in the special base32 representation */

    IS_ALLOCATE_MEMORY_OK(file_name_ent)

    memset(file_name_ent, '\0', file_name->len+1);
    strncpy(file_name_ent, file_name->name, file_name->len - 2); /* fill the var file_name_ent with the file_name without its extension (".am") */
    file_name_ent[file_name->len-2] = 'e'; file_name_ent[file_name->len-1] = 'n'; file_name_ent[file_name->len] = 't'; /* add the ".ent" extension */
    file_name_ent[file_name->len+1] = '\0';

    while (curr->next != NULL) { /* go over the linked list */
        if (curr->isEntry) { /* if the label is with entry flag on */
            if (!found_at_least_one_ent) { /* if didnt found at least one entry label before */
                ent_file = fopen(file_name_ent, "w"); /* create the file ".ent" with write mode */
                if (ent_file == NULL) { /* if the file didn't create */
                    printf("Error! Couldn't create necessary file! abort the program.\n");
                    exit(EXIT_FAILURE);
                }
            }
            found_at_least_one_ent = TRUE; /* turn on the flag */
            fprintf(ent_file, "%s %s\n", curr->name, convert_to_special_base_32(curr->value, in_special_base_32)); /* write the name of the label and its value */
        }
        curr = curr->next;
    }

    /* chang the extension of the parameter file name to ".ent" extension */
    file_name->len++;
    strcpy(file_name->name, file_name_ent);

    free(file_name_ent);
    if (found_at_least_one_ent) fclose(ent_file);
}

void create_externals_file(file_name_values file_name, int *IC, word_instruction instruction[]) {
    FILE *ext_file; /* pointer to the ".ext" file */
    boolean found_at_least_one_ext = FALSE; /* if there is at least one extern label */
    int i;
    char in_special_base_32[MAX_BASE_32_LENGTH]; /* represents a number in the special base32 representation */

    file_name.name[file_name.len-2] = 'x'; /* change the extension to ".ext" */

    for (i = 0; i < (*IC); i++) { /* find all the shows of externals labels at the machine code */
        if ((instruction[i].val & E) == E) { /* if the label is extern label */
            if (!found_at_least_one_ext) { /* if didnt found at least one extern label before */
                ext_file = fopen(file_name.name, "w"); /* create the file ".ext" with write mode */
                if (ext_file == NULL) { /* if the file didn't create */
                    printf("Error! Couldn't create necessary file! abort the program.\n");
                    exit(EXIT_FAILURE);
                }
            }
            found_at_least_one_ext = TRUE; /* turn on the flag */
            fprintf(ext_file, "%s %s\n", instruction[i].label_name, convert_to_special_base_32(i + LOAD_LOCATION, in_special_base_32)); /* write the name of the label and its value */
        }
    }


    if (found_at_least_one_ext) fclose(ext_file);
}

char *convert_to_special_base_32(unsigned int x, char x_in_special_base_32[]) {
    static char special_base_32_chars[] = "!@#$%^&*<>abcdefghijklmnopqrstuv"; /* the chars of the special base 32 by order */
    int i;

    memset(x_in_special_base_32, '\0', MAX_BASE_32_LENGTH); /* reset the parameter in_special_base_32 */

    if (x == 0) /* if zero got */
        x_in_special_base_32[0] = special_base_32_chars[0];

    /* fill the parameter in_special_base_32 with the special base 32 representation of x */
    i = 0;
    while (x != 0) {
        x_in_special_base_32[i] = special_base_32_chars[x%32];
        x /= 32;
        i++;
    }

    REVERSE_STRING(x_in_special_base_32) /* reverse the string to get a correct result */
    return x_in_special_base_32;
}

boolean is_error_occurred_second_pass(boolean is_error, boolean reset) {
    static boolean is_error_occurred_second_pass = FALSE; /* store the status of the compiling process of the first pass */

    if (reset) /* if reset required */
        is_error_occurred_second_pass = FALSE; /* reset to the initial value */

    if (is_error) /* if the function called because error */
        is_error_occurred_second_pass = TRUE; /* change the status to that there is an error */

    return is_error_occurred_second_pass; /* return the status of the compiling process of the first pass */
}

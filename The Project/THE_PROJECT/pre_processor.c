#include "headers/pre_processor.h"

boolean pre_processor(FILE *fptr, file_name_values file_name) {
    macro *head; /* create the linked list of macros */
    char line[MAX_LINE_LENGTH]; /* string to store the line chars from the file */
    int index_line = 1; /* the current index of line in the file */
    FILE *to_write; /* the ".am" file */

    head = (macro *)malloc(sizeof(macro)); /* allocate memory to the head */
    IS_ALLOCATE_MEMORY_OK(head) /* check if malloc succeeded */
    memset(head->content, '\0', MAX_LINE_LENGTH * MAX_LINES_MACRO); /* reset the content field of head */

    file_name.name[file_name.len-1] = 'm'; /* change the file name extension to ".am" from ".as" */
    to_write = fopen(file_name.name, "w"); /* create the ".am" file */
    if (to_write == NULL) { /* if the file didn't create */
        printf("Error! Couldn't create necessary file! abort the program.\n");
        exit(EXIT_FAILURE);
    }

    memset(line, '\0', sizeof(line)); /* reset the var line array values to '\0' */
    while (read_line_from_file(fptr, line, index_line, file_name) != NULL) { /* go across the file */
        line_analysis_macros(head, line, index_line, to_write, file_name); /* analyze specific line in the file */
        memset(line, '\0', sizeof(line)); /* reset the var line array values to '\0' */
        index_line++; /* increase the line index by 1 */
    }

    fprintf(to_write, "\n"); /* add new line to the end of the ".am" file, the reason is that the assembler algorithms rely on that each line ending with '\n' */

    free_allocated_memory_pre_processor(fptr, to_write, head); /* free allocate memory */

    /* return if error occurred at the ".as" file */
    return is_error_occurred_macro(FALSE, FALSE);
}

char *read_line_from_file(FILE *fptr, char *line, int index_line, file_name_values file_name) {
    char *temp = fgets(line, MAX_LINE_LENGTH, fptr);

    if (temp != NULL) { /* if the file didn't end */
        if (strlen(line) == MAX_LINE_LENGTH - 1 && line[MAX_LINE_LENGTH-1] != '\n') { /* if the line at the is over than 81 chars including '\n' */
            file_name.name[file_name.len-1] = 's'; /* change the extension to ".as" */
            printf("Error in line %d at \"%s\", line too long (over 80 characters)!\n", index_line, file_name.name);
            file_name.name[file_name.len-1] = 'm'; /* change the extension to ".am" */
            is_error_occurred_macro(TRUE, FALSE);
        }
    }

    return temp;
}

void line_analysis_macros(macro *head, char *line, int index_line, FILE *to_write, file_name_values file_name) {
    static boolean inside_macro = FALSE; /* if the line is inside macro definition */
    char content[MAX_LINE_LENGTH]; /* the content after blank characters at the line */
    macro *tmp; /* if macro name detected at the file, this pointer point to the macro that detected at the linked list */
    char line_copy[MAX_LINE_LENGTH]; /* a copy of the line */

    strcpy(line_copy, line);
    memset(content, '\0', MAX_LINE_LENGTH); /* reset the string to '\0' */

    /* check if the flag inside_macro is on */
    if (inside_macro) {
        if (is_end_of_macro(line)) /* if "endmacro" detected in the line */
            inside_macro = FALSE; /* turn off the flag inside_macro */
        else /* if still at the macro definition */
            strcat((find_last_macro(head))->content, line_copy); /* add the line to the content of the macro */
    } else {
        line = ignore_white_characters(line);
        line = get_until_white_characters(line, content); /* fill str with the non-blank characters at the line */
        if ((tmp = search_macro(head, content)) != NULL) { /* if macro name detected */
            fprintf(to_write, "%s", tmp->content); /* write the macro content to the ".am" file */
        } else if (!strcmp(MACRO_DEFINITION, content)) { /* if macro definition detected */
            inside_macro = TRUE; /* turn on the inside_macro flag */
            add_macro_to_list(head, line, index_line, file_name); /* add the new macro to the linked list of macros and change */
        } else { /* if the line doesn't require any special treatment */
            fprintf(to_write, "%s", line_copy); /* copy the line to the ".am" file */
        }
    }
}

macro *search_macro(macro *head, char *macro_name) {
    macro *curr = head; /* pointer that go across the linked list of macros */

    if (strlen(macro_name) == 0) /* if the name is "" */
        return NULL;

    while (curr != NULL) { /* until curr didn't reach the end of the linked list of macros */
        if (!strcmp(curr->name, macro_name)) /* if a macro that in the list has an equal name as the parameter macro_name */
            return curr;
        curr = curr->next; /* point to the next node at the linked list if macros */
    }

    /* return NULL if all the macros that in the linked list hasn't an equal name as the parameter macro_name */
    return NULL;
}

void add_macro_to_list(macro *head, char *line, int index_line, file_name_values file_name) {
    char macro_name[MAX_LABEL_MACRO_LENGTH]; /* the name of the macro in the definition */
    macro *curr = head; /* pointer that go across the linked list of macros */

    memset(macro_name, '\0', MAX_LABEL_MACRO_LENGTH);

    line = ignore_white_characters(line);
    get_until_white_characters(line, macro_name); /* fill the var macro_name with the name in the definition */

    if (!is_valid_label_macro_name(macro_name)) { /* check if the macro name has invalid name */
        file_name.name[file_name.len-1] = 's'; /* change the extension to ".as" */
        printf("Error in line %d at \"%s\", macro \"%s\" has an invalid name!\n", index_line, file_name.name, macro_name);
        file_name.name[file_name.len-1] = 'm'; /* change the extension to ".am" */
        is_error_occurred_macro(TRUE, FALSE); /* turn on the flag of error */
    }

    if (search_macro(head, macro_name) != NULL) { /* check if the macro already defined */
        file_name.name[file_name.len-1] = 's'; /* change the extension to ".as" */
        printf("Error in line %d at \"%s\", macro \"%s\" already defined!\n", index_line, file_name.name, macro_name);
        file_name.name[file_name.len-1] = 'm'; /* change the extension to ".am" */
        is_error_occurred_macro(TRUE, FALSE); /* turn on the flag of error */
    }

    if (register_index(macro_name) != NOT_REGISTER) { /* check if macro is register name */
        file_name.name[file_name.len-1] = 's'; /* change the extension to ".as" */
        printf("Error in line %d at \"%s\", macro \"%s\" cant be a register name!\n", index_line, file_name.name, macro_name);
        file_name.name[file_name.len-1] = 'm'; /* change the extension to ".am" */
        is_error_occurred_macro(TRUE, FALSE); /* turn on the flag of error */
    }

    while (curr->next != NULL) curr = curr->next; /* find the macro in the list that will store the macro values */
    strcpy(curr->name, macro_name); /* store the name of the macro at the macro node */

    /* create a new node for the next macro */
    curr->next = (macro *)malloc(sizeof(macro));
    IS_ALLOCATE_MEMORY_OK(curr->next) /* check if malloc succeeded */
    memset((curr->next)->content, '\0', MAX_LINE_LENGTH * MAX_LINES_MACRO); /* reset the content field of the node */
    (curr->next)->next = NULL;
}

macro *find_last_macro(macro *head) {
    macro *curr = head;

    while ((curr->next)->next != NULL)
        curr = curr->next;

    return curr;
}

boolean is_end_of_macro(char *line) {
    char content[MAX_LINE_LENGTH]; /* will fill with the non-blank characters from the parameter line */

    memset(content, '\0', MAX_LINE_LENGTH);
    line = ignore_white_characters(line); /* delete the blank characters from the start of the line */
    get_until_white_characters(line, content); /*fill with the non-blank characters from the parameter line */

    return !strcmp(content, END_MACRO_DEFINITION); /* return if the line ending the macro definition, in other words: var content equals to "endmacro" */
}

boolean is_error_occurred_macro(boolean is_error, boolean reset) {
    static boolean is_error_occurred_macro = FALSE; /* store the status of the compiling process of the pre-processor */

    if (reset) /* if reset required */
        is_error_occurred_macro = FALSE; /* reset to the initial value */

    if (is_error) /* if the function called because error */
        is_error_occurred_macro = TRUE; /* change the status to that there is an error */

    return is_error_occurred_macro; /* return the status of the compiling process of the pre-processor */
}

void free_allocated_memory_pre_processor(FILE *fptr, FILE *to_write, macro *head) {
    macro *curr;

    /* free the nodes of the linked list of macros */
    while (head != NULL) {
        curr = head;
        head = head->next;
        free(curr);
    }

    /* free the pointers to the ".as" and the ".am" files */
    fclose(fptr);
    fclose(to_write);
}

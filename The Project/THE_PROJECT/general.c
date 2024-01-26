#include "headers/general.h"

char *ignore_white_characters(char *line) {
    int i = 0;
    while (line[i] != '\n' && (line[i] == ' ' || line[i] == '\t')) i++; /* count until non-blank character */
    return line + i; /* delete the blank characters from the start of the line until non-blank character */
}

char *get_until_white_characters(char *line,  char *content) {
    int i = 0;

    /* go over line from start until blank character */
    while (line[i] != '\n' && line[i] != ' ' && line[i] != '\t') {
        content[i] = line[i]; /* fill the parameter content with non-blank characters from line */
        i++;
    }

    return line + i; /* delete the non-blank characters from the start of the line until blank character */
}

boolean is_valid_label_macro_name(char *name) {
    int i;
    boolean valid = TRUE; /* store if the parameter name is valid according the maman */

    /* if the first character at the parameter name is not between 'a'-'z' or not between 'A'-'Z' */
    if (!((name[0] >= 'a' && name[0] <= 'z')
          || (name[0] >= 'A' && name[0] <= 'Z')))
        valid = FALSE;

    /* go over the parameter name by characters */
    for (i = 1; i < strlen(name); i++)
        /* if character at the parameter name is not between 'a'-'z' or not between 'A'-'Z' or not between '0'-'9' */
        if (!((name[i] >= 'a' && name[i] <= 'z')
              || (name[i] >= 'A' && name[i] <= 'Z')
              || (name[i] >= '0' && name[i] <= '9')))
            valid = FALSE;

    /* if the length of the parameter name is over 30 characters */
    if (strlen(name) > MAX_LABEL_MACRO_LENGTH)
        valid = FALSE;

    if (register_index(name) != NOT_REGISTER) /* if the name is register name */
        valid = FALSE;

    for (i = 0; i < 8; i++)
        if (!strcmp(get_opcode_names()[i], name)) /* if the name is opcode name */
            i = 8;

    return valid;
}

char **get_opcode_names() {
    /* the opcode names */
    static char *opcode_names[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};
    return opcode_names; /* return the string array */
}

int register_index(char *str) {
    static char *registers_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    int i;

    for (i = 0; i < 8; i++)
        if (!strcmp(registers_names[i], str)) /* if the parameter str is a name of register */
            break;

    return i; /* return the register index */
}

#include "headers/assembler.h"

int main(int argc, char *argv[]) {
    extract_files_from_command_line(argc, argv); /* calling a function that handle the program arguments */
    return EXIT_SUCCESS;
}

void extract_files_from_command_line(int argc, char *argv[]) {
    int i;
    FILE *fptr; /* pointer to the ".as" file */
    file_name_values file_name; /* the name of the file */

    /* if no arguments received to the program, exit */
    if (argc <= 1) {
        printf("No arguments received!\n");
        exit(EXIT_FAILURE);
    }

    /* loop over the file names that received to the program */
    for (i = 1; i < argc; i++) {
        file_name.len = (int)strlen(argv[i]) + 3;
        file_name.name = (char *)malloc((file_name.len + 1) * sizeof(char));

        IS_ALLOCATE_MEMORY_OK(file_name.name) /* check if malloc succeeded */
        memset(file_name.name, '\0', file_name.len + 1); /* reset values to '\0' */
        strcpy(file_name.name, argv[i]); /* fill the file name at the attribute text at the struct file_name */
        file_name.name[file_name.len-3] = '.'; file_name.name[file_name.len-2] = 'a'; file_name.name[file_name.len-1] = 's'; /* add the extension ".as" to the file name */

        /* open the file ".as" */
        fptr = fopen(file_name.name, "r");
        if (fptr == NULL) {
            printf("The file \"%s\" doesnt exist!\n\n", file_name.name);
            free(file_name.name);
            continue;
        }

        /* compiling the file */
        compile(fptr, &file_name);

        RESET_ERROR_FUNCTIONS
    }
}

void compile(FILE *fptr, file_name_values *file_name) {
    word_data data[BUFF]; /* array that store the encode of the data guidance sentences at the code */
    word_instruction instruction[BUFF]; /* array that store the encode of the instruction sentences at the code */
    int *IC; /* IC - instruction counter */
    int *DC; /* DC - data counter */
    label *head; /* the head of the linked list of labels */
    FILE *after_pre_processor; /* pointer to the ".am" file in read mode */

    printf("Start compiling the file \"%s\":\n", file_name->name);

    /* check if the file didn't succeed the pre-processor */
    if (pre_processor(fptr, *file_name)) {
        file_name->name[file_name->len-1] = 's'; /* change the extension to ".as" */
        printf("The file is including error/s, do not creating .am file!\n");
        printf("Abort the compiling of the file.\n\n");
        file_name->name[file_name->len-1] = 'm'; /* change the extension to ".am" */
        remove(file_name->name); /* delete the ".am" file */
        free(file_name->name);
        return;
    }

    after_pre_processor = fopen(file_name->name, "r"); /* the ".am" file in read mode */

    /* create the linked list of labels */
    head = (label *)malloc(sizeof(label));
    IS_ALLOCATE_MEMORY_OK(head)
    head->name[0] = '\0';
    head->value = 0;
    head->isData = head->isExtern = head->isEntry = head->isCode = FALSE;
    head->next = NULL;

    /* create the IC and DC and initialize them to 0 */
    IC = (int *)malloc(sizeof(int));
    IS_ALLOCATE_MEMORY_OK(IC)
    DC = (int *)malloc(sizeof(int));
    IS_ALLOCATE_MEMORY_OK(DC)
    (*IC) = (*DC) = 0;

    /* run the first pass, and check if error occurred */
    if (first_pass(after_pre_processor, *file_name, IC, DC, instruction, data, head)) {
        printf("The file is including error/s, do not creating .ob, .ent, .ext!\n");
        printf("Abort the compiling of the file.\n\n");
        free_allocated_memory(file_name, IC, DC, head, after_pre_processor); /* free allocated memory at the program */
        return;
    }

    /* run the second pass, and check if error occurred */
    if (second_pass(after_pre_processor, *file_name, head, IC, DC, instruction, data)) {
        printf("The file is including error/s, do not creating .ob, .ent, .ext!\n");
        printf("Abort the compiling of the file.\n\n");
        free_allocated_memory(file_name, IC, DC, head, after_pre_processor); /* free allocated memory at the program */
        return;
    }

    free_allocated_memory(file_name, IC, DC, head, after_pre_processor); /* free allocated memory at the program */
    printf("The file compiled successfully!\n\n");
}

void free_allocated_memory(file_name_values *file_name, int *IC, int *DC, label *head, FILE *after_pre_processor) {
    label *curr;
    while (head != NULL) { /* free all the linked list nodes */
        curr = head;
        head = head->next;
        free(curr);
    }

    fclose(after_pre_processor);
    free(file_name->name);
    free(IC);
    free(DC);
}

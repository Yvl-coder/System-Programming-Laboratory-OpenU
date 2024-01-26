#include "mat.c"

void fillZeros(mat a, mat b, mat c, mat d, mat e, mat f);
int equals(char s1[], char s2[], int len1, int len2);

typedef struct {
    int len;
    char *line;
} string;


int main() {
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;
    mat *mats[6]; /* represents array of pointers to the mats above */
    char matsNames[6][5] = {"MAT_A", "MAT_B", "MAT_C", "MAT_D", "MAT_E", "MAT_F"}; /* name of the mats in string */

    int error; /* represents if the command has an error (made because inner while's at the code, not always in use) */
    string cmdLine; /* represents the command line */
    char c = '\0'; /* represents every char in the command line */

    int curr = 0; /* represents index that moving across the command line */
    string cmdName; /* represents the name of the command that need to run */
    char matName[5]; /* represents the mat/s name in the command line */
    int matIndex; /* represents the index of the mat in the array mats */

    double values[16]; /* represents the numbers that the user entered at "read_mat"*/
    string number; /* represents the number the user entered in the command line*/
    char *end; /* created for the function "strtod"*/
    double num; /* represents the string number as a double type of it */

    int indexMats[3]; /* represents array of indexes of mats in the command line by they order in it */

    int i, j; /* indexes */

    fillZeros(MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F); /* puts 0's at all the mats */

    /* set the mats array */
    mats[0] = &MAT_A;
    mats[1] = &MAT_B;
    mats[2] = &MAT_C;
    mats[3] = &MAT_D;
    mats[4] = &MAT_E;
    mats[5] = &MAT_F;


    while (TRUE) {
        error = FALSE;
        cmdLine.len = 30;
        cmdLine.line = (char *)malloc(30 * sizeof(char));
        if (cmdLine.line == NULL) {
            printf("Error 1 - Please restart the program.\n");
            return -1;
        }


        if (c == EOF) {
            printf("\nError 11 - Program stopped by EOF and not by the command \"stop\".\n");
            break;
        }

        printf("\nPlease enter the command >>> ");
        c = (char)getchar(); /* represents chars in the command line that the user enter */
        i = 0;
        /* fill the var (cmdLine.line) that represents the command line with the command line the user entered */
        while (c != '\n' && c != EOF) {
            if (i >= cmdLine.len) {
                cmdLine.len += 15;
                cmdLine.line = (char *)realloc(cmdLine.line, cmdLine.len);
                if (cmdLine.line == NULL) {
                    printf("Error 1 - Please restart the program.\n");
                    return -1;
                }
            }
            cmdLine.line[i] = c;
            i++;
            c = (char)getchar();
        }

        printf("The command that entered is >>> %s\n", cmdLine.line);

        cmdLine.len = (int)strlen(cmdLine.line); /* change to the real len */
        curr = 0;

        /* ignore spaces and tabs in the command line */
        while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

        /* if the command line is just tabs and spaces, then skip */
        if (curr == cmdLine.len) continue;


        cmdName.len = 0;
        cmdName.line = (char *)malloc(10 * sizeof(char));
        if (cmdName.line == NULL) {
            printf("Error 1 - Please restart the program.\n");
            return -1;
        }

        /* fill the command name from what the user entered */
        while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && curr < cmdLine.len) {
            if (cmdName.len >= 10) {
                printf("Error 3 - Undefined command name.\n");
                error = TRUE;
            }
            cmdName.line[cmdName.len] = cmdLine.line[curr];
            if (cmdName.line[cmdName.len] == ',') {
                printf("Error 7 - Illegal comma.\n");
                error = TRUE;
            }
            curr++;
            cmdName.len++;
        }
        if (error) continue;


        /* checking which command the user want to execute */
        if (equals(cmdName.line, "read_mat", cmdName.len, 8)) {
            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - \"read_mat\" must have matrix argument and numbers.\n");
                continue;
            }
            if (cmdLine.line[curr] == ',') {
                printf("Error 7 - Illegal comma.\n");
                continue;
            }

            memset(matName, '\0', sizeof(matName));

            i = 0;
            while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' && curr < cmdLine.len) {
                if (i >= 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                matName[i] = cmdLine.line[curr];
                curr++;
                i++;
            }
            if (error) continue;

            /* find the matrix according to the user input */
            i = 0;
            while (TRUE) {
                if (equals(matName, matsNames[i], 5, 5))
                    break;
                if (i == 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                i++;
            }
            if (error) continue;
            matIndex = i;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) {
                printf("Error 6 - Missing argument.\n");
                continue;
            }
            if (cmdLine.line[curr] != ',') {
                printf("Error 8 - Missing comma.\n");
                continue;
            }
            curr++;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) {
                printf("Error 6 - Missing argument.\n");
                continue;
            }

            for (i = 0; i < 16; i++)
                values[i] = 0;

            i = 0;
            /* takes the numbers the user entered */
            while (curr < cmdLine.len) {
                if (i == 16) break;

                number.len = 30;
                number.line = (char *)malloc(30 * sizeof(char));
                if (number.line == NULL) {
                    printf("Error 1 - Please restart the program.\n");
                    return -1;
                }
                j = 0; /* index of the string number */
                while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' && curr < cmdLine.len) {
                    if (j >= number.len) {
                        number.len += 15;
                        number.line = (char *)realloc(number.line, number.len);
                        if (number.line == NULL) {
                            printf("Error 1 - Please restart the program.\n");
                            return -1;
                        }
                    }
                    number.line[j] = cmdLine.line[curr];
                    curr++;
                    j++;
                }

                if (strlen(number.line) == 0) {
                    printf("Error 9 - Multiple consecutive commas.\n");
                    error = TRUE;
                    break;
                }


                num = strtod(number.line, &end);
                if (strlen(end) == 0) {
                    values[i] = num;
                } else {
                    printf("Error 4 - Argument is not a real number.\n");
                    error = TRUE;
                    break;
                }

                if (curr == cmdLine.len) {
                    break;
                }

                /* ignore spaces and tabs in the command line */
                while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

                if (cmdLine.line[curr] == ',') {
                    curr++;
                } else {
                    if (curr == cmdLine.len) break;
                    if (cmdLine.line[curr] != ',') {
                        printf("Error 8 - Missing comma.\n");
                        error = TRUE;
                        break;
                    }
                }

                /* ignore spaces and tabs in the command line */
                while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
                if (curr == cmdLine.len) {
                    printf("Error 5 - Extraneous text after end of command.\n");
                    error = TRUE;
                    break;
                }
                i++;
            }
            if (error) continue;

            read_mat(*mats[matIndex], values);

            printf("Values entered successfully.\n");
        } else if (equals(cmdName.line, "print_mat", cmdName.len, 9)) {
            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - \"print_mat\" must have matrix argument.\n");
                continue;
            }
            if (cmdLine.line[curr] == ',') {
                printf("Error 7 - Illegal comma.\n");
                continue;
            }

            memset(matName, '\0', sizeof(matName));
            i = 0;
            while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' && curr < cmdLine.len) {
                if (i >= 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                matName[i] = cmdLine.line[curr];
                curr++;
                i++;
            }
            if (error) continue;

            /* find the matrix according to the user input */
            i = 0;
            while (TRUE) {
                if (equals(matName, matsNames[i], 5, 5))
                    break;
                if (i == 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                i++;
            }
            if (error) continue;
            matIndex = i;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

            /* if there were only spaces and tabs in the end of the command line */
            if (curr == cmdLine.len)
                print_mat(*mats[matIndex]);
            else {
                printf("Error 5 - Extraneous text after end of command.\n");
                continue;
            }
        } else if (equals(cmdName.line, "add_mat", cmdName.len, 7)) {
            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - \"add_mat\" must have matrix arguments.\n");
                continue;
            }
            if (cmdLine.line[curr] == ',') {
                printf("Error 7 - Illegal comma.\n");
                continue;
            }

            memset(indexMats, -1, sizeof(indexMats));
            j = 0; /* an index that go over the mats in the command line */
            while (j < 3) {
                memset(matName, '\0', sizeof(matName));
                i = 0;
                while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' &&
                       curr < cmdLine.len) {
                    if (i >= 5) {
                        printf("Error 2 - Undefined matrix name.\n");
                        error = TRUE;
                        break;
                    }
                    matName[i] = cmdLine.line[curr];
                    curr++;
                    i++;
                }
                if (error) break;

                if (i == 0) {
                    printf("Error 9 - Multiple consecutive commas.\n");
                    error = TRUE;
                    break;
                }

                /* find the matrix according to the user input */
                i = 0;
                while (TRUE) {
                    if (equals(matName, matsNames[i], 5, 5))
                        break;
                    if (i == 5) {
                        printf("Error 2 - Undefined matrix name.\n");
                        error = TRUE;
                        break;
                    }
                    i++;
                }
                if (error) break;

                indexMats[j] = i;

                if (j < 2) {
                    /* ignore spaces and tabs in the command line */
                    while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

                    if (curr == cmdLine.len) {
                        printf("Error 6 - Missing argument.\n");
                        error = TRUE;
                        break;
                    }

                    if (cmdLine.line[curr] == ',') {
                        curr++;
                    } else {
                        if (cmdLine.line[curr] != ',') {
                            printf("Error 8 - Missing comma.\n");
                            error = TRUE;
                            break;
                        }
                    }

                    /* ignore spaces and tabs in the command line */
                    while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

                    if (curr == cmdLine.len) {
                        printf("Error 6 - Missing argument.\n");
                        error = TRUE;
                        break;
                    }
                }
                j++;
            }
            if (error) continue;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

            if (curr == cmdLine.len) {
                add_mat(*mats[indexMats[0]], *mats[indexMats[1]], *mats[indexMats[2]]);
                printf("Command execute successfully.\n");
            } else {
                printf("Error 5 - Extraneous text after end of command.\n");
                continue;
            }
        } else if (equals(cmdName.line, "sub_mat", cmdName.len, 7)) {
            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - \"sub_mat\" must have matrix arguments.\n");
                continue;
            }
            if (cmdLine.line[curr] == ',') {
                printf("Error 7 - Illegal comma.\n");
                continue;
            }

            memset(indexMats, -1, sizeof(indexMats));
            j = 0;
            while (j < 3) {
                memset(matName, '\0', sizeof(matName));
                i = 0;
                while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' &&
                       curr < cmdLine.len) {
                    if (i >= 5) {
                        printf("Error 2 - Undefined matrix name.\n");
                        error = TRUE;
                        break;
                    }
                    matName[i] = cmdLine.line[curr];
                    curr++;
                    i++;
                }
                if (error) break;

                if (i == 0) {
                    printf("Error 9 - Multiple consecutive commas.\n");
                    error = TRUE;
                    break;
                }

                /* find the matrix according to the user input */
                i = 0;
                while (TRUE) {
                    if (equals(matName, matsNames[i], 5, 5))
                        break;
                    if (i == 5) {
                        printf("Error 2 - Undefined matrix name.\n");
                        error = TRUE;
                        break;
                    }
                    i++;
                }
                if (error) break;

                indexMats[j] = i;

                if (j < 2) {
                    /* ignore spaces and tabs in the command line */
                    while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

                    if (curr == cmdLine.len) {
                        printf("Error 6 - Missing argument.\n");
                        error = TRUE;
                        break;
                    }

                    if (cmdLine.line[curr] == ',') {
                        curr++;
                    } else {
                        if (cmdLine.line[curr] != ',') {
                            printf("Error 8 - Missing comma.\n");
                            error = TRUE;
                            break;
                        }
                    }

                    /* ignore spaces and tabs in the command line */
                    while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

                    if (curr == cmdLine.len) {
                        printf("Error 6 - Missing argument.\n");
                        error = TRUE;
                        break;
                    }
                }
                j++;
            }
            if (error) continue;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

            if (curr == cmdLine.len) {
                sub_mat(*mats[indexMats[0]], *mats[indexMats[1]], *mats[indexMats[2]]);
                printf("Command execute successfully.\n");
            } else {
                printf("Error 5 - Extraneous text after end of command.\n");
                continue;
            }
        } else if (equals(cmdName.line, "mul_mat", cmdName.len, 7)) {
            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - \"mul_mat\" must have matrix arguments.\n");
                continue;
            }
            if (cmdLine.line[curr] == ',') {
                printf("Error 7 - Illegal comma.\n");
                continue;
            }

            memset(indexMats, -1, sizeof(indexMats));
            j = 0;
            while (j < 3) {
                memset(matName, -1, sizeof(matName));
                i = 0;
                while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' &&
                       curr < cmdLine.len) {
                    if (i >= 5) {
                        printf("Error 2 - Undefined matrix name.\n");
                        error = TRUE;
                        break;
                    }
                    matName[i] = cmdLine.line[curr];
                    curr++;
                    i++;
                }
                if (error) break;

                if (i == 0) {
                    printf("Error 9 - Multiple consecutive commas.\n");
                    error = TRUE;
                    break;
                }

                /* find the matrix according to the user input */
                i = 0;
                while (TRUE) {
                    if (equals(matName, matsNames[i], 5, 5))
                        break;
                    if (i == 5) {
                        printf("Error 2 - Undefined matrix name.\n");
                        error = TRUE;
                        break;
                    }
                    i++;
                }
                if (error) break;

                indexMats[j] = i;

                if (j < 2) {
                    /* ignore spaces and tabs in the command line */
                    while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

                    if (curr == cmdLine.len) {
                        printf("Error 6 - Missing argument.\n");
                        error = TRUE;
                        break;
                    }

                    if (cmdLine.line[curr] == ',') {
                        curr++;
                    } else {
                        if (cmdLine.line[curr] != ',') {
                            printf("Error 8 - Missing comma.\n");
                            error = TRUE;
                            break;
                        }
                    }

                    /* ignore spaces and tabs in the command line */
                    while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

                    if (curr == cmdLine.len) {
                        printf("Error 6 - Missing argument.\n");
                        error = TRUE;
                        break;
                    }
                }
                j++;
            }
            if (error) continue;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

            if (curr == cmdLine.len) {
                mul_mat(*mats[indexMats[0]], *mats[indexMats[1]], *mats[indexMats[2]]);
                printf("Command execute successfully.\n");
            } else {
                printf("Error 5 - Extraneous text after end of command.\n");
                continue;
            }
        } else if (equals(cmdName.line, "mul_scalar", cmdName.len, 10)) {
            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - \"mul_scalar\" must have matrix arguments and scalar argument.\n");
                continue;
            }
            if (cmdLine.line[curr] == ',') {
                printf("Error 7 - Illegal comma.\n");
                continue;
            }

            memset(indexMats, -1, sizeof(indexMats));
            memset(matName, -1, sizeof(matName));
            i = 0;
            while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' &&
                   curr < cmdLine.len) {
                if (i >= 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                matName[i] = cmdLine.line[curr];
                curr++;
                i++;
            }
            if (error) continue;


            /* find the matrix according to the user input */
            i = 0;
            while (TRUE) {
                if (equals(matName, matsNames[i], 5, 5))
                    break;
                if (i == 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                i++;
            }
            if (error) continue;

            indexMats[0] = i;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - Missing argument.\n");
                continue;
            }
            if (cmdLine.line[curr] != ',') {
                printf("Error 8 - Missing comma.\n");
                continue;
            }
            curr++;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - Missing argument.\n");
                continue;
            }

            number.len = 30;
            number.line = (char *)malloc(30 * sizeof(char));
            if (number.line == NULL) {
                printf("Error 1 - Please restart the program.\n");
                return -1;
            }
            j = 0; /* index of the string number */
            while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' && curr < cmdLine.len) {
                if (j >= number.len) {
                    number.len += 15;
                    number.line = (char *)realloc(number.line, number.len);
                    if (number.line == NULL) {
                        printf("Error 1 - Please restart the program.\n");
                        return -1;
                    }
                }
                number.line[j] = cmdLine.line[curr];
                curr++;
                j++;
            }

            if (strlen(number.line) == 0) {
                printf("Error 9 - Multiple consecutive commas.\n");
                continue;
            }

            num = strtod(number.line, &end);
            if (strlen(end) != 0) {
                printf("Error 4 - Argument is not a scalar.\n");
                continue;
            }

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - Missing argument.\n");
                continue;
            }
            if (cmdLine.line[curr] != ',') {
                printf("Error 8 - Missing comma.\n");
                continue;
            }
            curr++;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - Missing argument.\n");
                continue;
            }

            memset(matName, '\0', sizeof(matName));
            i = 0;
            while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' &&
                   curr < cmdLine.len) {
                if (i >= 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                matName[i] = cmdLine.line[curr];
                curr++;
                i++;
            }
            if (error) continue;

            if (matName[0] == '\0') {
                printf("Error 9 - Multiple consecutive commas.\n");
                continue;
            }

            /* find the matrix according to the user input */
            i = 0;
            while (TRUE) {
                if (equals(matName, matsNames[i], 5, 5))
                    break;
                if (i == 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                i++;
            }
            if (error) continue;

            indexMats[1] = i;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

            if (curr == cmdLine.len) { /* if there is no argument */
                mul_scalar(*mats[indexMats[0]], num, *mats[indexMats[1]]);
                printf("Command execute successfully.\n");
            } else {
                printf("Error 5 - Extraneous text after end of command.\n");
                continue;
            }
        } else if (equals(cmdName.line, "trans_mat", cmdName.len, 9)) {
            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - \"trans_mat\" must have matrix arguments.\n");
                continue;
            }
            if (cmdLine.line[curr] == ',') {
                printf("Error 7 - Illegal comma.\n");
                continue;
            }

            memset(indexMats, -1, sizeof(indexMats));
            memset(matName, -1, sizeof(matName));
            i = 0;
            while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' &&
                   curr < cmdLine.len) {
                if (i >= 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                matName[i] = cmdLine.line[curr];
                curr++;
                i++;
            }
            if (error) continue;


            /* find the matrix according to the user input */
            i = 0;
            while (TRUE) {
                if (equals(matName, matsNames[i], 5, 5))
                    break;
                if (i == 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                i++;
            }
            if (error) continue;

            indexMats[0] = i;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - Missing argument.\n");
                continue;
            }
            if (cmdLine.line[curr] != ',') {
                printf("Error 8 - Missing comma.\n");
                continue;
            }
            curr++;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;
            if (curr == cmdLine.len) { /* if there is no argument */
                printf("Error 6 - Missing argument.\n");
                continue;
            }


            memset(matName, '\0', sizeof(matName));
            i = 0;
            while (cmdLine.line[curr] != ' ' && cmdLine.line[curr] != '\t' && cmdLine.line[curr] != ',' &&
                   curr < cmdLine.len) {
                if (i >= 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                matName[i] = cmdLine.line[curr];
                curr++;
                i++;
            }
            if (error) continue;

            if (matName[0] == '\0') {
                printf("Error 9 - Multiple consecutive commas.\n");
                continue;
            }

            /* find the matrix according to the user input */
            i = 0;
            while (TRUE) {
                if (equals(matName, matsNames[i], 5, 5))
                    break;
                if (i == 5) {
                    printf("Error 2 - Undefined matrix name.\n");
                    error = TRUE;
                    break;
                }
                i++;
            }
            if (error) continue;

            indexMats[1] = i;

            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

            if (curr == cmdLine.len) { /* if there is no argument */
                trans_mat(*mats[indexMats[0]], *mats[indexMats[1]]);
                printf("Command execute successfully.\n");
            } else {
                printf("Error 5 - Extraneous text after end of command.\n");
                continue;
            }
        } else if (equals(cmdName.line, "stop", cmdName.len, 4)){
            /* ignore spaces and tabs in the command line */
            while ((cmdLine.line[curr] == ' ' || cmdLine.line[curr] == '\t') && curr < cmdLine.len) curr++;

            /* if after 'stop' there are only tabs and spaces */
            if (curr == cmdLine.len) {
                printf("\n");
                break;
            }

            printf("Error 5 - Extraneous text after end of command.\n");
            continue;
        } else {
            printf("Error 3 - Undefined command name.\n");
            continue;
        }
    }

    if (cmdLine.line != NULL)
        free(cmdLine.line);
    if (cmdName.line != NULL)
        free(cmdName.line);
    if (number.line != NULL)
        free(number.line);

    return 0;
}

void fillZeros(mat a, mat b, mat c, mat d, mat e, mat f) {
    int i, j;
    for (i = 0; i < LEN_MAT; i++) {
        for (j = 0; j < LEN_MAT; j++) {
            a[i][j] = b[i][j] = c[i][j] = d[i][j] = e[i][j] = f[i][j] = 0;
        }
    }
}

int equals(char s1[], char s2[], int len1, int len2) {
    int res = TRUE;
    int i = 0;

    if (len1 != len2)
        return FALSE;

    while (i < len1) {
        if (s1[i] != s2[i]) {
            res = FALSE;
            break;
        }
        i++;
    }

    return res;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LEN_MAT 4
#define TRUE 1
#define FALSE 0

typedef double mat[LEN_MAT][LEN_MAT];

void read_mat(mat a, double values[]);
void print_mat(mat a);
void add_mat(mat a, mat b, mat c);
void sub_mat(mat a, mat b, mat c);
void mul_mat(mat a, mat b, mat c);
void mul_scalar(mat a, double b, mat c);
void trans_mat(mat a, mat b);
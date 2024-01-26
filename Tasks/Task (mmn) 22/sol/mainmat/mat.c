#include "mat.h"

void read_mat(mat a, double values[]) {
    int i, j;
    for (i = 0; i < LEN_MAT; i++) {
        for (j = 0; j < LEN_MAT; j++) {
            a[i][j] = values[LEN_MAT * i + j];
        }
    }
}

void print_mat(mat a) {
    int i, j;
    for (i = 0; i < LEN_MAT; i++) {
        for (j = 0; j < LEN_MAT; j++) {
            printf("%7.2f  ", a[i][j]);
        }
        printf("\n");
    }
}

void add_mat(mat a, mat b, mat c) {
    double results[16];

    int i, j;
    for (i = 0; i < LEN_MAT; i++) {
        for (j = 0; j < LEN_MAT; j++) {
            results[LEN_MAT*i + j] = a[i][j] + b[i][j];
        }
    }

    read_mat(c, results);
}

void sub_mat(mat a, mat b, mat c) {
    double results[16];

    int i, j;
    for (i = 0; i < LEN_MAT; i++) {
        for (j = 0; j < LEN_MAT; j++) {
            results[LEN_MAT*i + j] = a[i][j] - b[i][j];
        }
    }

    read_mat(c, results);
}

void mul_mat(mat a, mat b, mat c) {
    double results[16];

    int i, j;
    for (i = 0; i < LEN_MAT; i++) {
        for (j = 0; j < LEN_MAT; j++) {
            int result = 0, k;
            for (k = 0; k < LEN_MAT; k++)
                result += a[i][k] * b[k][j];
            results[LEN_MAT*i + j] = result;
        }
    }

    read_mat(c, results);
}

void mul_scalar(mat a, double b, mat c) {
    double results[16];

    int i, j;
    for (i = 0; i < LEN_MAT; i++) {
        for (j = 0; j < LEN_MAT; j++) {
            results[LEN_MAT*i + j] = a[i][j] * b;
        }
    }

    read_mat(c, results);
}

void trans_mat(mat a, mat b) {
    double results[16];

    int i, j;
    for (i = 0; i < LEN_MAT; i++) {
        for (j = 0; j < LEN_MAT; j++) {
            results[LEN_MAT*i + j] = a[j][i];
        }
    }

    read_mat(b, results);
}

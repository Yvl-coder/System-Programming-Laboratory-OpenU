/* this program calculate the scalar product of 2 vectors (in max length of 1000) by adding the multiplication of the 2 vectors in the same index each adding */

#include <stdio.h>
#define MAX_LENGTH 1000 /* the max length that the arrays in this program can be */

int scalar_product(int v[], int u[], int len); /* calculate the scalar product of 2 vectors */

int main() {
    int i;
    int len; /* represents the vectors length */
    int v[MAX_LENGTH]; /* represents a vector */
    int u[MAX_LENGTH]; /* represents a vector */

    printf("Enter the length of the vectors: ");
    scanf("%d", &len);
    if (len <= 0) { /* check if the length value is valid */
    	printf("\nInvalid length!\n");
        return 0;
    }
    
    /* fill v vector */
    printf("\nFirst vector:\n");
    for (i = 0; i < len; i++) {
        printf("Enter the value of the cell with the index %d: ", i);
        scanf("%d", &v[i]);
    }
    
    /* fill u vector */
    printf("\nSecond vector:\n");
    for (i = 0; i < len; i++) {
        printf("Enter the value of the cell with the index %d: ", i);
        scanf("%d", &u[i]);
    }
    
    printf("\nThe scalar product is: %d\n", scalar_product(v, u, len));
    return 0;
}

int scalar_product(int v[], int u[], int len) {
    int sum = 0;

    int i;
    for (i = 0; i < len; i++) /* run over the vectors and calculate the scalar product by definition */
        sum += v[i] * u[i];

    return sum;
}

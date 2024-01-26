/* this program finds what the amount of the 1's that in 2 numbers and in the same place are */
#include <stdio.h>

int main() {
    unsigned long x;
    unsigned long y;
    unsigned long z; /* represents the '&' of x and y (x & y) */
    int c = 0; /* represents the amount of the 1's that in x and y and in the same place */
    
    printf("Enter the x value: ");
    scanf("%lu", &x);
    printf("The value of x is: %lu\n\n", x);

    printf("Enter the y value: ");
    scanf("%lu", &y);
    printf("The value of y is: %lu\n\n", y);

    z = x & y;
    
    for (; z; z >>= 1) /* run all over the z binary value */
        if (z & 1) /* check if z have 1 in specific location, if it applies, so it must be that x and y have 1 in the same place */ 
            c++;

    printf("The amount of the 1's that in x (at binary represention) and y (at binary represention) and in the same place are: %d\n", c);
    return 0; 
}

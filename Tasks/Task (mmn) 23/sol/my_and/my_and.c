/* this program shows the performence of the function 'my_and'. The function 'my_and' is a function with non-constant amount of parameters, and returns the and (&) between all the parameters. */ 
#include <stdio.h>
#include <stdarg.h>

unsigned int my_and(int count, ...); /* function with non-constant amount of parameters, and returns the and (&) between all the parameters. */

int main() {
    /* Example performance */
    unsigned int temp = my_and(3, 248, 471, 654);
    printf("my_and(3, 248, 471, 654) returned - in decimal: %d, in hexadecimal: %x.\n\n", temp, temp);

    temp = my_and(0);
    printf("my_and(0) returned - in decimal: %d, in hexadecimal: %x.\n\n", temp, temp);

    temp = my_and(1, 24);
    printf("my_and(1, 24) returned - in decimal: %d, in hexadecimal: %x.\n\n", temp, temp);

    temp = my_and(6, 1, 2, 3, 4, 5, 6);
    printf("my_and(6, 1, 2, 3, 4, 5, 6) returned - in decimal: %d, in hexadecimal: %x.\n", temp, temp);

    return 0;
}

unsigned int my_and(int count, ...) {
    unsigned int and_all; /* stores the '&' from all the arguments that received */
    va_list argp;
    va_start(argp, count);

    if (count <= 0) return -1; /* edge case */

    /* calculate the '&' from all the arguments that received */
    and_all = va_arg(argp, unsigned int);
    while (--count != 0)
        and_all &= va_arg(argp, unsigned int);

    va_end(argp);
    return and_all;
}

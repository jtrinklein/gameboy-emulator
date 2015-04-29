/* endtest
 * a small utility program to determine end-ness of a machine
 */
#include <stdio.h>

void main() {
    int* test;
    test = (int*)"\01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    if((*test) == 1) {
        printf("this is big endian");
    } else {
        printf("this is little endian");
    }
}

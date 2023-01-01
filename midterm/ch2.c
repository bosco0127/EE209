#include <stdio.h>
void Short_SU(void) {
    short s_val = 32768;
    unsigned short u_val = 32768;

    printf("\n=====Signed vs. Unsigned short=====\n");
    /* signed */
    printf("Print \"32768\" as signed: %hd\n", s_val);
    /* unsigned */
    printf("Print \"32768\" as unsigned: %hu\n", u_val);
    /* signed */
    printf("Print \"32768-1\" as signed: %hd\n", s_val-1);    
    /* unsigned */
    printf("Print \"32768-1\" as unsigned: %hu\n", u_val-1);  
}

int main(void) {
  Short_SU();
}

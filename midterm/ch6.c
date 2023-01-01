#include <stdio.h>

int main(){
  char a[3] = "";
  char (*p)[3]=&a;
  **p='H';
  *(*p+1)='i';
  printf("%ld\n",sizeof(p));
  printf("%s\n",*p);
  printf("%s\n",a);
  printf("%c\n",**p);
}

#include<stdio.h>

struct array {int a[10];} a1, a2={{1,2,3}};

typedef struct arrays {
  int a[10];
  char b[10];
} arrays;

int main(void) {
  arrays a = {{9,8,7,6},"arraystyp"};
  a1=a2;
  for(int i=0;i<10;i++)
    printf("%d ",a1.a[i]);
  printf("\n");
  for(int i=0;i<10;i++)
    printf("%d ",a.a[i]);
  printf("\n");
  printf("%s\n",a.b);
}

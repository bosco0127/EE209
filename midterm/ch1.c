#include <stdio.h>

/* Casting Example */
void Cast(void) {
  printf("\n=====Casting Example=====\n");
  printf("0==0U ? :%d\n",(0==0U));
  printf("-1<0 ? :%d\n",(-1<0));
  printf("-1>0U ? :%d\n",(-1>0U));
  printf("(unsigned int)-3>-2 ? :%d\n",((unsigned int)-3>-2));
  printf("2147483647>(int) 2147483648U ? :%d\n",(2147483647>(int) 2147483648U));
}

/* Expansion */
void Expansion(void) {
  short x = 15123;
  int ix = (int) x;
  short y = -15123;
  int iy = (int) y;
  short z = 0xAF02;
  char cz = (char) z;

  printf("\n=====Expansion Example=====\n");
  printf("x: %#X\n",x);
  printf("ix: %#X\n",ix);
  printf("y: %#X\n",y);
  printf("iy: %#X\n",iy);
  printf("z: %#X\n",z);
  printf("cz: %#X\n",cz);
}

/* Boolean Algebra */
void BitwiseBool(void){
  int A = 0b1010;
  int B = 0b1111;
  printf("\n=====Bitwise Boolean Example=====\n");
  printf("A = %#x\n",A);
  printf("B = %#x\n",B);
  printf("A&B = %#x\n",A&B);
  printf("A|B = %#x\n",A|B);
  printf("~A = %#x\n",~A);
  printf("A^B = %#x\n",A^B);
}

/* Shift operation */
void Shift(void){
  signed int x = 0b11110010;
  signed int y;
  unsigned int ux = 0b10100010;
  unsigned int uy;
  
  printf("\n=====Shift Example=====\n");
  printf("y=x<<2, y:%#X\n",y=x<<2);
  printf("uy=ux<<2, uy:%#X\n",uy=ux<<2);
  printf("y=x>>2, y:%#X\n",y=x>>2);
  printf("uy=ux>>2, uy:%#X\n",uy=ux>>2);
}

int main(void) {
  Cast();
  Expansion();
  BitwiseBool();
  Shift();
}

#include<stdio.h>

int fibo(int n) {
  int n_2=0;
  int n_1=1;
  int n_0=n_1+n_2;

  if(n<2) return n;

  for(int i=2; i<=n; i++) {
    n_0=n_1+n_2;
    n_2=n_1;
    n_1=n_0;
  }
  return n_0;
}

int main(void){
  int n=5;
  printf("%d fibo:%d\n",n,fibo(n));
}

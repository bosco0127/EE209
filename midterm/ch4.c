#include<stdio.h>

void swap(int v[], int i, int j) {

  int temp=v[i];
  v[i]=v[j];
  v[j]=temp;
}

void qsort(int v[], int left, int right) {
  static int LEN=10;
  static int n=1;
  int last=left;
  
  if(left >= right) return;
  
  swap(v,left,(left+right)/2);

  for(int i=left+1;i<=right;i++) {
    if (v[left]>v[i])
      swap(v,++last,i);
  }
  swap(v,left,last);
  printf("%d: {%d",n++,v[0]);
  for(int i=1; i<LEN; i++)
    printf(",%d",v[i]);
  printf("}\n");
  qsort(v,left,last-1);
  qsort(v,last+1,right);
}

int main(void) {
  
  int v[]={0,1,2,3,4,5,6,7,8,9};

  qsort(v,0,9);
}

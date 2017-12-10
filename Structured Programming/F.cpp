#include <stdio.h>
#include <stdlib.h>

void penalty(int **speed,int **violation,int i,int num,int limit);
int main() {
int N,**speed,**number,**violation,i,num,j,limit;
printf("Dwse ton arithmo twn kamerwn!\n");
scanf("%d",&N);
speed=(int **)malloc(N*sizeof(int *));
if (speed==NULL){
  printf("Den yparxei arketh diathesimi mnhmh gia to programma!\n");
  exit(1);
}
number=(int **)malloc(N*sizeof(int *));
if (number==NULL){
  printf("Den yparxei arketh diathesimi mnhmh gia to programma!\n");
  exit(1);
}
violation=(int **)malloc(N*sizeof(int *));
if (violation==NULL){
  printf("Den yparxei arketh diathesimi mnhmh gia to programma!\n");
  exit(1);
}

for (i=0;i<N;i++){
  printf("Dwse to orio taxythtas gia thn %d kamera!\n",i);
  scanf("%d",&limit);
  printf("Dwse ton arithmo twn aytokinitwn pou paraviasan to orio ths %d kameras!\n",i);
  scanf("%d",&num);
  speed[i]=(int *)malloc(num*sizeof(int));
  if (speed[i]==NULL){
  printf("Den yparxei arketh diathesimi mnhmh gia to programma!\n");
  exit(1);
  }
  number[i]=(int *)malloc(num*sizeof(int));
  if (number[i]==NULL){
  printf("Den yparxei arketh diathesimi mnhmh gia to programma!\n");
  exit(1);
  }
  for (j=0;j<num;j++){
    printf("Dwse tin taxythta kai ton aritho kykloforias tou aytokinitwn pou paraviase to orio taxythtas!\n"); 
    scanf("%d %d",&speed[i][j],&number[i][j]);
  }
  violation[i]=(int *)malloc(num*sizeof(int));
  if (violation[i]==NULL){
  printf("Den yparxei arketh diathesimi mnhmh gia to programma!\n");
  exit(1);
  }
  penalty(speed,violation,i,num,limit);
  for (j=0;j<num;j++)  
    printf("O paravatis me arithmo aytokinitou %d me taxythta %d anhkei sthn %d h kathgoria\n",number[i][j],speed[i][j],violation[i][j]); 
}

 
}
void penalty(int **speed,int **violation,int i,int num,int limit ) {
int j;
for (j=0;j<num;j++)
  if (speed[i][j] <= (limit+(10/100)*limit))
    violation[i][j]=1;
  else if (speed[i][j] <= (limit+(20/100)*limit))
    violation[i][j]=2;
  else
    violation[i][j]=3;
}

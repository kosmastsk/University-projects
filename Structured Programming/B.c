#include <stdio.h>
#include <math.h>
#define N 50
int main(){

int numA,numB,kA[N],coA[N][2],kB[N],coB[N][2],min_dist,over[N*N][2],i,j;
for (i=0;i<N;i++)
    kA[i]=coA[i][0]=kB[i]=coB[i][0]=coA[i][1]=coB[i][1]=0;
for (i=0;i<N*N;i++)
    over[i][0]=over[i][1]=0;
printf("Dwse ton arithmo twn shmeiwn ths etairias A\n");
scanf("%d",&numA);
for (i=0;i<numA;i++){
    printf("Dwse ton kwdiko kai tis syntetagmenes tou shmeiou %d ths etairias A antistoixa\n",i);
    scanf("%d",&kA[i]);
    scanf("%d",&coA[i][0]);
    scanf("%d",&coA[i][1]);
    }
printf("Dwse ton arithmo twn shmeiwn ths etairias B\n");
scanf("%d",&numB);
for (i=0;i<numB;i++){
    printf("Dwse ton kwdiko kai tis syntetagmenes tou shmeiou %d ths etairias B antistoixa\n",i);
    scanf("%d",&kB[i]);
    scanf("%d",&coB[i][0]);
    scanf("%d",&coB[i][1]);
    }
printf("Dwse thn elaxisth apodekti apostash\n");
scanf("%d",&min_dist);
int c=0;
char dist;
for (i=0;i<numA;i++){
    for (j=0;j<numB;j++){
        dist=sqrt((coA[i][0]-coB[j][0])*(coA[i][0]-coB[j][0])+(coA[i][1]-coB[j][1])*(coA[i][1]-coB[j][1]));
        printf("%d\n",dist);
        if (dist<min_dist) {
        over[c][0]=kA[i];
        over[c][1]=kB[j];
        c += 1;
        }
    }
}

for (i=0;i<4;i++){
    printf("%d %d\n",over[i][0],over[i][1]);
}
for (i=0;i<numA;i++){
    for (j=0;j<numB;j++){
        dist=sqrt((coA[i][0]-coB[j][0])*(coA[i][0]-coB[j][0])+(coA[i][1]-coB[j][1])*(coA[i][1]-coB[j][1]));
        if (dist<min_dist) {
            kA[i]=kB[j]=0;
        }
    }
}

printf("Den yparxei epikalypsh gia ta shmeia me tous eksis kodikous kai syntetagmenes\n");
for (i=1;i<numA;i++){
    if (kA[i] != 0)
        printf("%d (%d,%d) \n",kA[i],coA[i][0],coA[i][1]);
}
for (i=1;i<numB;i++){
    if (kB[i] != 0)
        printf("%d (%d,%d)\n",kB[i],coB[i][0],coB[i][1]);
}
}

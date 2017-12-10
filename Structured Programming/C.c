#include <stdio.h>
#define N 5
int main() {

int dis[N][N],i,j,thesi=0,lysh[N][N+1],w=0,min=0,varos=0,m,min_varos,tel_lysh;
for (i=0;i<N;i++) {
    for (j=0;j<=i;j++) {
        if (i != j){
            printf("Dwse to varos gia thn diadromi %d - %d \n",i,j);
            scanf("%d",&dis[i][j]);
            dis[j][i]=dis[i][j];
        }
        else {
        dis[i][j]=0;
        }
    }
}
for (i=0;i<N;i++) {
    for (j=0;j<N;j++) {
            if (dis[i][j]>min)
                min=dis[i][j];
    }
}
for (i=0;i<N;i++){
for (j=0;j<N && j!=i; j++) {
    if (dis[i][j]<min && dis[i][j]!=0){
        min=dis[i][j];
        thesi=j;
    }
varos += dis[i][j];
lysh[i][w]=j;
lysh[i][N]=varos;
w += 1;
}
for (m=0; m<N; m++)
    dis[m][i]=0;
}
min_varos=lysh[0][N];
for (i=0;i<N;i++){
    if (lysh[i][N]<min_varos){
        min_varos=lysh[i][N];
        tel_lysh=i;
    }
}
printf("H zhtoumenh diadromh einai: \n");
for (j=0;j<N+1;j++)
    printf("%d \n",lysh[tel_lysh][j]);
printf("me varos: %d \n",min_varos);
}

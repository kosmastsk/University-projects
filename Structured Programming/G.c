#include <stdio.h>
#include <string.h>
int main() {

FILE *numpers, *file;
char name[11],caller[11];
int N,i,flag;
long int out;
printf("Dwse ton arithmo twn syndromitwn\n");
scanf("%d",&N);
numpers=fopen("numpers","w");
if (numpers==NULL){
    printf("Den mporw na anoiksw to arxeio\n");
    exit(1);
}
for (i=0;i<N;i++){
    printf("Dwse ton arithmo klhshs tou syndromiti\n");
    scanf("%s",&name);
    fprintf(numpers,"%s\n",name);
}
fclose(numpers);

numpers=fopen("numpers","r");
if (numpers==NULL){
    printf("Den mporw na anoiksw to arxeio\n");
    exit(1);
}
while (1){
    printf("Dwse ton arithmo klhshs enos syndromiti\n");
    scanf("%s",&caller);
    if (caller[0] == 0 ) break;
    rewind(numpers);
    flag=1;
    for (i=0;i<N;i++){
        fscanf(numpers,"%s",&name);
        if (strcmp(name,caller)) continue;
        flag=0;
    }
    if (flag)
        printf("O arithmos pou dwsate den exei kataxwrithei\n");
    else {
        file=fopen("caller","ab");
        if (file==NULL){
            printf("Den mporw na anoiksw to arxeio\n");
            exit(1);
        }
        printf("Dwse ton arithmo pou ekane thn klhsh\n");
        scanf("%ld",&out);
        fseek(file,0,SEEK_END);
        fwrite(&out,sizeof out,1,file);
        fclose(file);
    }
}
rewind(numpers);
for (i=0;i<N;i++){
    fscanf(numpers,"%s",name);
    file=fopen("file","rb");
    if (file==NULL){
        printf("Den mporw na anoiksw to arxeio tou syndromiti %s h den yparxoyn klhseis pros ayton\n",name);
        continue;
    }
    printf("Ton syndromiti me arithmo %s exoun kalesei oi ekshs:\n",name);
    for (;;){
    flag=fread(&out,sizeof out,1,file);
    if (flag=1)
        printf("%ld\n",out);
    else
        break;
    }
    fclose(file);
}
}

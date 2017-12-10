#include <stdio.h>
int main()
{
int k,min;
printf("Grapse ton arithmo twn perioxwn\n");
scanf("%d",&k);
printf("Grapse ton elaxisto apodekto meso etisio arithmo imerwn iliofaneias\n");
scanf("%d",&min);
int i, days, plithos=0, max1=-1, max2=-1, max3=-1,thesi1=-1,thesi2=-1,thesi3=-1 ;
 for( i=1; i<=k; i++) {
         printf("Grapse ton meso etisio arithmo imerwn iliofaneias tis perioxis %d\n",i);
         scanf("%d", &days);
         if (days<min)
           printf("H perioxi %d exei xamilotero meso etisio arithmo imerwn iliofaneias\n",i);
         else
           plithos += 1 ;
          if (days>max1){
          max3=max2;
          thesi3=thesi2;
          max2=max1;
          thesi2=thesi1;
          max1=days;
          thesi1=i;}
         else if (days>max2){
          max3=max2;
          thesi3=thesi2;
          max2=days;
          thesi2=i; }
         else if (days>max3){
          max3=days;
          thesi3=i; }
 }
printf("O arithmos twn perioxwn me apodektes times imerwn iliofaneias einai %d\n",plithos);
if (plithos>=3)
    printf("Oi treis perioxes me to megisto arithmo imerwn einai %d , %d ,%d me antistoixo arithmo %d , %d , %d\n",max1,max2,max3,thesi1,thesi2,thesi3);
else
    printf("Oi perioxes einai ligoteres apo 3. Den mporoyn na emfanistoun oi 3 me ton megalytero arithmo imerwn");
}

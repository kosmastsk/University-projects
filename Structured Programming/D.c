#include <stdio.h>
#include <math.h>
#include <string.h>
double alarm(double x1, double y1, double z1, double x2, double y2, double z2, double secure_distance, int *result);
void main() {

int N=0,i=0,r,k;
double x1,y1,z1,x2,y2,z2,sec_dist, distance;
char id[11];
printf("Dwse ton arithmo twn aeroplanwn tou sminous\n");
scanf("%d",&N);
printf("Dwse thn apostash asfaleias\n");
scanf("%lf",&sec_dist);
for (;;){
    printf("Dwse tis syntetagmenes x,y,z tou aeroplanou pou ferei to systhma\n");
    scanf("%lf %lf %lf",&x1,&y1,&z1);
    if ( z1 == 0 )
        break;
    for (i=0;i<N-1;i++){
        do {
            printf("Dwse thn taytothta tou aeroplanou(To poly 10 xaraktires)\n");
            scanf("%s",id);
        } while (strlen(id)>10);
        printf("Dwse tis syntetagmenes x,y,z tou aeroplanou %s\n",id);
        scanf("%lf %lf %lf",&x2,&y2,&z2);
        distance = alarm(x1,y1,z1, x2,y2,z2,sec_dist, &r);
        k= *r;
        if (k=0)
            printf("To aeroplano %s exei apostash %lf\n",id,distance);
        else if (k=1){
            printf("Kitrinos synagermos\n");
            printf("To aeroplano %s exei apostasi %lf\n",id,distance);
        }
        else{
            printf("Kokkinos synagermos");
            printf("To aeroplano %s exei apostasi %d\n",id,distance);
        }
    }
}

}

double alarm(double x1, double y1, double z1, double x2, double y2, double z2, double secure_distance,int *result)
{
double distance,sec_dist;
int k;
distance=sqrt( (x1-x2)*(x1-x2) +(y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
if (distance>sec_dist)
    k=0;
else if (distance<sec_dist && distance>(sec_dist)*(3/4) )
    k=1;
else
    k=2;
r = &k;
return distance;
}


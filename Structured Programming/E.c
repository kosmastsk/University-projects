#include <stdio.h>
#include <math.h>
#define e 0.000000001

float nroot(float (*p)(float),float (*d)(float),float x0,int *counter1,int max);
float froot(float (*p)(float),float x0,float x1,int *counter2,int max);
float f(float x);
float der(float x);
void main(){
int counter1=0,counter2=0,max;
float r1,r2,x0,x1;
printf("Dwse ton megisto arithmo epanalipsewn gia thn efarmogi tou algorithmou\n"),
scanf("%d",&max);
printf("Dwse tin arxikh timh x0 gia thn prwth methodo\n");
scanf("%f",&x0);
r1=nroot(f,der,x0,&counter1,max);
printf("Dwse tis arxikes times x0 kai x1 gia thn deyterh methodo\n");
scanf("%f %f",&x0,&x1);
r2=froot(f,x0,x1,&counter2,max);

if (counter1<max)
    printf("H riza ths prwths methodou einai %f kai vrethike me %d epanalipseis\n",r1,counter1);
else
    printf("H akolouthia apoklinei. Den vrethike lysh\n");
if (counter2<max)
    printf("H riza ths deyterhs methodou einai %f kai vrethike me %d epanalipseis\n",r2,counter2);
else
    printf("H akolouthia apoklinei. Den vrethike lysh\n");
}

float f(float x){
return (3*(x*x)-9*x+6);
}

float der(float x){
return (6*x-9);
}

float nroot(float (*p)(float),float (*d)(float),float x0,int *counter1,int max){
float temp,x1;
*counter1++;
x1=x0-(p(x0)/der(x0));
if (*counter1<max){
    if (fabs(x1-x0)<e)
        return x1;
    else{
        x0=x1;
        x1=nroot(f,der,x0,counter1,max);
    }
}
else
    return 1;
}

float froot(float (*p)(float),float x0,float x1,int *counter2,int max) {
float x,temp;
*counter2++;
x=x1-(((x1-x0)*(p(x1)))/(p(x1)-p(x0)));
if (*counter2<max) {
    if (fabs(x-x1)<e)
        return x;
    else{
        x0=x1;
        x1=x;
        x=froot(f,x0,x1,counter2,max);
    }
}
else
    return 1;
}

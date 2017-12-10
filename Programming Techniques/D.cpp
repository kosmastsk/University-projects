//ΑΣΚΗΣΗ D-8255
#include <iostream>
#include <math.h>
#include <malloc.h>

using namespace std;

class Vector;
class matrix
{
    float **mat;
    int n;
public:
    matrix(int matrixsize,float **b);
    int getsize(){return n;}
    float* operator[](int i);
    Vector operator!(void);//friend γιατι θελω προσβαση στον πινακα vec
};

class Vector
{
    float *vec;
    int n;
public:
    Vector(int a,float *b);
    int getsize(){return n;}
    int operator>(Vector w);
    void operator=(matrix a);
    Vector operator-(Vector w);
    float operator++();
    float getvec(int i){return vec[i];}
};

//ΣΥΝΑΡΤΗΣΕΙΣ ΓΙΑ ΤΗ MATRIX
matrix::matrix(int matrixsize,float **b)
{
    int i,j;
    mat=b;
    n=matrixsize;
    if ((b=(float **)malloc(n*sizeof(float *)))==NULL)
        { //ΔΕΣΜΕΥΣΗ ΜΝΗΜΗΣ ΓΙΑ ΤΟΝ ΤΕΤΡΑΓΩΝΙΚΟ ΠΙΝΑΚΑ
        cout<<"Provlima mnhmhs!";
        exit(2);
        }
    for (i=0;i<n;i++)
    {
        if ((b[i]=(float *)malloc(n*sizeof(int)))==NULL)
        {
            cout<<"Provlima mnhmhs!";
            exit(2);
        }
    }
    for (i=0;i<n;i++)//ΔΙΑΒΑΣΜΑ ΤΩΝ ΣΤΟΙΧΕΙΩΝ ΤΟΥ nxn ΠΙΝΑΚΑ
    {
        for (j=0;j<n;j++)
        {
            cout<<"\nDwse to stoixeio "<<i<<","<<j<<" tou pinaka: ";
            cin>>b[i][j];
        }
    }
}

float* matrix::operator[](int i)
{
    return mat[i];
}

Vector matrix::operator!(void)
{
    float *p,sum;
    int i,j;
    Vector v1(getsize(),p);
    for (i=0;i<getsize();i++)
    {
        sum=0;
        for (j=0;j<getsize();j++)
        {
            if (i=j) continue;
            sum+=mat[i][j];
        }
        v1.getvec[i]=sum;//error: invalid types '<unresolved overloaded function type>[int]' for array subscript|
    }
    return v1;
}
//ΣΥΝΑΡΤΗΣΕΙΣ ΓΙΑ ΤΗ VECTOR-->
Vector::Vector(int a,float *b)
{
    n=a;
    vec=b;
    if ((vec=((float *)malloc(n*sizeof(float))))==NULL)
    {
        cout<<"Provlima mnhmhs!";
        exit(2);
    }
}

int Vector::operator>(Vector w)
{
    int flag=1,i;
    for (i=0;i<getsize();i++)
    {
        if (!(vec[i]>w.vec[i]))
        {
            flag=0;
        }
    }
    return flag;
}

void Vector::operator=(matrix a)
{
    int i;
    for (i=0;i<getsize();i++)
    {
        vec[i]=abs(a[i][i]);
    }
}

Vector Vector::operator-(Vector w)
{
    int i;
    float *q;
    Vector v3(getsize(),q);
    for (i=0;i<getsize();i++)
    {
        v3.vec[i]=vec[i]-w.vec[i];
    }
    return v3;
}

float Vector::operator++(void)
{
    float sum;
    int i;
    for (i=0;i<getsize();i++)
    {
        sum += vec[i];
    }
    return sum;
}

//MAIN -->
int main(){
    int matsize,i,flag1,flag2,j;
    float **m,*vec,*p;
    cout<<"\nDwse th diastash n tou nxn pinaka: ";
    cin>>matsize;
    matrix a(matsize,m);
    flag1=1;
    flag2=1;
    Vector x(matsize,vec);
    Vector v4=(!a);
    Vector absdiag(matsize,p); //error: conversion from 'matrix' to non-scalar type 'Vector' requested|
    absdiag=a;
    Vector sumd=v4-absdiag;//ο πινακας που προκυπτει απο τη διαφορα των στοιχειων που δεν ανηκουν στη διαγωνιο και των στοιχειων της διαγωνιου
    for (i=0;i<matsize;i++)
    {
        if (absdiag.getvec[i]>v4.getvec[i])//error: invalid types '<unresolved overloaded function type>[int]' for array subscript|
        {
            flag1=0;
        }
    }
    for (i=0;i<matsize;i++)
    {
        if (absdiag.getvec[i] > v4.getvec[i])
        {
            flag2=0;
        }
    }
    if (flag1==1)
    {
        cout<<"\nIsxyei h sxesh (1) gia oles tis grammes. \n";
    }
    if (flag2==1)
    {
        cout<<"Isxyei h sxesh (2) gia oles tis sthles. \n";
    }
    if(flag1==1)
    {
        cout<<"O pinakas einai diagwniws yperterwn!\n";
        for (i=0;i<matsize;i++){
            cout<<"H diafora gia thn "<<i<<" grammh einai: "<<sumd.getvec[i];
        }
        cout<<"To athroisma twn diaforwn einai: "<<++sumd;
    }
    if(flag2==1)
    {
        cout<<"O pinakas einai diagwniws yperterwn!\n";
        for (i=0;i<matsize;i++){
            cout<<"H diafora gia thn "<<i<<" sthlh einai: "<<sumd.getvec[i];
        }
        cout<<"To athroisma twn diaforwn einai: "<<++sumd;
    }
}

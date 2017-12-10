#include <iostream>
#include <malloc.h>
#include <math.h>
#include <conio.h>
#define n1 5
#define n2 4
#define M 5 //ο αριθμος των materials

using namespace std;

class material
{
    float maze,spweight;
    char id[11];
public:
    material();
    float getmaze(){return maze;}
    //float getspweight(){return spweight;}
    char *getid(){return id;}
    float getvol(){return maze/spweight;} //maze=vol*spweight
    void setmaze(float v){maze=spweight*v;}
};

material::material()
{
    cout<<"Dwse to onoma tou metallou(ews 10 xaraktires): ";
    cin>>id;
    cout<<"Dwse thn posothta tou "<<id<<": ";
    cin>>maze;
    cout<<"Dwse to eidiko varos tou "<<id<<": ";
    cin>>spweight;
    cout<<endl;
}

class product
{
public:
    product();
    virtual float calcvol()=0;
    virtual float getnumber()=0;
};

product **pr;//global μεταβλητες, χρειαζονται σε ολο το προγραμμα
int n=0;

product::product()
{
    cout<<"H product() ekteleitai! "<<endl;
    if (n==0)
    {
        pr=(product **)malloc(sizeof(product*));
        if (pr==NULL)
        {
            cout<<"Provlima mnhmhs!"<<endl;
            exit(2);
        }
        pr[0]=this;
        n++;
    }
    else
    {
        n++;
        pr=(product **)realloc(pr,n*sizeof(product*));
        if (pr==NULL)
        {
            cout<<"Provlima mnhmhs!"<<endl;
            exit(2);
        }
        pr[n-1]=this;
    }
}

void production(product **p,int N,material *m,int)
{
    int i,num,j,sum;
    float prodvol,totalvol,matvol,rprod;
    for (i=0;i<N;i++)
    {
        num=p[i]->getnumber();
        prodvol=p[i]->calcvol();
        totalvol=num*prodvol;
        for (j=0;j<M;j++)
        {
            matvol=m[j].getvol();
            if (matvol==0) continue;
            if (totalvol<=matvol)
            {
                cout<<num<<" temaxia apo to yliko "<<m[j].getid()<<endl;
                sum += num;
                matvol -= totalvol;
                m[j].setmaze(matvol);
                break;
            }
            else
            {
                rprod=m[j].getvol()/prodvol;
                if (rprod==0) continue;
                cout<<num<<" temaxia apo to yliko "<<m[j].getid()<<endl;
                sum =+ sum;
                matvol -= rprod*prodvol;
                m[j].setmaze(matvol);
                num -= rprod;
                totalvol=num*prodvol;
            }
        }
    }
    cout<<"Kataskeyastikan synolika "<<sum<<" temaxia "<<endl;

}

class product1:public product
{
    float r,height,vol;
    int number;
public:
    product1();
    float calcvol();
    float getnumber(){return number;}

};

product1::product1()
{
    cout<<"Dwse thn aktina vashs tou kylindrou: ";
    cin>>r;
    cout<<"Dwse to ypsos tou kylindrou: ";
    cin>>height;
    cout<<"Dwse ton arithmo twn temaxiwn: ";
    cin>>number;
    cout<<endl;
}

float product1::calcvol()
{
    //float vol;
    vol=M_PI*r*r*height;
    return vol;
}

class product2:public product
{
    float side,vol;
    int number;
public:
    product2();
    float calcvol();
    float getnumber(){return number;}
};

product2::product2()
{
    cout<<"Dwse thn pleyra tou kyvou: ";
    cin>>side;
    cout<<"Dwse ton arithmo twn temaxiwn: ";
    cin>>number;
    cout<<endl;
}

float product2::calcvol()
{
    vol=side*side*side;
    return vol;
}

int main()
{
    material mat[M];
    int N;
    //int i;
    product1 prod1[n1];
    product2 prod2[n2];
    N=n;
    /*for (i=0;i<5;i++)
    {
        mat[i]=material();
    }*/
    production(pr,N,mat,M);
    getch();
}

//ΕΡΓΑΣΙΑ F 8255
#include <iostream>
#include <malloc.h>
#define c 0.1 //ο συντελεστης για τον δευτερο τυπο συσκευων

using namespace std;

class device
{
protected:
    static device **d;
    static int n;
    float dist,power,engP;
    static int *type;
public:
    device();
    device(char *s);
    virtual float get_p()=0;
    static int getn(){return n;}
    static device **getd(){return d;}
    float get_dist(){return dist;}
    static int *get_type(){return type;}
    float get_engP(){return engP;}
};
int device::n;
device **device::d;
int* device::type;

device::device()
{
    cout<<"Antikeimeno device dhmiourgithike! "<<endl;
}

device::device(char *s)
{
    cout<<"Antikeimeno typou "<<s<<" dimiourgithike! ";
}

class device1:public device
{
    float tol,cross,length;
public:
    device1();
    device1(char *s);
    float get_p();
    void createUnits();
} dev1("Syskeyh 1");

device1::device1()
{
    cout<<"Dwse thn apostash apo ton pinaka: ";
    cin>>dist;
    cout<<endl<<"Dwse thn megisth onomastikh isxy ths syskeyhs: ";
    cin>>power;
    n++;
    if (n==1)
    {
        if ((d=(device**)malloc(n*sizeof(device*)))==NULL)
        {
            cout<<"Provlima mnhmhs!"<<endl;
            exit(2);
        }
        if ((type=(int *)malloc(n*sizeof(int)))==NULL)
        {
            cout<<"Provlima mnhmhs!";
            exit(2);
        }
    }
    else
    {
        if ((d=(device**)realloc(d,n*sizeof(device*)))==NULL)
        {
            cout<<"Provlima mnhmhs!"<<endl;
            exit(2);
        }
        if ((type=(int *)realloc(type,n*sizeof(int)))==NULL)
        {
            cout<<"Provlima mnhmhs!";
            exit(2);
        }
    }
    d[n-1]=this;
    type[n-1]=1;
}

void device1::createUnits()
{
    int m;
    cout<<"Dwse ton aritho twn syskeywn typou 1: ";
    cin>>m;
    for (int i=0;i<m;i++)
    {
        device1 temp1;
    }
}

device1::device1(char *s)
{
    createUnits();
}

float device1::get_p()
{
    return power;
}

class device2:public device
{
    float tol,cross,length;
public:
    device2();
    device2(char *s);
    float get_p();
    void createUnits();
}dev2("Syskeyh 2");

device2::device2()
{
    cout<<"Dwse thn apostash apo ton pinaka: ";
    cin>>dist;
    cout<<endl<<"Dwse thn megisth onomastikh isxy ths syskeyhs: ";
    cin>>power;
    cout<<endl<<"Dwse thn isxy tou kinhthra: ";
    cin>>engP;
    n++;
    if (n==1)
    {
        if ((d=(device**)malloc(n*sizeof(device*)))==NULL)
        {
            cout<<"Provlima mnhmhs!"<<endl;
            exit(2);
        }
        if ((type=(int *)malloc(n*sizeof(int)))==NULL)
        {
            cout<<"Provlima mnhmhs!";
            exit(2);
        }
    }
    else
    {
        if ((d=(device**)realloc(d,n*sizeof(device*)))==NULL)
        {
            cout<<"Provlima mnhmhs!"<<endl;
            exit(2);
        }
        if ((type=(int *)malloc(n*sizeof(int)))==NULL)
        {
            cout<<"Provlima mnhmhs!";
            exit(2);
        }
    }
    d[n-1]=this;
    type[n-1]=2;
}

void device2::createUnits()
{
    int m;
    cout<<"Dwse ton aritho twn syskeywn typou 2: ";
    cin>>m;
    for (int i=0;i<m;i++)
    {
        device2 temp2;
    }
}

device2::device2(char *s)
{
    createUnits();
}

float device2::get_p()
{
    float totalP;
    totalP=power+engP+c*engP;
    return totalP;
}

float get_total_P(int V,device **d,int num,int *fs,float *cl,int *t)
{
    float totalp,I;
    for (int j=0;j<num;j++)
    {
        I=d[j]->get_p() / V;//μετραει και το ρευμα στην εκκινηση του κινητηρα.
        //εδω κολλαει οταν τρεχει το προγραμμα
        //system("pause");
        if (I<10)// φτιαχνω τους πινακες που ζητουνται αναλογα με το μεγιστο ρευμα
        {
            fs[0]+=1;
            cl[0]+=d[j]->get_dist();
        }
        else if(I<16)
        {
            fs[1]+=1;
            cl[1]+=d[j]->get_dist();
        }
        else if(I<20)
        {
            fs[2]+=1;
            cl[2]+=d[j]->get_dist();
        }
        else if(I<=25)
        {
            fs[3]+=1;
            cl[3]+=d[j]->get_dist();
        }
        else
        {
            cout<<"Den epitrepetai h timh tou reymatos na yperbainei ta 25A.";
            continue;
        }
        totalp+=d[j]->get_p();
        if (t[j]==2)
        {
            totalp -= (d[j]->get_engP() )* c ; //δεν υπολογιζουμε το ρευμα στην εκκινηση του κινητηρα αρα το αφαιρω
           //πως θα χρησιμοποιησω τη συναρτηση get_engP αν ηταν γραμμενη στο σωμα της device2??
        }
    }
    return totalp;
}

int main()
{
    int v,n;//,n1,n2;
    cout<<"Dwse thn tash trofodosias: ";
    cin>>v;
    device **D;
    int *type;
    D=device::getd();
    n=device::getn();
    type=device::get_type();
    float totalPower;
    int fuse_store[4]={0,0,0,0};
    float cross_length[4]={0,0,0,0};
    /*cout<<endl<<"Dwse ton arithmo syskeywn tou prwtou typou: ";
    cin>>n1;
    cout<<endl<<"Dwse ton arithmo syskeywn tou deyterou typou: ";
    cin>>n2;
    */
    totalPower=get_total_P(v,D,n,fuse_store,cross_length,type);
    cout<<endl<<"H synolikh isxys pou tha apaithsei to ktirio einai: "<<totalPower<<endl;
    cout<<"Apo thn asfaleia me anoxh 10A xrhsimopoihthikan "<<fuse_store[0]<<" temaxia."<<endl;//εκτυπωση των πινακων
    cout<<"Apo thn asfaleia me anoxh 16A xrhsimopoihthikan "<<fuse_store[1]<<" temaxia."<<endl;
    cout<<"Apo thn asfaleia me anoxh 20A xrhsimopoihthikan "<<fuse_store[2]<<" temaxia."<<endl;
    cout<<"Apo thn asfaleia me anoxh 25A xrhsimopoihthikan "<<fuse_store[3]<<" temaxia."<<endl;
    cout<<"Apo ta kalwdia me diatomh 1.5 mm^2 xrhsimopoihthikan "<<cross_length[0]<<" metra."<<endl;
    cout<<"Apo ta kalwdia me diatomh 2.5 mm^2 xrhsimopoihthikan "<<cross_length[1]<<" metra."<<endl;
    cout<<"Apo ta kalwdia me diatomh 6 mm^2 xrhsimopoihthikan "<<cross_length[2]<<" metra."<<endl;
    cout<<"Apo ta kalwdia me diatomh 10 mm^2 xrhsimopoihthikan "<<cross_length[3]<<" metra."<<endl;
    return 0;
}

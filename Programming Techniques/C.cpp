//ΑΣΚΗΣΗ C-8255
#include <iostream>
#include <malloc.h>
#include <conio.h>
using namespace std;

class circuitS{
protected:
    int n,*R,*Imax;
    float Rsum;
public:
    circuitS(int number);
    circuitS(int number,int spS);
    float get_tres();
    void chek(float v);
    void out();
    ~circuitS();
};

class circuitP{
protected:
    int m,*R,*Imax;
    float Rsum;
public:
    circuitP(int number);
    circuitP(int number,int spP);
    float get_tres();
    void chek(float v);
    void out();
    ~circuitP();
};

class circuit:public circuitS, public circuitP { //παραγομενη απο τις αλλες 2 κλασεις
    float v;
public:
    circuit(float Vs,int n,int m);
    circuit(float Vs,int n,int spS,int m);
    circuit(int n,float Vs,int m,int spP);
    circuit(float Vs,int n,int spS,int m,int spP);
    float set_tres(int k);
    void chek(int k);
    ~circuit();
};

//ΣΥΝΑΡΤΗΣΕΙΣ ΓΙΑ ΤΗΝ ΚΛΑΣΗ circuitS -->
circuitS::circuitS(int number){
    cout<<"H klash circuitS gia 1 orisma dhmiourgithike!\n";
    int i;
    n=number;
    if ((R=(int *)malloc(n*sizeof(int)))==NULL){
        cout <<"Provlima mnhmhs!";
        exit(0);
    }
    if ((Imax=(int *)malloc(n*sizeof(int)))==NULL){
        cout <<"Provlima mnhmhs!";
        exit(0);
    }
    for (i=0;i<n;i++){
        cout<<"\nDwse thn timh kai to megisto reyma ths "<<i+1<<"hs antistashs ";
        cin>>R[i]>>Imax[i];
    }
}

circuitS::circuitS(int number,int spS){
    cout<<"H klash circuitS gia 2 orismata dhmiourgithike!\n";
    number=n;
    int i,rmax,rmin,imax,imin;
    cout <<"Dwse thn megisth kai thn elaxisth timh ths antistashs: ";
    cin>>rmax>>rmin;
    cout<<"\nDwse thn megisth kai thn elaxisth timh tou reymatos: ";
    cin>>imax>>imin;
    if ((R=(int *)malloc(n*sizeof(int)))==NULL){
        cout <<"Provlima mnhmhs!\n";
        exit(0);
    }
    if ((Imax=(int *)malloc(n*sizeof(int)))==NULL){
        cout <<"Provlima mnhmhs!\n";
        exit(0);
    }
    srand(spS);
    for (i=0;i<n;i++){
        do{
        R[i]=(rand()%100);
        } while (R[i]<rmax && R[i]>rmin);
        do{
        Imax[i]=(rand()%100);
        } while (Imax[i]<imax && Imax[i]>imin);
    }
}

float circuitS::get_tres(){
    int i;
    Rsum=0;
    for (i=0;i<n;i++){
        Rsum += R[i];
    }
    return Rsum;
}

void circuitS::chek(float V){
    int i;
    for (i=0;i<n;i++){
        if ((V/R[i])>Imax[i]){
            cout <<"H "<<i+1<<"h antistash tou kyklwmatos S tha kaei\n";
        }
    }
}

void circuitS::out(){
    int i;
    cout <<"\nTa stoixeia tou kyklwmatos S einai(timh antistashs/megisto reuma): \n";
    for (i=0;i<n;i++){
        cout<<R[i]<<" Ohm "<<Imax[i]<<" Amber\n";
    }
    cout <<"H synolikh antistash tou kyklwmatos S einai: "<<Rsum<< " Ohm \n";
}

circuitS::~circuitS(){
    free(R);
    free(Imax);
    cout<<"H klash circuitS katastrafike!\n";

}

//ΣΥΝΑΡΤΗΣΕΙΣ ΓΙΑ ΤΗΝ ΚΛΑΣΗ circuitP -->
circuitP::circuitP(int number){
    cout<<"H klash circuitP gia 1 orisma dhmiourgithike!\n";
    int i;
    m=number;
    if ((R=(int *)malloc(m*sizeof(int)))==NULL){
        cout <<"Provlima mnhmhs!";
        exit(0);
    }
    if ((Imax=(int *)malloc(m*sizeof(int)))==NULL){
        cout <<"Provlima mnhmhs!";
        exit(0);
    }
    for (i=0;i<m;i++){
        cout<<"\nDwse thn timh kai to megisto reyma ths "<<i+1<<"hs antistashs: ";
        cin>>R[i]>>Imax[i];
    }
}

circuitP::circuitP(int number,int spP){
    cout<<"H klash circuitP gia 2 orismata dhmiourgithike!\n";
    number=m;
    int i,rmax,rmin,imax,imin;
    cout <<"Dwse thn megisth kai thn elaxisth timh ths antistashs: ";
    cin>>rmax>>rmin;
    cout<<"\nDwse thn megisth kai thn elaxisth timh tou reymatos: ";
    cin>>imax>>imin;
    if ((R=(int *)malloc(m*sizeof(int)))==NULL){
        cout <<"Provlima mnhmhs!\n";
        exit(0);
    }
    if ((Imax=(int *)malloc(m*sizeof(int)))==NULL){
        cout <<"Provlima mnhmhs!\n";
        exit(0);
    }
    srand(spP);
    for (i=0;i<m;i++){
        do{
        R[i]=(rand()%100);
        } while (R[i]<rmax && R[i]>rmin);
        do{
        Imax[i]=(rand()%100);
        } while (Imax[i]<imax && Imax[i]>imin);
    }
}

float circuitP::get_tres(){
    int i;
    Rsum=0;
    for (i=0;i<m;i++){
        Rsum += 1/R[i];
    }
    return Rsum;
}

void circuitP::chek(float V){
    int i;
    for (i=0;i<m;i++){
        if ((V/R[i])>Imax[i]){
            cout <<"H "<<i+1<<"h antistash tou kyklwmatos P tha kaei\n";
        }
    }
}

void circuitP::out(){
    int i;
    cout <<"\nTa stoixeia tou kyklwmatos P einai(timh antistashs/megisto reuma): \n";
    for (i=0;i<m;i++){
        cout<<R[i]<<" Ohm "<<Imax[i]<<" Amber\n";
    }
    cout <<"\nH synolikh antistash tou kyklwmatos P einai: "<<Rsum<< " Ohm \n";
}

circuitP::~circuitP(){
    free(R);
    free(Imax);
    cout<<"H klash circuitP katastrafike!\n";
}

//ΣΥΝΑΡΤΗΣΕΙΣ ΓΙΑ ΤΗΝ ΚΛΑΣΗ circuit -->
circuit::circuit(float Vs,int n,int m): circuitS(n),circuitP(m){
    v=Vs;
}

circuit::circuit(float Vs,int n,int spS,int m): circuitS(n,spS),circuitP(m){
    v=Vs;
}

circuit::circuit(int n,float Vs,int m,int spP): circuitS(n),circuitP(m,spP){
    v=Vs;
}

circuit::circuit(float Vs,int n,int spS,int m,int spP): circuitS(n,spS),circuitP(m,spP){
    v=Vs;
}

float circuit::set_tres(int k){
    float s,p,Rsum;
    s=circuitS::get_tres();
    p=circuitP::get_tres();
    switch(k){
        case 0:
            Rsum=(1/s)+(1/p);
            break;
        default:
            Rsum=s+p;
    }
    Rsum=1/Rsum;
    return Rsum;
}

void circuit::chek(int k){
    float I,vS,vP;
    int i;
    I=v/set_tres(k);
    if (k){
        vS=I*(circuitS::get_tres());
        circuitS::chek(vS);
    }
    else{
        vP=I*(circuitP::get_tres());
        circuitP::chek(vP);
    }
}

circuit::~circuit(){
    cout<<"\nH klash circuit katastrafike!";
}
//MAIN-->
int main(){
    int n,m,seedS,seedP;//n=αριθμος αντιστασεων του S //m=αριθμος αντιστασεων του P
    float V;
    cout<<"\nDwse thn tash V pou tha efarmostei sto kyklwma: ";
    cin>> V;
    cout<<"\nDwse ton arithmo twn antistasewn twn kyklwmatwn S kai P antistoixa: ";
    cin>>n>>m;
    //1
    cout<<"\n\n PRWTH PERIPTWSH-->\n";
    circuit A(V,n,m);
    cout<<"Ektypwsh twn stoixeiwn twn kyklwmatwn S kai P\n";
    A.circuitS::out();
    A.circuitP::out();
    cout<<"\nEktypwsh twn antistasewn pou tha katastrafoun an ta S kai P syndethoun parallila: \n";
    A.chek(0);
    cout<<"Ektypwsh twn antistasewn pou tha katastrafoun an ta S kai P syndethoun parallila: \n";
    A.chek(1);
    //2
    cout<<"\n\n DEYTERH PERIPTWSH-->\n";
    cout<<"Dwse ton sporo gia thn paragwgh tyxaiwn arithmwn sto kyklwma S: ";
    cin>>seedS;
    circuit B(V,n,seedS,m);
    B.circuitS::out();
    B.circuitP::out();
    cout<<"\nEktypwsh twn antistasewn pou tha katastrafoun an ta S kai P syndethoun parallila: \n";
    B.chek(0);
    cout<<"Ektypwsh twn antistasewn pou tha katastrafoun an ta S kai P syndethoun parallila: \n";
    B.chek(1);
    //3
    cout<<"\n\n TRITH PERIPTWSH-->\n";
    cout<<"Dwse ton sporo gia thn paragwgh tyxaiwn arithmwn sto kyklwma P: ";
    cin>>seedP;
    circuit C(n,V,m,seedP);
    C.circuitS::out();
    C.circuitP::out();
    cout<<"\nEktypwsh twn antistasewn pou tha katastrafoun an ta S kai P syndethoun parallila: \n";
    C.chek(0);
    cout<<"Ektypwsh twn antistasewn pou tha katastrafoun an ta S kai P syndethoun parallila: \n";
    C.chek(1);
    //4
    cout<<"\n\n TETARTH PERIPTWSH-->\n";
    cout<<"Dwse ton sporo gia thn paragwgh tyxaiwn arithmwn sto kyklwma S: ";
    cin>>seedS;
    cout<<"Dwse ton sporo gia thn paragwgh tyxaiwn arithmwn sto kyklwma P: ";
    cin>>seedP;
    circuit D(V,n,seedS,m,seedP);
    D.circuitS::out();
    D.circuitP::out();
    cout<<"\nEktypwsh twn antistasewn pou tha katastrafoun an ta S kai P syndethoun parallila: \n";
    D.chek(0);
    cout<<"Ektypwsh twn antistasewn pou tha katastrafoun an ta S kai P syndethoun parallila: \n";
    D.chek(1);
    getch();
};

#include <iostream>
//#include <stdlib.h>
#include <string.h>
#define MAX 30 //ποιος θα ειναι ο μεγιστος αριθμος χαρακτηρων στα ονοματα και στον κωδικο!
using namespace std;

class Members {
    int id,friendsnumber ;
    char name[MAX], loginame[MAX], password[MAX];
    int friendids[10]={0} ;
public:
    Members();
    char *Log_In_name(void);
    char *Log_In_pwd(void);
    char *getname(void);
    void setid(int k);
    int getid();
    void Search(Members A,int n);
    void Add (int friendid);
    void Remove(int friendid);
    int *Friendslist(int &fn);
    int *PrintFriends(int personalid,int &fn,Members *A);
};

Members::Members(){
    cout << "/n Dwse to login name sou" ;
    cin >> loginame[MAX];
    cout << "/n Name?" ;
    cin >> name[MAX];
    cout << "/n Dwse to password sou" ;
    cin >> password[MAX];
    friendsnumber=0;
    id=0;
}

char Members::*Log_In_name(void){
    return loginame[MAX];
}

char Members::*Log_In_pwd(void){
    return password[MAX];
}

char Members::*getname(){
    return name[MAX];
}

void Members::setid(int k){
    id=k;
}

int Members::getid(){
    return id;
}

void Members::Search(Members A,int n){
    int flag,i;
    char searchname[MAX];
    flag=1;
    cout << "Dwse to onoma pou thes na anazhthseis!";
    cin >> searchname;
    for (i=0;i<n;i++){
        if (A[i].getname() == searchname)
            flag=0;
    }
    if (flag==1)
        cout << "/n To onoma pou anazhteite den vrethike";
    else
        cout << "To onoma pou anazhteitai vrethike, o xristis" <<searchname <<"exei ID:" <<i;
}

void Members::Add(int friendid){
    if (friendsnumber<10){
        friendids[friendsnumber]=friendid;
        friendsnumber++;
    }
    else
        cout << " Exei symplhrwthei to orio filwn\n.";
    return;
}

void Members::Remove(int friendid){
    int i;
    for (i=0;i<friendsnumber;i++){
        if (friendids[i]==friendid){
            friendsnumber--;
            if (i==friendsnumber) return;
            friendids[i]=friendids[friendsnumber];
        }
    }
    return;
}

int Members::*Friendslist(int &fn){
    fn=friendsnumber;
    int i;
    for (i=0;i<fn;i++){
        cout << friendids[i];
    }
    return;
}

int Members::*PrintFriends(int personalid,int &fn,Members *A){
    friendids=A[personalid-1].Friendslist(fn);
    int i;
    cout <<"Oi filoi tou"<<A[personalid-1].getname()<<"einai oi:";
        for (i=0;i<fn;i++){
            cout<<A[friendids[i]].getname()<<"/n";
        }
}

int main(){
    int n=0,choice,personalid=0,id,friendsnumber,flag,i;
    Members *A;
    char searchname[MAX],name[MAX],password[MAX],loginame[MAX];
    for (;;){ //Prwth atermwn epanalipsi!
        Members new;
        loginame=new.Log_In_name();
        password=new.Log_In_pwd();
        if (((name)== 'admin' ) && (password== 'admin' )) break;
        if (n == 0){ //Desmeyw meros ths mnhmhs gia to prwto melos. Meta xrhsimopoiw realloc gia ta ypoloipa
            if ((A=(Members *)malloc((sizeof)Members))==NULL ){
                cout << "Den yparxei arketh mnhmh";
                exit(1);
            }
        }
        else {
            if (A=((Members *)realloc(A ,(n+1)*(sizeof)Members)==NULL)){
                cout << "Den yparxei arketh mnhmh";
                exit(1);
            }
            name=new.getname();
            A[n]=new;
            A[n].setid(n+1);
            n++;
        }
        cout << "H eggrafh oloklhrwthike!";
    }
    for (;;){
        if (personalid==0){
            cout <<"Dwse login name kai password gia eisodo!";
            cin >> loginame >>password;
            if ((loginame== 'admin' ) && (password== 'admin' )) break;
            for (i=0;i<n;i++){
                if ((strcmp(A[i].Log_In_name(),name)) || (strcmp(A[i].Log_In_pwd,password))) continue;
                cout <<"Welcome :-) ";
                personalid=A[i].getid();
                break;
            }
        }
        if (personalid==0){
            cout << "Den exete eggrafei sto site!";
            continue;
        }
        cout <<"Gia anazhthsh melwn pathse to 1 /n";
        cout <<"Gia na prosthseis filo pathse to 2 /n";
        cout <<"Gia na diagrapseis filo pathse to 3 /n";
        cout <<"Gia na ektypwseis ta onomata twn filwn sou pathse to 4 /n";
        cin >> choice;
            switch(choice){
                case 1:
                    Search(Members A,int n);
                case 2:
                    cout<<"Dwse to ID tou atomou pou theleis na prostheseis ws filo.";
                    cin>>id;
                    A[personalid-1].Add(id);
                    cout <<"O/H"<<A[personalid-1].getname()<<"prostethike sth lista twn filwn sas/n";
                case 3:
                    cout<<"Dwse to ID tou atomou pou theleis na diagrapseis apo filo.";
                    cin>>id;
                    friends=A[personalid-1].Friendslist(friends);
                    flag=1;
                    for (i=0;i<fn;i++){
                        if (id==friendids[i]){
                            A[personalid-1].Remove(id);
                            cout <<"O/H"<<A[id-1].getname()<<"diagrafike apo filos!";
                            flag=0;
                            break;
                        }
                        if (flag=1){
                            cout <<"O/H"<<A[id-1].getname()<<"den anhkei sth lista twn filwn sou!";
                            break;
                        }
                    }
                case 4:
                    PrintFriends(int personalid,int &fn,Members *A);
                default:
                    personalid=0;
            }
    }
    return 0;
}


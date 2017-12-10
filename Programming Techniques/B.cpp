#include <iostream>
#include <malloc.h>
//#include <stdlib.h>

using namespace std;

class product{
    int store,category,code,offerfactor=0;
    float price,kgstore;
public:
    product(int category,int _code,int _store,float _price);//Για την πρωτη κατηγορια προιοντων
    product(int category,int _code,int _store,float _price,int _offerfactor);//Για τη δευτερη κατηγορια προιοντων
    product(int category,int _code,float _kgstore,float _price);//Για την τριτη κατηγορια προιοντων
    float sale(product odj,float *quantity);
    int getcode(){return code;}
    float getstore(){return store;}
    float getkgstore(){return kgstore;}
    float getprice(){return price;}
    int getofferfactor(){return offerfactor;}
    int getcategory(){return category;}
    ~product();
};

product::product(int category,int _code,int _store,float _price){//ΚΑΤΗΓΟΡΙΑ 1
    code=_code;
    store=_store;
    price=_price;
}

product::product(int category,int _code,int _store,float _price,int _offerfactor){//ΚΑΤΗΓΟΡΙΑ 2
    code=_code;
    store=_store;
    price=_price;
    offerfactor=_offerfactor;
}

product::product(int category,int _code,float _kgstore,float _price){//ΚΑΤΗΓΟΡΙΑ 3
    code=_code;
    store=_kgstore;
    price=_price;
}

float sale(product obj,float *quantity){
    float pay,store,n;
    int offer=obj.getofferfactor();
    if (2==obj.getcategory()){
        n=*quantity/offer;
        *quantity += n;//Το πηλικο της διαιρεσης ειναι ο αριθμος των δωρων που παιρνει ο πελατης
        store = obj.getstore()-*quantity;
    }
    else if(1==obj.getcategory()){
        store = obj.getstore()-*quantity;
    }
    else if(3==obj.getcategory()){
        store = obj.getkgstore()-*quantity;
    }
    pay=(*quantity)*obj.getprice();
    cout << "\nH posothta pou yparxei pleon sthn apothiki einai:"<<store<<"temaxia/kg";
    obj.getstore()=store; // Γιατι error σε αυτο ?
    cout << "\nTo poso plhrwmhs einai: "<<pay;
    return pay;
}

product::~product(){
    cout<<"free memory";
}

void main(){
    int num,category,i,_code,_offerfactor,_store,stop,flag;
    float sum=0,_kgstore,_price,pay,quantity;
    cout <<"Posa diaforetika proionta yparxoyn sto katasthma? ";
    cin >> num;
    product *p;
    if ((p=(product *)malloc(num*sizeof(product)))== NULL){
        cout <<"Provlima me th mnhmh \n";
        exit(1);
    }
    for (i=0;i<num;i++){
        do{
        cout <<"\nDwse thn kathgoria sthn opoia anhkei to proion. ";
        cin >> category;
        if (category != 1 && category != 2 && category != 3)
            cout <<"Lathos pliktrologisi!\n";
        } while (category != 1 && category != 2 && category != 3);
        cout<<"\nDwse ton kwdiko tou proiontos: ";
        cin >> _code;
        cout <<"\nDwse thn timh tou proiontos ana temaxio h kilo: ";
        cin >> _price;
        switch(category){
          case 1:
            cout<<"\nDwse to apothema pou yparxei: ";
            cin >> _store;
            p[i]=product(category,_code,_store,_price);
            break;
          case 2:
            cout<<"\nDwse to apothema pou yparxei: ";
            cin >> _kgstore;
            cout<<"\nDwse ton paragonta prosforas: ";
            cin>> _offerfactor;
            p[i]=product(category,_code,_kgstore,_price,_offerfactor);
            break;
          case 3:
            cout<<"\nDwse to apothema pou yparxei: ";
            cin >> _store;
            p[i]=product(category,_code,_store,_price);
            break;
          default:
            exit(0);
        }
    }
    cout <<"Kalwsirthate sto katasthma mas!\n";
    for(;;){
        cout << "\nDwse ton kwdiko tou proiontos: ";
        cin >> _code;
        cout<<"\nDwse ton arithmo twn temaxiwn/kg tou proiontos pou agorases: ";
        cin>> quantity;
        for (i=0;i<num;i++){
            if (_code==p[i].getcode()){
                flag=1;
                pay=sale(p[i],&quantity);
                if (p[i].getcategory()==2)
                    p[i].getkgstore() -= quantity; //error//ανανεωση του αποθεματος
                else
                    p[i].getstore() -= quantity; //error//lvalue required as left operand of assignment|
                break;
            }
        }
        if (flag==0) {
            cout<<"To proion pou epileksate den yparxei!";
            continue;
        }
        sum += pay;
        cout <<"\nAn exeis kai alla proionta patise to 1 ";
        cin>>stop;
        if (stop !=1) break;
    }
    cout << "\n\nTo synoliko poso plhrwmhs einai:"<<sum<<"\n" ;
    cout<<"Eyxaristoume pou mas protimhsate!";
}

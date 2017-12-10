//ERGASIA H_8255
#include <iostream>
#include <cstdlib>
#include <conio.h>

using namespace std;

template <class burner>
class device
{
    int n;
    burner *a;
  public:
    device();
    burner* b_short();
    int get_n(){return n;}
};

template <class burner>
device<burner>::device()
{
  cout<<"Give the number of the burners: ";
  cin>>n;
  a=new burner[n];
  if (a==0)
  {
    cout<<"Allocation error!";
    exit(2);
  }
}

template <class burner>
burner *device<burner>::b_short()
{
  int i,j;
  burner temp;//not burner*. it is just one array element!
  for (i=0;i<n;i++)//bubblesort
  {
    for (j=0;j<n-1;j++)
    {
      if (a[j].get_P_FR()<a[j+1].get_P_FR()) //Could i use a[j]<a[j+1] with overloading < ??
      {
        temp=a[j];
        a[j]=a[j+1];
        a[j+1]=temp;
      }
    }
  }
  return a;//returns the address of the table
}

template <class burner1, class burner2>
void choice(burner1 *a1,int n1,burner2 *a2,int n2,float min_p,float max_cost,int days)
{
  int i,j;
  float a1_tot_cost,a1_oper_cost,a2_tot_cost,a2_oper_cost;
  int payback=0;
  char *a1id,*a2id;
  for (i=0;i<n1;i++)
  {
    if (a1[i].get_P()>=min_p && a1[i].get_cost()<=max_cost)//the i burner is the most suitable from burner1(oil_burner)
      break;
  }
  for (j=0;j<n2;j++)
  {
    if (a2[j].get_P()>=min_p && a2[j].get_cost()<=max_cost)//the j burner is the most suitable from burner2(gas_burner)
      break;
  }
  a1id=a1[i].get_id();
  a2id=a2[j].get_id();
  a1_tot_cost=a1[i].get_cost();
  a1_oper_cost=a1[i].get_oper_cost(days);
  a2_tot_cost=a2[j].get_cost();
  a2_oper_cost=a2[j].get_oper_cost(days);
  if (a1_tot_cost<a2_tot_cost && a1_oper_cost<a2_oper_cost)
  {
    cout<<"The burner "<<a1id<<" is the most suitable!"<<endl;
  }
  else if (a1_tot_cost==a2_tot_cost && a1_oper_cost==a2_oper_cost)
  {
    cout<<"The burners "<<a1id<<" and "<<a2id<<" are equivalent!"<<endl;
  }
  else if (a1_tot_cost>a2_tot_cost && a1_oper_cost>a2_oper_cost)
  {
    cout<<"The burner "<<a2id<<" is the most suitable!"<<endl;
  }
  else if(a1_tot_cost<a2_tot_cost)
  {
    payback=(a2_tot_cost-a1_tot_cost)/(days*a2[j].get_FR()*a2[j].get_unitcost());
    cout<<"The burner "<<a2id<<" is the most suitable and the payback time is : "<<payback<<" days.";
  }
  else if(a1_tot_cost>a2_tot_cost)
  {
    payback=(a1_tot_cost-a2_tot_cost)/(days*a1[j].get_FR()*a1[j].get_unitcost());
    cout<<"The burner "<<a1id<<" is the most suitable and the payback time is : "<<payback<<" days.";
  }
  else
  {
    cout<<"None of the burners is suitable .";
  }
}

class oil_burner
{
    char id[21];
    float P,FR,unit_cost,buy_cost,serv_cost;//max_power,daily_consumption(fuel rate),cost per unit,unit_cost,buying_cost,annual_maintenance_cost
  public:
    oil_burner();
    float get_P(){return P;}
    float get_P_FR(){return P/FR;}
    float get_cost(){return buy_cost;}
    char *get_id(){return id;}
    float get_oper_cost(int days){return unit_cost*FR*days+serv_cost;}//operating cost
    float get_FR(){return FR;}
    float get_unitcost(){return unit_cost;}
};

oil_burner::oil_burner()
{
  cout<<"Give the id of the burner: ";
  cin>>id;
  cout<<"Give the max power of the burner: ";
  cin>>P;
  cout<<"Give the daily consumption of the burner: ";
  cin>>FR;
  cout<<"Give the cost per unit: ";
  cin>>unit_cost;
  cout<<"Give the buying cost of the burner: ";
  cin>>buy_cost;
  cout<<"Give the annual maintenance cost: ";
  cin>>serv_cost;
}

class gas_burner
{
    char id[21];
    float P,FR,unit_cost,buy_cost,serv_cost,con_cost;////max_power,daily_consumption(fuel rate),cost per unit,buying_cost,annual_maintenance_cost,connection_cost
  public:
    gas_burner();
    float get_P(){return P;}
    float get_P_FR(){return P/FR;}
    char *get_id(){return id;}
    float get_cost(){return con_cost+buy_cost;}
    float get_oper_cost(int days){return serv_cost+days*FR*unit_cost;}
    float get_FR(){return FR;}
    float get_unitcost(){return unit_cost;}
};

gas_burner::gas_burner()
{
  cout<<"Give the id of the burner: ";
  cin>>id;
  cout<<"Give the max power of the burner: ";
  cin>>P;
  cout<<"Give the daily consumption of the burner: ";
  cin>>FR;
  cout<<"Give the cost per unit: ";
  cin>>unit_cost;
  cout<<"Give the buying cost of the burner: ";
  cin>>buy_cost;
  cout<<"Give the annual maintenance cost: ";
  cin>>serv_cost;
  cout<<"Give the connection cost of the burner with the network: ";
  cin>>con_cost;
}

int main()
{
  float min_p,money;
  int days;
  cout<<"Give the minimum power that is needed: ";
  cin>>min_p;
  cout<<"Give the amount of money that is available for buying and installing the burner: ";
  cin>>money;
  cout<<"Give the number of the days that heating works: ";
  cin>>days;
  cout<<"Oil burners --> ";
  device<oil_burner>a;
  cout<<"Gas burners --> ";
  device<gas_burner>b;
  choice(a.b_short(),a.get_n(),b.b_short(),b.get_n(),min_p,money,days);
  getch();
}

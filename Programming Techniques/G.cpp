//ERGASIA G 8255
#include <iostream>
#include <cstdlib>

using namespace std;

//CLASS NEURON --->
class neuron
{
    int id,state,near_number;
    int *near_id;
    float *synapse,threshold;
  public:
    neuron();
    friend istream &operator>(istream &c,neuron &neur);
    void* operator new[](size_t size);
    void operator delete(void *p);
    int get_id(){return id;}
    int get_state(){return state;}
    int get_near_num(){return near_number;}
    float get_thres(){return threshold;}
    float get_synapse(int i){return synapse[i];}
    int get_near_id(int i){return near_id[i];}
    void set_state(int i){state=i;}
    ~neuron();
};

neuron::neuron()
{
  cout<<"Neuron object created."<<endl;
}

istream &operator>(istream &c,neuron &neur)
{
  int i;
  for (i=0;i<neur.near_number;i++)
  {
    cout<<"Give the id of the "<<i<<" neuron that is associated to the "<<neur.id<<" neuron:";
    c>>neur.near_id[i];
    cout<<"Give the weight of the synapse: ";
    c>>neur.synapse[i];
  }
  cout<<endl<<"Give the trigger threshold for the "<<neur.id<<" neuron: ";
  c>>neur.threshold;
  cout<<endl<<"Give an initial value for the "<<neur.id<<" neuron state: ";
  c>>neur.state;
  return c;
}

void *neuron::operator new[](size_t size)
{
  cout<<"Using overloaded operator neuron::new[]"<<endl;
  neuron *n;//in pointer n I save neuron objects
  int num;
  if ((n=(neuron*)malloc(size))==NULL)
  {
    cout<<"Allocation error."<<endl;
    exit(2);
  }
  num=size/sizeof(neuron);//to find how many neurons exist !ATTENTION!
  for (int i=0;i<num;i++)
  {
    n[i].id=i;
    cout<<"How many neurons are associated to the "<<n[i].id<<" neuron?";
    cin>>n[i].near_number;
    n[i].near_id=new int[n[i].near_number];//Create a table for each object.table's size is the number of the near neurons
    n[i].synapse=new float[n[i].near_number];//the same as above
    cin>n[i];//Using the extractor from class neuron
  }
  return n;
}

void neuron::operator delete(void *p)
{
  cout<<"Using the overloaded neuron::delete."<<endl;
  free(p);
}

neuron::~neuron()
{
  cout<<"Neuron object deleted."<<endl;
}

//CLASS NETWORK --->
class network
{
  neuron *p;
  int netstate,num;//for the netstate : 1=stable , 0=unstable
public: //////////num=the total number of neurons
  network();
  void *operator new(size_t size);
  friend ostream &operator<(ostream &c,network q);
  void operator delete(void *p);
  int calk_state();
  ~network();
};

network::network()
{
  cout<<"Network object created!"<<endl;
}

void *network::operator new(size_t size)
{
  cout<<"Using overloaded network::new"<<endl;
  network *a;
  if ((a=(network*)malloc(size))==NULL)
  {
      cout<<"Allocation error."<<endl;
      exit(2);
  }
  cout<<"Give the total number of the neurons: ";
  cin>>a->num;
  a->p=new neuron[a->num];//Using the overloaded new for neuron objects.
  return a;
}

ostream &operator<(ostream &c,network q)
{
  if (q.netstate==0)
  {
    cout << "Network's state is: unstable" << endl;
  }
  else
  {
    cout << "Network's state is: stable" << endl;
  }
  for (int i=0;i<q.num;i++)
  {
    if (q.p[i].get_state()==1)
    {
      cout<<"The neuron "<<q.p[i].get_id()<<" is stable."<<endl;
    }
    else
    {
      cout<<"The neuron "<<q.p[i].get_id()<<" is unstable."<<endl;
    }
  }
  return c;
}

void network::operator delete(void *p)
{
  cout << "Using overloaded network::delete"<<endl;
  free(p);
}

network::~network()
{
    cout<<"Network object deleted."<<endl;
}

int network::calk_state()
{
  int max_rep,i,flag,j,near,old_state,k,n_id;
  float sum;//sum=Ó (state*synapse)
  int *new_state;
  new_state=new int[num];
  cout<<"Give the maximum number of redefining the status of the neurons.";
  cin>>max_rep;
  for (i=0;i<max_rep;i++)//To check how many repeats it does
  {
    flag=1;
    for (j=0;j<num;i++)//a loop to check all of the neurons
    {                   //neurons belong to the pointer p of the class network
      sum=0;
      near=p[j].get_near_num();//How many "neighbors" the neuron has
      old_state=p[j].get_state();
      for (k=0;k<near;k++)//a loop for all the neighboors of the j neuron
      {
        n_id=p[j].get_near_id(k);
        sum+=p[j].get_synapse(k)*p[n_id].get_state();
      }
      if (sum>p[j].get_thres())
      {
        new_state[j]=1;
      }
      else
      {
        new_state[j]=-1;
      }
      if (old_state!=new_state[j])
      {
        flag=0;
      }
    }
    if (flag) break;
    for (k=0;k<num;k++)//A loop for all the neurons that their state has changed
    {
      p[k].set_state(new_state[k]);
    }
  }
  netstate=flag;
  delete new_state;
  return netstate;
}

//MAIN FUNCTION --->
int main()
{

  network *obj;
  obj=new network;
  obj->calk_state();
  cout < obj;
  delete obj;
}

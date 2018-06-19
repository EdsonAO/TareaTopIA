#include <bits/stdc++.h>
using namespace std;

#define e 2.71828182846

typedef float SALIDA;
typedef vector<float> PESOS;
typedef vector<vector<float> > INP;
typedef vector<SALIDA> OUT;
struct Neurona{
    SALIDA y;
    PESOS W;
};
struct Capa{
    int NroNeu;
    vector<Neurona> vNeu;
};


class BackProp{
    private:
        float umbral;
	    vector<Capa> Layer;
        vector<Capa> Last_Layer;
        vector<int> Func;
        vector<OUT> OO;
        INP II;
        float Tasa;
    public:
        BackProp(float n, float t);
        Capa create_layer(int inp, int out);
        float Activation(float acc, int f);
        void Start(int inp, int out, vector<int> hiden, vector<int> fu);
        float Get_Error(OUT sd);
        float Error_Total(vector<float> E);
        float Prod(Capa a, PESOS b);
        void S_I(vector<float> v);
        void S_H(int a, int b);
        void S_O(int a, int b);
        float Forward(vector<float> i, OUT o);
        vector<float> Change_WO(OUT o);
        void Change_WH(vector<float> inp, vector<float> deltas);
        void Backward(vector<float> i, OUT o);
        void Train(INP In, vector<OUT> Out);
        void Test(vector<float> query);
};

BackProp::BackProp(float n, float t)
{
    umbral=n;
    Tasa=t;
}

Capa BackProp::create_layer(int inp, int out)
{
    int i;
    Capa O;
    O.NroNeu=out;
    vector<Neurona> t;
    uniform_real_distribution<float> distribution(0.0,0.3);
    random_device rd;
    default_random_engine generator(rd());
    //Crea el numero de neuronas 
    for(i=0;i<out;i++){
        Neurona tmp;
        tmp.y=1;
        PESOS w;
        //Llena cada neurona con pesos random de 0 a 0.3
        for(int j=0;j<inp;j++){
            w.push_back(distribution(generator));
        }
        tmp.W=w;
        t.push_back(tmp);
    }
    O.vNeu=t;
    return O;   
}

float BackProp::Activation(float acc, int f)
{
    switch (f)
    {   
        //Sigmoidal de 0 a 1
        case 1:
                return 1.0/(1.0+pow(e,-acc));
        //ReLU 0 o X
        case 2:
                return max(acc,float(0));
        //TanH de -1 a 1
        case 3:
                return tanh(acc);
        //Raiz de -1 a 1
        case 4:
                return acc/sqrt(1+pow(acc,2));                
        //Default                
        default:
                return 1.0/(1.0+pow(e,-acc));
    }
}

void BackProp::Start(int inp, int out, vector<int> hiden, vector<int> fu)
{
    Func=fu;
    //Crea la primera capa que conecta a los inputs
    Layer.push_back(create_layer(inp+1,hiden[0]+1));
    //Crea las capas ocultas
    for(int i=1;i<hiden.size();i++){
        Layer.push_back(create_layer(hiden[i-1]+1,hiden[i]+1));
    }
    //Crea la capa de salida
    Layer.push_back(create_layer(hiden[hiden.size()-1]+1,out+1));
}

float BackProp::Get_Error(OUT sd)
{   float EE=0;
    vector<Neurona> R = Layer[Layer.size()-1].vNeu;
    for(int i=0; i < sd.size();i++){
      //  cout<<sd[i]<<"=="<<R[i+1].y<<endl;
        EE+=pow(sd[i]-R[i+1].y,2);
    }
    return EE/2;
}

float BackProp::Error_Total(vector<float> E)
{
    float error=0;
    for(auto err : E)
        error+=pow(err,2);
    return error/E.size();   
}

float BackProp::Prod(Capa a, PESOS b)
{
    float sum=0;
    vector<Neurona> tmp=a.vNeu;
    for(int i=0;i<b.size();i++){
        sum+=tmp[i].y*b[i];
    }
    return sum;
}

void BackProp::S_I(vector<float> v)
{
    for(auto &NN : Layer[0].vNeu){
        float sum=0;
        for(int i=0;i<v.size();i++){
            sum+=v[i]*NN.W[i];
        }
        NN.y = Activation(sum,Func[0]);
    }
    //Valor del bias que debe ser 1
    Layer[0].vNeu[0].y=1;
}

void BackProp::S_H(int a, int b)
{
    Capa A = Layer[a];
    for(auto &NN : Layer[b].vNeu){
        NN.y = Activation(Prod(A,NN.W),Func[b]);
    }
    Layer[b].vNeu[0].y=1;
}

void BackProp::S_O(int a, int b)
{
    Capa A = Layer[a];     
    for(auto &NN : Layer[b].vNeu){
        NN.y = Activation(Prod(A,NN.W),Func[b]);
    }
    Layer[b].vNeu[0].y=1;
}

float BackProp::Forward(vector<float> i, OUT o)
{    
    S_I(i);
#   pragma omp parallel for num_threads(Layer.size()-2)
    for(int c=1;c<Layer.size()-1;c++){
        S_H(c-1,c);
    }
    S_O(Layer.size()-2,Layer.size()-1);
    return Get_Error(o);
}

vector<float> BackProp::Change_WO(OUT o)
{
    Last_Layer=Layer;
    float delta;
    vector<float> R;
    vector<Neurona> salida = Layer[Layer.size()-1].vNeu;
    vector<Neurona> hiden = Layer[Layer.size()-2].vNeu;
    for(int i=1; i < salida.size();i++){
        delta=-(o[i-1]-salida[i].y)*(1.0-salida[i].y)*salida[i].y;
        for(int j=0; j < hiden.size();j++){
            Layer[Layer.size()-1].vNeu[i].W[j] = (Layer[Layer.size()-1].vNeu[i].W[j])-(Tasa*delta*hiden[j].y);
        }
        R.push_back(delta);
    }
    return R;
}

void BackProp::Change_WH(vector<float> inp, vector<float> deltas)
{
    int i,j,k;
    vector<Neurona> n_current;
    vector<Neurona> n_last;
    vector<float> a_deltas,n_deltas;
    
    a_deltas=deltas;
    // Cambio de capas internas
    for(int capa_=Layer.size()-2;capa_>0;capa_--){
        n_current=Last_Layer[capa_].vNeu;
        n_last=Last_Layer[capa_-1].vNeu;
        float delta;
        for(i=1;i<n_current.size();i++){
            float sum=0;
            for(k=0;k<a_deltas.size();k++)
                sum += (Last_Layer[capa_+1].vNeu[k].W[i])*a_deltas[k]; // Obviar bias?
            delta = sum*(1-n_current[i].y)*n_current[i].y;
            for(j=0;j<n_last.size();j++){
                Layer[capa_].vNeu[i].W[j]=Last_Layer[capa_].vNeu[i].W[j]-(Tasa*delta*n_last[j].y);
            }
            n_deltas.push_back(delta);        
        }
        a_deltas=n_deltas;
        n_deltas.clear();
    }
    // Cambio de capa inicial con los inputs
    n_current=Layer[0].vNeu;
    float delta;
    for(i=1;i<n_current.size();i++){
        float sum=0;
        for(k=0;k<a_deltas.size();k++)
            sum += (Last_Layer[1].vNeu[k+1].W[i])*a_deltas[k]; // Obviar bias?    
        delta = sum*(1-n_current[i].y)*n_current[i].y;
        for(j=0;j<inp.size();j++){
            Layer[0].vNeu[i].W[j]=Layer[0].vNeu[i].W[j]-(Tasa*delta*inp[j]);
        }     
    }
}

void BackProp::Backward(vector<float> i, OUT o)
{
    vector<float> d=Change_WO(o);
    Change_WH(i,d);
}

void BackProp::Train(INP In, vector<OUT> Out)
{   int i;
    II=In;
    OO=Out;
    float Error=99999;
    vector<float> Errores; 
    while(umbral<Error){
        Errores.clear();
        for(i=0;i<II.size();i++){
            Error=Forward(II[i],OO[i]);
            Backward(II[i],OO[i]);
            Errores.push_back(Forward(II[i],OO[i]));
        }
        Error=Error_Total(Errores);
        cout<<"Error:: "<<Error<<endl;
    }
}

void BackProp::Test(vector<float> query)
{
    S_I(query);
    for(int c=1;c<Layer.size()-1;c++){
        S_H(c-1,c);
    }
    S_O(Layer.size()-2,Layer.size()-1);
    for(int val=1; val< Layer[Layer.size()-1].vNeu.size();val++)
        cout<<"[ "<<Layer[Layer.size()-1].vNeu[val].y<<" ";
    cout<<"]"<<endl;    
}

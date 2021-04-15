#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
class Scale;
//Груз
class Cargo{
    private:
        int weight;//вес груза
    public:
        Cargo(int _weight) : weight(_weight){};
        int getWeight(){ return weight; };
        void setWeight(int w){weight = w;};
     
};

class Scale{
    protected:
        int carrying;//грузоподъемность
        Cargo* cargo;
    public:
        Scale(int _carrying, Cargo& _cargo) : carrying(_carrying){ cargo = &_cargo;};
        vector<int>* bin_wght();
        Cargo* getCargo(){return cargo;};
        void setCargo(Cargo* c){cargo = c;};
    };

class Weighing{
    private:
        Scale* scale;
    public:
        Weighing(Scale& _scale){
            scale = &_scale;
            };
        void setWeighing(Scale* s){scale = s;};
    friend int query();
    void result();
};
int query(){
    int weight;
    cout<< "Введите вес груза: ";
    cin >> weight;
    return weight;
}
void Weighing :: result(){
    vector <int> bin_weight;// двоичное представление веса 
    int w = scale->getCargo()->getWeight();

    while( w >0){
            bin_weight.push_back(w%2);
            w/=2;
        }

    cout << "Взятые гири: ";

    for(int i=0; i<bin_weight.size(); ++i)
        {
            if (bin_weight[i] == 1 )
                cout<<pow(2,i)<<" ";
        }
    cout<<endl;
    bin_weight.clear();
}
int main(int argc,char* argv[]){

    if ( argc<2 ){
        cout << "Не указана грузоподъемность весов!\n";
        return 1;
    }
    int weight = query();//запрос веса груза
    Cargo* cargo = new Cargo(weight); 
    Scale* scale = new Scale(atoi(argv[1]),*cargo);
    Weighing* weighing = new Weighing(*scale);
    while(true){
        if (weight > atoi(argv[1])){
            cout<< "Груз превышает грузоподъемность\n";
            return 1;
        }
        weighing->result();
        weight = query();
        cargo->setWeight(weight);
        scale->setCargo(cargo);
        weighing->setWeighing(scale);
    }
    delete cargo;
    delete scale;
    delete weighing;
}    
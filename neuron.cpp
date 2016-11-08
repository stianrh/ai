#include <iostream>
#include <cmath>
#include "network.h"
#include "layer.h"
#include "neuron.h"
#include "utils.h"
using namespace std;

Neuron::Neuron(int n, Layer * l): numInput(n), parent_l(l){
    for(int i = 0; i < numInput + 1; i++){

        weight.push_back(0.5*RandomClamped()+0.5);
    }
    lRate = &parent_l->parent_nw->lRate;
};

void Neuron::print(){
    for(int i = 0; i < (int)weight.size(); i++){
        //cout << "weight w" << i << ": " << weight[i] << endl;
    }
};

void Neuron::calcOutput(){
    double a = 0;
    int p = 1;
    for(int i = 0; i < numInput; i++){
        a += inputs[i]->output*weight[i];
    }
    a += weight.back();
    netInput = a;
    output = 1/(1+exp(-a/p));
}

void Neuron::calcDelta(double cd_in){
    delta = cd_in*(output*(1-output));
    cd = cd_in;
}
void Neuron::calcError(){
    new_weight.clear();
    double lR =  *lRate;
    for(int i = 0; i < numInput; i++){
        new_weight.push_back(weight[i]-lR*delta*inputs[i]->output);
    }
    new_weight.push_back(weight.back()-lR*delta);
}

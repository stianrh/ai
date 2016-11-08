#include <iostream>
#include "layer.h"
#include "neuron.h"
using namespace std;

Layer::Layer(int n, int numInput, bool out, Network * netw): numNeurons(n), isOutputLayer(out), parent_nw(netw){
    for(int i = 0; i < n; i++){
        Neuron * n = new Neuron(numInput, this);
        neurons.push_back(n);
    }
}

void Layer::connect(Layer * l){
    for(int i = 0; i < numNeurons; i++){
        for(int j = 0; j < neurons[i]->numInput; j++){
            neurons[i]->inputs.push_back(l->neurons[j]);
            l->neurons[j]->outputs.push_back(neurons[i]);
        }
    }
    parentLayer = l;
    l->childLayer = this;
}

void Layer::propagate(){
    
    for(int i = 0; i < numNeurons; i++){
        neurons[i]->print();
        
        neurons[i]->calcOutput();
    }
    if(!isOutputLayer){
        childLayer->propagate();
    }
}

void Layer::error(double ideal[]){
    double cd;
    for(int i = 0; i < numNeurons; i++){
        if(isOutputLayer){
            cd = neurons[i]->output - ideal[i];
        }else{
            cd = 0;
            for(int j = 0; j < childLayer->numNeurons; j++){
                cd += (neurons[i]->outputs[j]->delta * neurons[i]->outputs[j]->weight[i]);
            }
        }
        neurons[i]->calcDelta(cd);
        neurons[i]->calcError();
    }
}

void Layer::updateWeights(){
    for(int i = 0; i < numNeurons; i++){
        neurons[i]->weight = neurons[i]->new_weight;
    }
}

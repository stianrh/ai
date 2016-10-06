#include <iostream>
#include "nn.h"
#include "utils.h"
#include <cmath>
using namespace std;


Neuron::Neuron(int n, double l): numInput(n), lRate(l){
    for(int i = 0; i < numInput + 1; i++){
        weight.push_back(RandomClamped());
    }
};

void Neuron::calcOutput(){
    double a = 0;
    int p = 1;
    for(int i = 0; i < numInput; i++){
        a += inputs[i]->output*weight[i];
    }
    a -= weight.back();
    output = 1/(1+exp(-a/p));
    //cout << "OUT: " << output << endl;
}

void Neuron::calcDelta(double cd){
    delta = cd*(output*(1-output));
}
void Neuron::calcError(){
    new_weight.clear();
    for(int i = 0; i < numInput; i++){
        new_weight.push_back(weight[i]-lRate*delta*inputs[i]->output);
    }
}


void Neuron::print(){
    for(int i = 0; i < (int)weight.size(); i++){
        //cout << "weight w" << i << ": " << weight[i] << endl;
    }
};

Layer::Layer(int n, int numInput, bool out, double lr): numNeurons(n), isOutputLayer(out), lRate(lr){
    for(int i = 0; i < n; i++){
        neurons.push_back(new Neuron(numInput, lRate));
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
    double cd = 0;
    for(int i = 0; i < numNeurons; i++){
        if(isOutputLayer){
            cd = neurons[i]->output - ideal[i];
            //cout << "is output" << endl;
        }else{
            cd = 0;
            for(int j = 0; j < childLayer->numNeurons; j++){
                cd += neurons[i]->outputs[j]->delta * neurons[i]->outputs[j]->weight[j];
            }

        }
        //cout << "childd: " << cd << " " << i << endl;
        neurons[i]->calcDelta(cd);
        //cout << "delta: " << neurons[i]->delta << endl;
        neurons[i]->calcError();
        //cout << "old w: " << neurons[i]->weight[i]<< endl;
        //cout << "new w: " << neurons[i]->new_weight[i]<< endl << endl;
    }
}

void Layer::updateWeights(){
    for(int i = 0; i < numNeurons; i++){
        neurons[i]->weight = neurons[i]->new_weight;
    }
}

Network::Network(int in, int out, int hid, int n, double lr):
    numInput(in), 
    numOutput(out), 
    numHiddenLayer(hid), 
    numNeuron(n),
    lRate(lr)
{
    layers.push_back(new Layer(numInput, 1, 0, lRate));
    Layer * l;
    for(int i = 0; i < numHiddenLayer; i++){
        if(i == 0){
            l = new Layer(numNeuron, numInput, 0, lRate);
        }else{
            l = new Layer(numNeuron, numNeuron, 0, lRate);
        }
        l->connect(layers.back());
        layers.push_back(l);
    }
    l = new Layer(numOutput, numNeuron, 1, lRate);
    l->connect(layers.back());
    layers.push_back(l);
    output = new double[out];
}

void Network::setInput(double inValues[]){
    for(int i = 0; i < numInput; i++){
        layers[0]->neurons[i]->output = inValues[i];
    }
}
    
double * Network::getOutput(){
    layers[1]->propagate();
    for(int i = 0; i < numOutput; i++){
        output[i] = layers.back()->neurons[i]->output;
    }
    return output;
}

void Network::calcError(double ideal[]){
    for(int i = numHiddenLayer+1; i >= 1; i--){
        layers[i]->error(ideal);
    }
    
}
double Network::getError(double ideal[]){
    double err = 0;
    for(int i = 0; i < numOutput; i++){
        err += abs(layers.back()->neurons[i]->output - ideal[i]);
    }
    return err;
}
int Network::getErrorInt(double ideal[]){
    double err = 0;
    for(int i = 0; i < numOutput; i++){
        err += abs(round(layers.back()->neurons[i]->output) - ideal[i]);
    }
    return err;
}
void Network::updateWeights(){
    for(int i = numHiddenLayer+1; i >= 1; i--){
        layers[i]->updateWeights();
    }
}


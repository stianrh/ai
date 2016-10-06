#ifndef __NN_H
#define __NN_H
#include <vector>
using namespace std;

class Neuron{
public:
    Neuron(int numInput, double l);
    int numInput;
    double lRate;
    double delta;
    double error;
    vector<double> weight;
    vector<double> new_weight;
    vector<Neuron*> inputs;
    vector<Neuron*> outputs;
    double output;
    void calcOutput();
    void calcDelta(double cd);
    void calcError();
    void print();
};

class Layer{
public:
    Layer(int numNeurons, int numInput, bool isOutputLayer, double lr);
    int numNeurons;
    vector<Neuron*> neurons;
    bool isOutputLayer;
    double lRate;
    Layer * parentLayer;
    Layer * childLayer;
    void connect(Layer * l);
    void propagate();
    void error(double ideal[]);
    void updateWeights();

};

class Network{
public:
    Network(int numInput, int numOutput, int numHiddenLayer, int numNeuron, double lr);
    int numInput;
    int numOutput;
    int numHiddenLayer;
    int numNeuron;
    double lRate;
    double *output;
    double error;
    vector<Layer*> layers;
    void setInput(double inValues[]);
    double* getOutput();
    double cost(double y);
    void calcError(double ideal[]);
    double getError(double ideal[]);
    int getErrorInt(double ideal[]);
    void updateWeights();
};

#endif //__NN_H


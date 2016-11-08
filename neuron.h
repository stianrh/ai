#ifndef __NEURON_H
#define __NEURON_H
#include <vector>
using namespace std;

class Layer;

class Neuron{
public:
    Neuron(int numInput, Layer * l);
    int numInput;
    Layer * parent_l;
    double delta;
    double error;
    double cd;
    double * lRate;
    vector<double> weight;
    vector<double> new_weight;
    vector<Neuron*> inputs;
    vector<Neuron*> outputs;
    double output;
    double netInput;
    void calcOutput();
    void calcDelta(double cd);
    void calcError();
    void print();
};



#endif //__NEURON_H


#ifndef __LAYER_H
#define __LAYER_H
#include <vector>
#include "neuron.h"
using namespace std;

class Network;

class Layer{
public:
    Layer(int numNeurons, int numInput, bool isOutputLayer, Network * netw);
    int numNeurons;
    bool isOutputLayer;
    Network * parent_nw;
    vector<Neuron*> neurons;
    Layer * parentLayer;
    Layer * childLayer;
    void connect(Layer * l);
    void propagate();
    void error(double ideal[]);
    void updateWeights();
};

#endif //__LAYER_H

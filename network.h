#ifndef __NETWORK_H
#define __NETWORK_H
#include <vector>
#include "layer.h"
using namespace std;

class Network{
public:
    Network(int numInput, int numOutput, int numHiddenLayer, int numNeuron, double lr);
    int numInput;
    int numOutput;
    int numHiddenLayer;
    int numNeuron;
    int numRun;
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
    void print(int s, double ideal[], int t_size, int t_num);
};

#endif //__NETWORK_H


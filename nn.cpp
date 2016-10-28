#include <iostream>
#include <curses.h>
#include "nn.h"
#include "utils.h"
#include <cmath>
using namespace std;


Neuron::Neuron(int n, double l): numInput(n), lRate(l){
    for(int i = 0; i < numInput + 1; i++){

        weight.push_back(RandomClamped());
    }
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
    //cout << "OUT: " << output << endl;
}

void Neuron::calcDelta(double cd_in){
    delta = cd_in*(output*(1-output));
    cd = cd_in;
}
void Neuron::calcError(){
    new_weight.clear();
    for(int i = 0; i < numInput; i++){
        new_weight.push_back(weight[i]-lRate*delta*inputs[i]->output);
    }
    new_weight.push_back(weight.back()-lRate*delta);
}



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
    numRun = 0;
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
        err += 0.5 * pow(ideal[i] - layers.back()->neurons[i]->output, 2);
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
void Network::print(int s, double ideal[], int t_size, int t_num){
    char input[ 100 ];

    if (s == 0){
        initscr();
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
        (void)echo();
        error = 0;
    }else if(s == 1){
        //clear();
        //addstr( "Network:" );

        //refresh();
        int row = t_size+2;

        //if(err){
            for(int l = 1; l < numHiddenLayer+2; l++){
                int n_neurons = layers[l]->numNeurons;
                int n_input= layers[l]->neurons[0]->numInput;
                for(int n = 0; n < n_neurons; n++){
                    for(int i = 0; i < n_input; i++){
                        move(row+i,n*15);
                        printw("          ");
                        move(row+i,n*15);
                        printw("%2.3f",layers[l]->neurons[n]->weight[i]);
                    }
                    move(row +n_input,n*15);
                    printw("N:(%d,%d)",l,n); 

                    move(row +n_input+1,n*15);
                    printw("          ");
                    move(row +n_input+1,n*15);
                    printw("d:%2.3f", abs(layers[l]->neurons[n]->cd));

                    //move(row +n_input+2+dist+t_num,n*10);
                    //printw("          ");
                    //move(row +n_input+2+dist+t_num,n*10);
                    //printw("c:%2.7f", abs(layers[l]->neurons[n]->cd));

                    //move(row +n_input+3+dist+t_num,n*10);
                    //printw("          ");
                    //move(row +n_input+3+dist+t_num,n*10);
                    //printw("o:%2.7f", abs(layers[l]->neurons[n]->output));
                           
                }
                row+=n_input+3;
            }
        //}
        if(t_num == t_size-1){
            move(t_size,0);
            error += getError(ideal);
            printw("error: %2.9f",error);
            error = 0;
        }
        else{
            error += getError(ideal);
        }
        int t0 = 0;
        int t1 = 0;
        for(int i = 0; i < numOutput; i++){
            t0 += ideal[i]*pow(2,i);
            t1 += round(layers.back()->neurons[i]->output)*pow(2,i);
        }
        move(t_num,0);
        printw("                              ");
        move(t_num,0);
        std::string actual = int2bin(t1, numOutput);
        std::string ideal = int2bin(t0, numOutput);
        printw("ACTUAL: ");
        for (int s = 0; s < (int)actual.length(); s++){
            if(actual[s] == ideal[s]){
                attron(COLOR_PAIR(3));
                printw("%c",actual[s]);
            }else{
                attron(COLOR_PAIR(1));
                printw("%c",actual[s]);
            }
        }
        attron(COLOR_PAIR(3));
        printw(" IDEAL: %s", ideal.c_str());
        

 
        refresh();
        //getnstr( input, sizeof( input) - 1 );

    }else if(s == 2){
        //printw( "\n\n\nPress ENTER to quit." );
        getnstr( input, sizeof( input) - 1 );
        endwin();
    }

}
    //getnstr( users_name, sizeof( users_name ) - 1 );







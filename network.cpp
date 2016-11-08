#include <iostream>
#include <curses.h>
#include <cmath>
#include "network.h"
#include "layer.h"
#include "utils.h"
using namespace std;

Network::Network(int in, int out, int hid, int n, double lr):
    numInput(in), 
    numOutput(out), 
    numHiddenLayer(hid), 
    numNeuron(n),
    lRate(lr)
{
    Layer * l;
    l = new Layer(numInput, 1, 0, this);
    layers.push_back(l);
    for(int i = 0; i < numHiddenLayer; i++){
        if(i == 0){
            l = new Layer(numNeuron, numInput, 0, this);
        }else{
            l = new Layer(numNeuron, numNeuron, 0, this);
        }
        l->connect(layers.back());
        layers.push_back(l);
    }
    l = new Layer(numOutput, numNeuron, 1, this);
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
            printw("\nl rate: %2.9f",lRate);
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

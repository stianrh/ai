#include "network.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#define NUM_NEURONS 6
#define NUM_HID_LAYERS 1 
#define NUM_OUTPUT 7
#define NUM_INPUT 4
#define T_SIZE 16 
#define P_ERROR 0
#define L_RATE 0.45
//#define DEBUG
//void int2bin(int in, double out[], int l){
//
    //for(int i = 0; i < l; i++){
        //out[i] = (in >> i) & 0x01;
        ////cout << "int2bin " << out[i] << endl;
    //}
//
//}

void double2bin(double in, int out[], int l){
    for(int i = 0; i < l; i++){
        out[i] = (int)(in * i*10);
        cout << "double2bin " << out[i] << endl;
    }
}
int bin2int(double in[], int l){
    int temp = 0;
    for(int i = 0; i < l; i++){
        temp += round(in[i])*pow(2,i);
    }
    return temp;
}


class TrSet{
public:
    TrSet(int size) : size(size){
       in = new double*[size];//[NUM_INPUT];
       out = new double*[size];//[NUM_INPUT];
       generate(); 

    };
    int size;
    double **in;
    double **out;
    void generate(){
        for(int i = 0; i < size; i++){
            out[i] = new double[NUM_OUTPUT];
            in[i] = new double[NUM_INPUT];
            double temp;
            
            if(i == 0) temp = 0b1000000;
            else if(i == 1) temp = 0b1111001;
            else if(i == 2) temp = 0b0100100;
            else if(i == 3) temp = 0b0110000;
            else if(i == 4) temp = 0b0011001;
            else if(i == 5) temp = 0b0010010;
            else if(i == 6) temp = 0b0000010;
            else if(i == 7) temp = 0b1111000;
            else if(i == 8) temp = 0b0000000;
            else if(i == 9) temp = 0b0010000;
            else if(i == 10) temp = 0b0001000;
            else if(i == 11) temp = 0b0000011;
            else if(i == 12) temp = 0b1000110;
            else if(i == 13) temp = 0b0100001;
            else if(i == 14) temp = 0b0000110;
            else if(i == 15) temp = 0b0001110;
            else temp = 0xFF;
            int2bin(temp, out[i], NUM_OUTPUT);
            int2bin(i, in[i], NUM_INPUT);
        }
    }
    void print(){
        for(int i = 0; i < size; i++){
            cout << "Input: ";
            for(int j = 0; j < NUM_INPUT; j++){
               cout << in[i][j];
            }
            cout << ", output: ";
            for(int j = 0; j < NUM_OUTPUT; j++){
               cout << out[i][j];
            }
            cout << endl;
        }
    }
};
int main(){
    double lRate = L_RATE;
    srand (time(NULL));
    Network n(NUM_INPUT,NUM_OUTPUT,NUM_HID_LAYERS,NUM_NEURONS,lRate);
    TrSet * t_set  = new TrSet(T_SIZE);
    vector<double> err_l;
    t_set->print();
    
#ifndef DEBUG
    double dummy[1] = {0};
    n.print(0, dummy, 0, T_SIZE);
#endif
    //for(int j = 0; j < 2000000; j++){
    double error;
    double prev_error;
    double d_error;
    bool first = true;
    while(true){
        error = 0;
        for(int i = 0; i < t_set->size; i++){
            n.setInput(t_set->in[i]);
            n.getOutput();
            n.calcError(t_set->out[i]);
            n.updateWeights();
#ifndef DEBUG
            n.print(1,t_set->out[i], T_SIZE, i);
            n.print(2,dummy, 0, T_SIZE);
#endif
            error += n.getError(t_set->out[i]);
        }
        //cout << error << endl;
        if(error < 0.1){
            break;
        }
        for(int i = 0; i < T_SIZE; i++){
            //n.setInput(t_set->in[i]);
            //n.getOutput();
        }
        if(first){
            prev_error = error;
            first = false;
        }
        d_error = abs(error - prev_error);
        prev_error = error;
        n.lRate = lRate -d_error/10 + (1-L_RATE);
        
    }
#ifndef DEBUG
    n.print(2,dummy, 0, T_SIZE);
#endif
    for(int i = 0; i < T_SIZE; i++){
        n.setInput(t_set->in[i]);
        cout << "TEST OUTPUT: " << bin2int(n.getOutput(),NUM_OUTPUT) << " " << bin2int(t_set->out[i], NUM_OUTPUT) << endl;
    }
}



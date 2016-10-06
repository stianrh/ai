#include <iostream>
#include <algorithm>
#include <bitset>
#include <time.h>
#include <cstdlib>
#include <cmath>
using namespace std;


class Genome{
public:
    long gene;
    char num[5];
    char op[4];
    float fitness;
    bool valid;

    Genome(){
        gene = 0x000000000;
    };
    Genome(long gene) : gene(gene){
        if(check_valid()){
            set_fitness();
            valid = true;
        }else{
            fitness = 0;
            valid = false;
        }
    };
    bool set_gene(long g){
        gene = g;
        if(check_valid()){
            set_fitness();
            valid = true;
        }else{
            fitness = 0;
            valid = false;
        }
        return valid;
    }


    bool check_valid(){
        num[4] = gene & 0x0F;
        num[3] = (gene >> 8) & 0x0F;
        num[2] = (gene >> 16) & 0x0F;
        num[1] = (gene >> 24) & 0x0F;
        num[0] = (gene >> 32) & 0x0F;
        op[3] = (gene >> 4) & 0x0F;
        op[2] = (gene >> 12) & 0x0F;
        op[1] = (gene >> 20) & 0x0F;
        op[0] = (gene >> 28) & 0x0F;
        
        for(int i = 0; i < 5; i++){
            if(!(num[i] >= 0 && num[i] <= 9)){
                return false;
            }
        }
        for(int i = 0; i < 4; i++){
            if(!(op[i] >= 10 && op[i] <= 13)){
                return false;
            }
        }
        return true;
    };
    float obj_func(){
        float a = num[0];
        for(int i = 0; i < 4;i++){
            switch(op[i]){ 
                case 10:
                    a += num[i+1];
                    break;
                case 11:
                    a -= num[i+1];
                    break;
                case 12:
                    a *= num[i+1];
                    break;
                case 13:
                    a /= (float)num[i+1];
                    break;
                default:
                    cout << "error";
                    return -1;
            }
        }
        return a;
    }
    void set_fitness(){
        fitness = abs(1/(127-obj_func()));
    }
    void print_fitness(){
        cout << fitness << endl;
    }
    void mutate(){
        int r1 = rand() % 36;
        long mask = (1 << r1);
        gene = gene ^ mask;
        if(check_valid()){
            set_fitness();
            valid = true;
        }else{
            fitness = 0;
            valid = false;
        }
    }
    long pair(Genome g){
        int r1 = rand() % 2;
        int r2 = (rand() % 9)*4;
        long mask = 0;
        for(long i = r2; i < 36; i ++){
            long temp = ((long)1 << i);
            
            mask = mask | temp;
        }
        if(r1){
            mask = ~mask;
        }
        long g1 = gene & mask;
        long g2 = g.gene & ~mask;
        //cout << std::bitset<36>(g1 | g2) << endl; 
        return (g1 | g2);
    }
};

bool sort_genome(Genome * a, Genome * b){
    return (a->fitness > b->fitness);
}
void generate_rand(Genome * g_arr[], int num){
    for(int j = 0; j < num; j++){
        long r1 = (long)rand()&0xFFFF;
        long r2 = (long)rand()&0xFFFF;
        long r3 = (long)rand()&0x000F;
        long r4 = r1 | (r2 << 16) | (r3 << 32);

        g_arr[j] = new Genome(r4);
    }
}

void reproduce(Genome * g1, Genome * g2, Genome * res, int mut){
    for(int i = 0; i < mut; i++){
        g1->mutate();
        g2->mutate();
    }
    res->set_gene(g1->pair(*g2));
}
void generation(Genome * g_arr[], Genome * ret[], int num){
    Genome * res_arr[num*(num/2)];
    for(int i = 0; i < num; i+=2){
        Genome * g;
        Genome * temp_arr[num];
        for(int j = 0; j < num; j++){
            int r1 = rand()%num;
            int r2 = r1;
            while(r2 == r1){
                r2 = rand()%num;
            }
            g = new Genome();
            reproduce(g_arr[r1],g_arr[r2],g,0);
            temp_arr[j] = g;
        }
        for(int j = 0; j < num; j++){
            //cout << "index: " << ((i/2)*num)+j << endl;
            res_arr[((i/2)*num)+j]=temp_arr[j];
            //cout << temp_arr[j]->fitness << endl;
        }
    }
    sort(res_arr, res_arr+(num*(num/2)), sort_genome);
    //cout << "keeping" << endl;
    for(int j = 0; j<num; j++){
        ret[j]=res_arr[j];
        //cout << ret[j]->fitness << endl;
    }
    //cout << "dicarded" << endl;
    for(int j = num; j < num*(num/2); j++){
        //cout << res_arr[j]->fitness << endl;
        delete res_arr[j];
    }
}


int main(){
    srand(time(NULL));
    int num = 100;
    Genome * init_arr[num];
    Genome * ret[num];
    generate_rand(init_arr,num);
    /*
    Genome * g1 = new Genome(0x1a1a1a1a1);
    Genome * g2 = new Genome(0x1a1a1a1a4);
    Genome * g3 = new Genome(0x1a1a1a1a2);
    Genome * g4 = new Genome(0x1a1a1a1a6);
    init_arr[0] = g1;
    init_arr[1] = g2;
    init_arr[2] = g3;
    init_arr[3] = g4;
    */
    for(int j = 0;j < 10000; j++){
        //cout << "new gen: " << j << endl;
        generation(init_arr,ret,num);
        for(int i = 0;i < num; i++){
            delete init_arr[i];
            init_arr[i] = ret[i];
            if(ret[i]->fitness > 1000000000000000){
                cout << std::bitset<36>(ret[i]->gene) << endl; 
                cout << (int)ret[i]->num[0] << endl;
                cout << (int)ret[i]->op[0] << endl;
                cout << (int)ret[i]->num[1] << endl;
                cout << (int)ret[i]->op[1] << endl;
                cout << (int)ret[i]->num[2] << endl;
                cout << (int)ret[i]->op[2] << endl;
                cout << (int)ret[i]->num[3] << endl;
                cout << (int)ret[i]->op[3] << endl;
                cout << (int)ret[i]->num[4] << endl;
                cout << "generation: " << (int)j << endl;
                cout << "obj func: " << ret[i]->obj_func() << endl;
                return 1;
            }
        }
    }
    cout << "best" << endl;
    for(int i = 0;i < num; i++){
        ret[i]->print_fitness();
    }
    
}

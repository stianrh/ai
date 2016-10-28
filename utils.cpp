#include "utils.h"
#include <math.h>
#include <string>



//--------------------------itos------------------------------------
//	converts an integer to a string
//------------------------------------------------------------------		
string itos(int arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}


//--------------------------ftos------------------------------------
//	converts a float to a string
//------------------------------------------------------------------		
string ftos(float arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}
//-------------------------------------Clamp()-----------------------------------------
//
//	clamps the first argument between the second two
//
//-------------------------------------------------------------------------------------
void Clamp(double &arg, double min, double max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
}

std::string int2bin(int in, int l){
    std::string out;
    for(int i = 0; i < l; i++){
        out.insert(0,1,((in >> i) & 0x01) ? '1' : '0');
    }
    return out;
}

void int2bin(int in, double out[], int l){
    for(int i = 0; i < l; i++){
        out[i] = (in >> i) & 0x01;
    }
}

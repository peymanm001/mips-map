#include "header.h"
/// fix it

struct registerFile_output *RegisterFile(struct registerFile_input* in)
{
    int i;
    long Address1,Address2,Address3;
    struct registerFile_output *out;
    out = (struct registerFile_output *) malloc(sizeof(struct registerFile_output));
    Address1 = binTodec(in -> readReg1, 5, 0);
    Address2 = binTodec(in -> readReg2, 5, 0);
    if(in -> regWrite == 1)
    {
        Address3 = binTodec(in -> writeReg, 5, 0);
        if (!(Address3 == 0)){
            copyArr(in->writeData, registers[Address3], 32);
        }
    }
    copyArr(registers[Address1], out->readData1, 32);
    copyArr(registers[Address2], out->readData2, 32);

    return out;
}


int *Extends(int *in){
    int i,j;
    int *out;
    out = (int *)malloc(sizeof(int)*32);
    if(in[0]==0)
    {
        for(i=0;i<16;i++)
        {
            out[i]=0;
        }
        for(j=0;i<32;i++,j++)
        {
            out[i] = in[j];
        }
        return out;
    }
    else if(in[0]==1)
    {
        for(i=0;i<16;i++)
        {
            out[i]=1;
        }
        for(i,j=0;i<32;i++,j++)
        {
            out[i]=in[j];
        }
        return out;
    }
    else{
        for(i=0;i<32;i++){
            out[i] = 0;
        }
        return out;
    }
}


void regFile_tf(struct regFile_t* value){
    value->out = RegisterFile(value->in);
}

void extend_tf(struct extend_t* value){
    value->out = Extends(value->in);
}

#include "header.h"

void IF_ID(struct IF_ID_input* input, struct IF_ID_output* output){
    int i;
    /// reset
    if (input->flush == 1){
        copyArr(decTobin(0,32),output->pc, 32);
        copyArr(decTobin(0,5), output->readReg1, 5);
        copyArr(decTobin(0,5), output->Rs, 5);
        copyArr(decTobin(0,5), output->readReg2, 5);
        copyArr(decTobin(0,5), output->Rt, 5);
        copyArr(decTobin(0,5), output->regDst1, 5);
        copyArr(decTobin(0,5), output->regDst2, 5);
        copyArr(decTobin(0,6), output->controlUnit, 6);
        copyArr(decTobin(0,16), output->address, 16);
        return ;
    }
    /// if write enable
    if ( input->WE == 0){
        return;
    }

    copyArr(input->pc, output->pc, 32);
    copyArr(input->instruction + 6, output->readReg1, 5);
    copyArr(input->instruction + 6, output->Rs, 5);
    copyArr(input->instruction + 11, output->readReg2, 5);
    copyArr(input->instruction + 11, output->Rt, 5);
    copyArr(input->instruction + 11, output->regDst1, 5);
    copyArr(input->instruction + 16, output->regDst2, 5);
    copyArr(input->instruction, output->controlUnit, 6);
    copyArr(input->instruction + 16, output->address, 16);
    return;
}


void IF_ID_tf(struct IF_ID_t* value){
    IF_ID(value->in, value->out);
}

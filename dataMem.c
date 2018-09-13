#include "header.h"

struct dataMemory_output* dataMemory(struct dataMemory_input* input){
    struct dataMemory_output* output = (struct dataMemory_output*) malloc(sizeof(struct dataMemory_output));
    long address = binTodec(input->address,32,0);
    int i;
    if (address > MEMORY_SIZE - 1){
        output->error = 1;      /// memory out of range
        return output;
    }
    if (input->memRead == 1){
        for(i=0; i<32; i++){
            output->dataRead[i] = DataMemory[address][i];
        }
        output->error = 0;
        return output;
    }
    if(input->memWrite == 1){
        for(i=0; i<32; i++){
            DataMemory[address][i] = input->writeData[i];
        }
        output->error = 0;
        return output;
    }
    return output;
}


void dataMem_tf(struct dataMem_t* value){
    value->out = dataMemory(value->in);
}

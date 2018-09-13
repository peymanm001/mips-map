#include "header.h"

struct instructionMemory_output* instructionMemory(struct instructionMemory_input* input){
    struct instructionMemory_output* output = (struct instructionMemory_output*)malloc(sizeof(struct instructionMemory_output));
    int i;
    long address = binTodec(input->readAddress, 32, 0);
    /// memory out of range
    if ( address > MEMORY_SIZE - 1 ){
        output->error = 1;
        return output;
    }
    copyArr(InstructionMemory[address], output->instruction, 32);

    output->error = 0;
    return output;
}

struct pcAdder_output* pcAdder(struct pcAdder_input* input){
    struct pcAdder_output* output = (struct pcAdder_output*)malloc(sizeof(struct pcAdder_output));
    int adderNum[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0};
    big_add(input->pc, adderNum, 32, output->pc);
    return output;
}


void insMem_tf(struct insMem_t* value){
    value->out = instructionMemory(value->in);
}

void pcAdder_tf(struct pcAdder_t* value){
    value->out = pcAdder(value->in);
}

#include "header.h"

void ID_EX(struct ID_EX_input* input, struct ID_EX_output* output){
    int i;
    if (input->flush == 1){
        copyArr(decTobin(0,2), output->WB, 2);
        copyArr(decTobin(0,2), output->aluOp, 2);

        output->regDst = 0;
        output->aluSrc = 0;

        copyArr(decTobin(0,3), output->M, 3);

        copyArr(decTobin(0,32), output->pc, 32);
        copyArr(decTobin(0,32), output->address, 32);
        copyArr(decTobin(0,32), output->readReg1, 32);
        copyArr(decTobin(0,32), output->readReg2, 32);

        copyArr(decTobin(0,5), output->regDst1, 5);
        copyArr(decTobin(0,5), output->regDst2, 5);
        copyArr(decTobin(0,5), output->Rs, 5);
        copyArr(decTobin(0,5), output->Rt, 5);

        copyArr(decTobin(0,6), output->func, 6);

        return;
    }

    copyArr(input->WB, output->WB, 2);
    copyArr(input->EX + 1, output->aluOp, 2);

    output->regDst = input->EX[0];
    output->aluSrc = input->EX[3];

    copyArr(input->M, output->M, 3);

    copyArr(input->pc, output->pc, 32);
    copyArr(input->address, output->address, 32);
    copyArr(input->readData1, output->readReg1, 32);
    copyArr(input->readData2, output->readReg2, 32);

    copyArr(input->regDst1, output->regDst1, 5);
    copyArr(input->regDst2, output->regDst2, 5);
    copyArr(input->Rs, output->Rs, 5);
    copyArr(input->Rt, output->Rt, 5);

    copyArr(input->address + 26, output->func, 6);

    return;
}


void ID_EX_tf(struct ID_EX_t* value){
    ID_EX(value->in, value->out);
}

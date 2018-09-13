#include "header.h"

void MEM_WB(struct MEM_WB_input * in, struct MEM_WB_output* out){
    int i;
    if ( in->flush == 1){
        copyArr(decTobin(0,32), out->alu_result, 32);
        copyArr(decTobin(0,32), out->readData, 32);
        copyArr(decTobin(0,5), out->muxout, 5);
        out -> regWrite = 0;
        out -> memToReg = 0;
        return;
    }
    copyArr(in->alu_result, out->alu_result, 32);
    copyArr(in->readdata, out->readData, 32);
    copyArr(in->muxout, out->muxout, 5);
    out -> regWrite = in -> wb[0];
    out -> memToReg = in -> wb[1];
    return;
}

void MEM_WB_tf(struct MEM_WB_t* value){
    MEM_WB(value->in, value->out);
}

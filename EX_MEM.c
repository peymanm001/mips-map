#include "header.h"

void EX_MEM(struct EX_MEM_input * in, struct EX_MEM_output* out){
    int i;
    if ( in->flush == 1){
        copyArr(decTobin(0,2), out->wb, 2);
        copyArr(decTobin(0,5), out->muxout, 5);

        copyArr(decTobin(0,32), out->alu_result, 32);
        copyArr(decTobin(0,32), out->branch_add, 32);
        copyArr(decTobin(0,32), out->read_data2, 32);

        out -> alu_zero = 0;
        out -> branch = 0;
        out -> memread = 0;
        out -> memwrite = 0;
        return;
    }
    copyArr(in->wb, out->wb, 2);
    copyArr(in->muxout, out->muxout, 5);

    copyArr(in->alu_result, out->alu_result, 32);
    copyArr(in->branch_add, out->branch_add, 32);
    copyArr(in->read_data2, out->read_data2, 32);

    out -> alu_zero = in -> alu_zero;
    out -> branch = in -> mem[0];
    out -> memread = in -> mem[1];
    out -> memwrite = in -> mem[2];
    return;
}

void EX_MEM_tf(struct EX_MEM_t* value){
    EX_MEM(value->in, value->out);
}

#include "header.h"

struct CU_output * CU(struct CU_input * in){
    struct CU_output *out = (struct CU_output *) malloc(sizeof(struct CU_output));
    int opcode[4][6] = {{0,0,0,1,1,1}, {0,0,1,1,1,1}, {0,1,0,1,1,1}, {0,0,0,0,1,1}};
    if(in -> opcode[0] == 1){ // R-Type
        // execute
        out -> ex[0] = 1;
        out -> ex[1] = 1;
        out -> ex[2] = 0;
        out -> ex[3] = 0;

        // memory
        out -> M[0] = 0;
        out -> M[1] = 0;
        out -> M[2] = 0;

        // wb
        out -> WB[0] = 1;
        out -> WB[1] = 0;
    }
    else if(mystrcmp(in -> opcode,opcode[0], 6, 6) == 0){ // lw
        // execute
        out -> ex[0] = 0;
        out -> ex[1] = 0;
        out -> ex[2] = 0;
        out -> ex[3] = 1;

        // memory
        out -> M[0] = 0;
        out -> M[1] = 1;
        out -> M[2] = 0;

        // wb
        out -> WB[0] = 1;
        out -> WB[1] = 1;
    }
    else if(mystrcmp(in -> opcode,opcode[1], 6, 6) == 0){ // sw
        // execute
        out -> ex[0] = 0;
        out -> ex[1] = 0;
        out -> ex[2] = 0;
        out -> ex[3] = 1;

        // memory
        out -> M[0] = 0;
        out -> M[1] = 0;
        out -> M[2] = 1;

        // wb
        out -> WB[0] = 0;
        out -> WB[1] = 1;
    }
    else if(mystrcmp(in -> opcode,opcode[2], 6, 6) == 0){ // branch
        // execute
        out -> ex[0] = 0;
        out -> ex[1] = 0;
        out -> ex[2] = 1;
        out -> ex[3] = 0;

        // memory
        out -> M[0] = 1;
        out -> M[1] = 0;
        out -> M[2] = 0;

        // wb
        out -> WB[0] = 0;
        out -> WB[1] = 0;
    }
    else if(mystrcmp(in->opcode, opcode[3], 6, 6) == 0){ // addi
        // execute
        out -> ex[0] = 0;
        out -> ex[1] = 0;
        out -> ex[2] = 0;
        out -> ex[3] = 1;

        // memory
        out -> M[0] = 0;
        out -> M[1] = 0;
        out -> M[2] = 0;
        // wb
        out -> WB[0] = 1;
        out -> WB[1] = 0;
    }
    else{
        copyArr(decTobin(0, 4), out->ex, 4);
        copyArr(decTobin(0, 3), out->M, 3);
        copyArr(decTobin(0, 2), out->WB, 2);
    }
    return out;
}


struct aluControl_output* aluControl(struct aluControl_input* input){
    struct aluControl_output* output = (struct aluControl_output*) malloc(sizeof(struct aluControl_output));
    output->aluControl[0] = 0;
    output->aluControl[1] = 0;
    output->aluControl[2] = 0;
    output->aluControl[3] = 0;
    int functions[5][6] = {{1,0,0,0,0,0},{1,0,0,0,1,0},{1,0,0,1,0,0},{1,0,0,1,0,1},{1,0,1,0,1,0}};
    if (input->aluOp[0] == 0){
        if (input->aluOp[1] == 0){
            output->aluControl[1] = 0;
            output->aluControl[2] = 1;
            output->aluControl[3] = 0;
        }
        else if(input->aluOp[1] == 1){
            output->aluControl[1] = 1;
            output->aluControl[2] = 1;
            output->aluControl[3] = 0;
        }
    } else if( input->aluOp[0] == 1){
        if(input->aluOp[1] == 0){
            if (!mystrcmp(functions[0], input->func, 6, 6) ){       // add
                output->aluControl[1] = 0;
                output->aluControl[2] = 1;
                output->aluControl[3] = 0;
            } else if(!mystrcmp(functions[1], input->func, 6, 6)){  // sub
                output->aluControl[1] = 1;
                output->aluControl[2] = 1;
                output->aluControl[3] = 0;
            } else if(!mystrcmp(functions[2], input->func, 6, 6)){  // and
                output->aluControl[1] = 0;
                output->aluControl[2] = 0;
                output->aluControl[3] = 0;
            } else if(!mystrcmp(functions[3], input->func, 6, 6)){  // or
                output->aluControl[1] = 0;
                output->aluControl[2] = 0;
                output->aluControl[3] = 1;
            } else if(!mystrcmp(functions[4], input->func, 6, 6)){  // slt
                output->aluControl[1] = 1;
                output->aluControl[2] = 1;
                output->aluControl[3] = 1;
            } else{
                return NULL;
            }
        }
    }
    return output;
}

struct ForwardingUnit_output * FU(struct ForwardingUnit_input * in){
    int ans1, ans2;
    int zero[5] = {0,0,0,0,0};
    struct ForwardingUnit_output *out = (struct ForwardingUnit_output *) malloc(sizeof(struct ForwardingUnit_output));
    ans1 = (in->EX_MEM_WB[0] == 1) && mystrcmp(in->EX_MEM_RegDst, zero, 5, 5) && !mystrcmp(in->EX_MEM_RegDst, in->Rs, 5, 5);
    ans2 = (in->EX_MEM_WB[0] == 1) && mystrcmp(in->EX_MEM_RegDst, zero, 5, 5) && !mystrcmp(in->EX_MEM_RegDst, in->Rt, 5, 5);
    if (ans1){
        out->aluIn1[0] = 1;
        out->aluIn1[1] = 0;
    }
    else if ((in->MEM_WB_WB == 1) && mystrcmp(in->MEM_WB_RegDst, zero, 5, 5) && !mystrcmp(in->MEM_WB_RegDst, in->Rs, 5, 5) && !ans1){
        out->aluIn1[0] = 0;
        out->aluIn1[1] = 1;
    }
    else{
        out->aluIn1[0] = 0;
        out->aluIn1[1] = 0;
    }

    if (ans2){
        out->aluIn2[0] = 1;
        out->aluIn2[1] = 0;
    }
    else if ((in->MEM_WB_WB == 1) && mystrcmp(in->MEM_WB_RegDst, zero, 5, 5) && !mystrcmp(in->MEM_WB_RegDst, in->Rt, 5, 5) && !ans2){
        out->aluIn2[0] = 0;
        out->aluIn2[1] = 1;
    }
    else{
        out->aluIn2[0] = 0;
        out->aluIn2[1] = 0;
    }
    return out;
}

struct HazardDet_output* HazardDet(struct HazardDet_input* in){
    struct HazardDet_output* out = (struct HazardDet_output*)malloc(sizeof(struct HazardDet_output));
    if(in->ID_EX_memRead == 1)
    {
        if(!mystrcmp(in->ID_EX_Rt,in->IF_ID_Rs,5,5) || !mystrcmp(in->ID_EX_Rt,in->IF_ID_Rt,5,5))
        {
            out->CUmux = 1;
            out->IF_ID_write = out->pcWrite = 0;
        }
        else{
            out->CUmux = 0;
            out->IF_ID_write = out->pcWrite = 1;
        }
    }
    else{
        out->CUmux = 0;
        out->IF_ID_write = out->pcWrite = 1;
    }
    return out;
}


void CU_tf(struct CU_t* value){
    value->out = CU(value->in);
}

void aluControl_tf(struct aluControl_t* value){
    value->out = aluControl(value->in);
}

void FU_tf(struct FU_t* value){
    value->out = FU(value->in);
}

void HazardDet_tf(struct HazardDet_t* value){
    value->out = HazardDet(value->in);
}

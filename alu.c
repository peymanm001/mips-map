#include "header.h"

int * two_complement(int num[],int len){
    int i;
    int num2[len];
    int temp[len];
    for(i = 0;i < len;i ++){
        temp[i] = !num[i];
    }
    for(i = 0;i < 32;i ++){
        num2[i] = 0;
    }
    num2[31] = 1;
    int *res = (int *) malloc(sizeof(int) * 32);
    big_add(temp,num2,32,res);
    return res;
}

void array_or(int num1[],int num2[],int len,int res[]){
    int i;
    for(i = 0; i < len;i ++){
        res[i] = num1[i] | num2[i];
    }
}

void array_and(int num1[],int num2[],int len,int res[]){
    int i;
    for(i = 0; i < len;i ++){
        res[i] = num1[i] & num2[i];
    }
}
struct alu_output * alu(struct alu_input * in){
    struct alu_output * out = (struct alu_output *) malloc(sizeof(struct alu_output));
    long in1,in2;
    int i;
    int ctrl[5][4] = {{0,0,1,0},{0,1,1,0},{0,0,0,1},{0,0,0,0},{0,1,1,1}};
    if(mystrcmp(in -> aluControl,ctrl[0],4,4) == 0){
        //printf("add\n");
        big_add(in -> input1,in -> input2,32,out -> result);
    }
    else if(mystrcmp(in -> aluControl,ctrl[1],4,4) == 0){
        //printf("sub\n");
        int *temp;
        temp = two_complement(in -> input2,32);
        big_add(in -> input1,temp,32,out -> result);
        if(is_zero(out -> result,32)){
            out -> zero = 1;
        } else
            out -> zero = 0;
    }
    else if(mystrcmp(in -> aluControl,ctrl[2],4,4) == 0){
        //printf("or\n");
        array_or(in -> input1,in -> input2,32,out -> result);
    }
    else if(mystrcmp(in -> aluControl,ctrl[3],4,4) == 0){
        //printf("and\n");
        array_and(in -> input1,in -> input2,32,out -> result);
    }
    else if(mystrcmp(in -> aluControl,ctrl[4],4,4) == 0){
        //printf("slt\n");
        in1 = binTodec(in -> input1,32,1);
        in2 = binTodec(in -> input2,32,1);
        if(in1 < in2){
            for(i = 0;i < 32;i ++){
                out -> result[i] = 0;
            }
            out -> result[31] = 1;
        } else {
            for(i = 0;i < 32;i ++){
                out -> result[i] = 0;
            }
        }
    }
    if ( out->zero != 0 || out->zero != 1){
        out->zero = 0;
    }
    return out;
}

int is_zero(int *num,int len){
    int i;
    for(i = 0;i < len;i ++){
        if(num[i] != 0){
            return 0;
        }
    }
    return 1;
}

int* shiftLeft(int *num,int len,int amount){
    int i,j;
    int *temp = (int*) malloc(sizeof(int)*32);
    for(i = 0;i < len - amount;i ++){
        temp[i] = num[i + amount];
    }
    for(j = 0;j < amount;j ++){
        temp[i + j] = 0;
    }
    return temp;
}

struct branchAdder_output* branchAdder(struct branchAdder_input* input){
    struct branchAdder_output *output = (struct branchAdder_output*) malloc(sizeof(struct branchAdder_output));
    big_add(input->branchAddress , input->pc, 32,output->pc);
    return output;
}


void alu_tf(struct alu_t* value){
    value->out = alu(value->in);
}

void branchAdder_tf(struct branchAdder_t* value){
    value->out = branchAdder(value->in);
}

void shl_tf(struct shl_t* value){
    value->out = shiftLeft(value->in, 32, 2);
}

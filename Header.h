#include "Windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "tchar.h"
#include "direct.h"
#include "windef.h"
#include "structures.c"

void big_add(int num1[],int num2[],int len,int ans[]);
long binTodec(int *arr, int n, int sign);
int mystrcmp(int *s,int *t,int len1,int len2);
int *mux(int* data1, int* data2, int select);

void insMem_tf(struct insMem_t* value);
void pcAdder_tf(struct pcAdder_t* value);
void IF_ID_tf(struct IF_ID_t* value);
void regFile_tf(struct regFile_t* value);
void extend_tf(struct extend_t* value);
void ID_EX_tf(struct ID_EX_t* value);
void alu_tf(struct alu_t* value);
void branchAdder_tf(struct branchAdder_t* value);
void shl_tf(struct shl_t* value);
void EX_MEM_tf(struct EX_MEM_t* value);
void dataMem_tf(struct dataMem_t* value);
void MEM_WB_tf(struct MEM_WB_t* value);
void CU_tf(struct CU_t* value);
void aluControl_tf(struct aluControl_t* value);
void FU_tf(struct FU_t* value);
void HazardDet_tf(struct HazardDet_t* value);
void assembler(char infile[],char outfile[]);


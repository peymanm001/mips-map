#define MEMORY_SIZE 1024

int DataMemory[MEMORY_SIZE][8];
int InstructionMemory[MEMORY_SIZE][8];
int pc[32];
int registers[32][32];

struct registerFile_input{
    int readReg1[5];
    int readReg2[5];
    int writeReg[5];
    int writeData[32];
    int regWrite;
};

struct registerFile_output{
    int readData1[32];
    int readData2[32];
};
/// thread
struct regFile_t{
    struct registerFile_input* in;
    struct registerFile_output* out;
};

struct alu_input{
    int input1[32];
    int input2[32];
    int aluControl[4];
};

struct alu_output{
    int zero;
    int result[32];
};
/// thread
struct alu_t{
    struct alu_input* in;
    struct alu_output* out;
};

struct instructionMemory_input{
    int readAddress[32];
};

struct instructionMemory_output{
    int instruction[32];
    int error;
};
/// thread
struct insMem_t{
    struct instructionMemory_input* in;
    struct instructionMemory_output* out;
};

struct dataMemory_input{
    int address[32];
    int writeData[32];
    int memRead;
    int memWrite;
};

struct dataMemory_output{
    int dataRead[32];
    int error;
};
/// thread
struct dataMem_t{
    struct dataMemory_input* in;
    struct dataMemory_output* out;
};

struct branchAdder_input{
    int pc[32];
    int branchAddress[32];
};

struct branchAdder_output{
    int pc[32];
};
/// thread
struct branchAdder_t{
    struct branchAdder_input* in;
    struct branchAdder_output* out;
};

struct pcAdder_input{         /// constant 4 will be added in function
    int pc[32];
};

struct pcAdder_output{
    int pc[32];
};
/// thread
struct pcAdder_t{
    struct pcAdder_input* in;
    struct pcAdder_output* out;
};

struct aluControl_input{
    int aluOp[2];
    int func[6];
};

struct aluControl_output{
    int aluControl[4];
};
/// thread
struct aluControl_t{
    struct aluControl_input* in;
    struct aluControl_output* out;
};

struct IF_ID_input{
    int flush;
    int WE;
    int pc[32];
    int instruction[32];
};

struct IF_ID_output{
    int pc[32];
    int controlUnit[6];
    int readReg1[5];
    int readReg2[5];
    int address[16];
    int Rs[5];
    int Rt[5];
    int regDst1[5];
    int regDst2[5];
};
/// thread
struct IF_ID_t{
    struct IF_ID_input* in;
    struct IF_ID_output* out;
};

struct ID_EX_input{
    int flush;
    int WB[2];
    int M[3];
    int EX[4];
    int pc[32];
    int readData1[32];
    int readData2[32];
    int address[32];
    int Rs[5];
    int Rt[5];
    int regDst1[5];
    int regDst2[5];
};

struct ID_EX_output{
    int WB[2];
    int M[3];
    int regDst;
    int aluSrc;
    int aluOp[2];
    int func[6];
    int pc[32];
    int readReg1[32];
    int readReg2[32];
    int address[32];
    int Rs[5];
    int Rt[5];
    int regDst1[5];
    int regDst2[5];

};
/// thread
struct ID_EX_t{
    struct ID_EX_input* in;
    struct ID_EX_output* out;
};

struct EX_MEM_input{
    int flush;
    int wb[2];
    int mem[3];
    int branch_add[32];
    int alu_zero;
    int alu_result[32];
    int read_data2[32];
    int muxout[5];
};

struct EX_MEM_output{
    int wb[2];
    int memwrite;
    int memread;
    int branch;
    int branch_add[32];
    int alu_zero;
    int alu_result[32];
    int read_data2[32];
    int muxout[5];
};

/// thread
struct EX_MEM_t{
    struct EX_MEM_input* in;
    struct EX_MEM_output* out;
};

struct MEM_WB_input{
    int flush;
    int wb[2];
    int readdata[32];
    int alu_result[32];
    int muxout[5];
};

struct MEM_WB_output{
    int regWrite;
    int memToReg;
    int readData[32];
    int alu_result[32];
    int muxout[5];
};
/// thread
struct MEM_WB_t{
    struct MEM_WB_input* in;
    struct MEM_WB_output* out;
};

struct CU_input{
    int opcode[6];
};

struct CU_output{
    int ex[4];
    int M[3];
    int WB[2];
};
/// thread
struct CU_t{
    struct CU_input* in;
    struct CU_output* out;
};

struct ForwardingUnit_input{
    int Rs[5];
    int Rt[5];
    int EX_MEM_RegDst[5];
    int MEM_WB_RegDst[5];
    int EX_MEM_WB[2];
    int MEM_WB_WB;
};

struct ForwardingUnit_output{
    int aluIn1[2];
    int aluIn2[2];
};
/// thread
struct FU_t{
    struct ForwardingUnit_input* in;
    struct ForwardingUnit_output* out;
};

struct HazardDet_input{
    int IF_ID_Rs[5];
    int IF_ID_Rt[5];
    int ID_EX_Rt[5];
    int ID_EX_memRead;
};

struct HazardDet_output{
    int pcWrite;
    int IF_ID_write;
    int CUmux;
};
/// thread
struct HazardDet_t{
    struct HazardDet_input* in;
    struct HazardDet_output* out;
};

struct extend_t{
    int *in;
    int* out;
};

struct shl_t{
    int *in;
    int* out;
};

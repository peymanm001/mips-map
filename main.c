#include "resource.h"
#include "Header.h"
#define MAXFILENAME 1000

const char g_szClassName[] = "myWindowClass";
const int ID_TIMER = 1;

//Handel for creat window
HWND	 g_hToolbar_HELP = NULL;
HWND	 g_hToolbar_CRADIT = NULL;
HWND	 g_hToolbar_RUN_SIMULATION = NULL;
HWND	 g_hToolbar_VIRUS_LIST = NULL;
HWND	 g_hToolbar_VIRUS_LIST_Auto = NULL;
HWND	 g_hToolbar_EDIT = NULL;
HWND	 g_hToolbar_RUN_SIMULATION_STS = NULL;

const int BALL_MOVE_DELTA = 2;

typedef struct _BALLINFO
{
	int width;
	int height;
	int x;
	int y;

	int dx;
	int dy;
}BALLINFO;

BALLINFO g_ballInfo;
HBITMAP g_hbmBall = NULL;
HBITMAP g_hbmMask = NULL;

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
	HDC hdcMem, hdcMem2;
	HBITMAP hbmMask;
	BITMAP bm;

	GetObject(hbmColour, sizeof(BITMAP), &bm);
	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	hdcMem = CreateCompatibleDC(0);
	hdcMem2 = CreateCompatibleDC(0);

	SelectObject(hdcMem, hbmColour);
	SelectObject(hdcMem2, hbmMask);

	SetBkColor(hdcMem, crTransparent);

	BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

	DeleteDC(hdcMem);
	DeleteDC(hdcMem2);

	return hbmMask;
}

void DrawBall(HDC hdc, RECT* prc)
{
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, g_hbmMask);

	FillRect(hdcBuffer, prc, (HBRUSH)GetStockObject(WHITE_BRUSH));

	BitBlt(hdcBuffer, g_ballInfo.x, g_ballInfo.y, g_ballInfo.width, g_ballInfo.height, hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, g_hbmBall);
	BitBlt(hdcBuffer, g_ballInfo.x, g_ballInfo.y, g_ballInfo.width, g_ballInfo.height, hdcMem, 0, 0, SRCPAINT);

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

void UpdateBall(RECT* prc)
{
	g_ballInfo.x += g_ballInfo.dx;
	g_ballInfo.y += g_ballInfo.dy;

	if(g_ballInfo.x < 0)
	{
		g_ballInfo.x = 0;
		g_ballInfo.dx = BALL_MOVE_DELTA;
	}
	else if(g_ballInfo.x + g_ballInfo.width > prc->right)
	{
		g_ballInfo.x = prc->right - g_ballInfo.width;
		g_ballInfo.dx = -BALL_MOVE_DELTA;
	}

	if(g_ballInfo.y < 0)
	{
		g_ballInfo.y = 0;
		g_ballInfo.dy = BALL_MOVE_DELTA;
	}
	else if(g_ballInfo.y + g_ballInfo.height > prc->bottom)
	{
		g_ballInfo.y = prc->bottom - g_ballInfo.height;
		g_ballInfo.dy = -BALL_MOVE_DELTA;
	}
}

//*******************************************************************

BOOL CALLBACK ToolDlgProc_RUN_SIMULATION(HWND hwnd, UINT Message1, WPARAM wParam, LPARAM lParam)
{
	switch(Message1)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_CLOSE_SIMULATION:
					DestroyWindow(g_hToolbar_RUN_SIMULATION);
				break;

				case IDC_RUN:
				{
					SetDlgItemTextA(hwnd, IDC_PC,"Processing...");
					UpdateWindow(hwnd);
					//Start
					char filename[MAXFILENAME];
                    GetDlgItemTextA(hwnd,IDC_Name,filename,MAXFILENAME);
				    if(!strcmp(filename,""))
                    {
                        MessageBox(NULL, "Please enter the name", "Error!",MB_ICONEXCLAMATION | MB_OK);
                    }
                    else
                    {
                        /// stage 1
                        struct instructionMemory_input* insMem_in = (struct instructionMemory_input*) malloc(sizeof(struct instructionMemory_input));
                        struct instructionMemory_output* insMem_out;
                        struct insMem_t* insMem_tin = (struct insMem_t*) malloc(sizeof(struct insMem_t));
                        insMem_tin->in = insMem_in;
                        insMem_out = insMem_tin->out;


                        struct pcAdder_input* pcAdder_in = (struct pcAdder_input*) malloc(sizeof(struct pcAdder_input));
                        struct pcAdder_output* pcAdder_out;
                        struct pcAdder_t* pcAdder_tin = (struct pcAdder_t*) malloc(sizeof(struct pcAdder_t));
                        pcAdder_tin->in = pcAdder_in;
                        pcAdder_tin->out = pcAdder_out;

                        /// pipeline
                        struct IF_ID_input* IF_ID_in = (struct IF_ID_input*) malloc(sizeof(struct IF_ID_input));
                        struct IF_ID_output* IF_ID_out = (struct IF_ID_output*) malloc(sizeof(struct IF_ID_output));
                        struct IF_ID_t* IF_ID_tin = (struct IF_ID_t*) malloc(sizeof(struct IF_ID_t));
                        IF_ID_tin->in = IF_ID_in;
                        IF_ID_tin->out = IF_ID_out;

                        /// stage 2
                        struct registerFile_input* regFile_in = (struct registerFile_input*) malloc(sizeof(struct registerFile_input));
                        struct registerFile_output* regFile_out;
                        struct regFile_t* regFile_tin = (struct regFile_t*) malloc(sizeof(struct regFile_t));
                        regFile_tin->in = regFile_in;
                        regFile_tin->out = regFile_out;

                        int* extend_in = (int*) malloc(sizeof(int)*16);
                        int* extend_out;
                        struct extend_t* extend_tin = (struct extend_t*) malloc(sizeof(struct extend_t));
                        extend_tin->in = extend_in;
                        extend_tin->out = extend_out;

                        /// pipeline
                        struct ID_EX_input* ID_EX_in = (struct ID_EX_input*) malloc(sizeof(struct ID_EX_input));
                        struct ID_EX_output* ID_EX_out = (struct ID_EX_output*) malloc(sizeof(struct ID_EX_output));
                        struct ID_EX_t* ID_EX_tin = (struct ID_EX_t*) malloc(sizeof(struct ID_EX_t));
                        ID_EX_tin->in = ID_EX_in;
                        ID_EX_tin->out = ID_EX_out;

                        /// stage 3
                        struct alu_input* alu_in = (struct alu_input*) malloc(sizeof(struct alu_input));
                        struct alu_output* alu_out;
                        struct alu_t* alu_tin = (struct alu_t*) malloc(sizeof(struct alu_t));
                        alu_tin->in = alu_in;
                        alu_tin->out = alu_out;

                        struct branchAdder_input* branchAdder_in = (struct branchAdder_input*) malloc(sizeof(struct branchAdder_input));
                        struct branchAdder_output* branchAdder_out;
                        struct branchAdder_t* branchAdder_tin = (struct branchAdder_t*) malloc(sizeof(struct branchAdder_t));
                        branchAdder_tin -> in = branchAdder_in;
                        branchAdder_tin -> out = branchAdder_out;

                        int* shl_in = (int*) malloc(sizeof(int)*32);
                        int* shl_out;
                        struct shl_t* shl_tin = (struct shl_t*) malloc(sizeof(struct shl_t));
                        shl_tin->in = shl_in;
                        shl_tin->out = shl_out;

                        /// pipeline
                        struct EX_MEM_input* EX_MEM_in = (struct EX_MEM_input*) malloc(sizeof(struct EX_MEM_input));
                        struct EX_MEM_output* EX_MEM_out = (struct EX_MEM_output*) malloc(sizeof(struct EX_MEM_output));
                        struct EX_MEM_t* EX_MEM_tin = (struct EX_MEM_t*) malloc(sizeof(struct EX_MEM_t));
                        EX_MEM_tin -> in = EX_MEM_in;
                        EX_MEM_tin -> out = EX_MEM_out;

                        /// stage 4
                        struct dataMemory_input* dataMem_in = (struct dataMemory_input*) malloc(sizeof(struct dataMemory_input));
                        struct dataMemory_output* dataMem_out;
                        struct dataMem_t* dataMem_tin = (struct dataMem_t*) malloc(sizeof(struct dataMem_t));
                        dataMem_tin->in = dataMem_in;
                        dataMem_tin->out = dataMem_out;

                        /// pipeline
                        struct MEM_WB_input* MEM_WB_in = (struct MEM_WB_input*) malloc(sizeof(struct MEM_WB_input));
                        struct MEM_WB_output* MEM_WB_out = (struct MEM_WB_output*) malloc(sizeof(struct MEM_WB_output));
                        struct MEM_WB_t* MEM_WB_tin = (struct MEM_WB_t*) malloc(sizeof(struct MEM_WB_t));
                        MEM_WB_tin->in = MEM_WB_in;
                        MEM_WB_tin->out = MEM_WB_out;

                        /// Control
                        struct CU_input* cu_in = (struct CU_input*) malloc(sizeof(struct CU_input));
                        struct CU_output* cu_out;
                        struct CU_t* CU_tin = (struct CU_t*) malloc(sizeof(struct CU_t));
                        CU_tin->in = cu_in;
                        CU_tin->out = cu_out;

                        struct aluControl_input* aluC_in = (struct aluControl_input*) malloc(sizeof(struct aluControl_input));
                        struct aluControl_output* aluC_out;
                        struct aluControl_t* aluControl_tin = (struct aluControl_t*) malloc(sizeof(struct aluControl_t));
                        aluControl_tin->in = aluC_in;
                        aluControl_tin->out = aluC_out;

                        struct ForwardingUnit_input* FU_in = (struct ForwardingUnit_input*) malloc(sizeof(struct ForwardingUnit_input));
                        struct ForwardingUnit_output* FU_out;

                        struct HazardDet_input* HD_in = (struct HazardDet_input*) malloc(sizeof(struct HazardDet_input));
                        struct HazardDet_output* HD_out;

                        /// initial
                        copyArr(decTobin(0,32), pc, 32);
                        int stage[5][32];
                        int pc_select = 0;
                        int pcWrite = 1;
                        int lineNum = 0;
                        DWORD id;
                        int isEnd = 5;
                        int i;
                        int *make_zero;
                        make_zero = decTobin(0,32);
                        for (i=0;i<32;i++){
                            copyArr(make_zero, registers[i], 32);
                        }

                        for (i=0;i<MEMORY_SIZE;i++){
                            copyArr(make_zero, DataMemory[i], 32);
                            copyArr(make_zero, InstructionMemory[i], 32);
                        }
                        assembler(filename, "out.o");

                        HANDLE stageHandles[10];
                        HANDLE pipeHandles[5];

                        copyArr(decTobin(0,160), stage, 160);

                        IF_ID_in->flush = ID_EX_in->flush = EX_MEM_in->flush = MEM_WB_in->flush = 1;
                        IF_ID(IF_ID_in, IF_ID_out);
                        ID_EX(ID_EX_in, ID_EX_out);
                        EX_MEM(EX_MEM_in, EX_MEM_out);
                        MEM_WB(MEM_WB_in, MEM_WB_out);
                        IF_ID_in->flush = ID_EX_in->flush = EX_MEM_in->flush = MEM_WB_in->flush = 0;

                        long num = readIns("out.o", 0);
                        int clock = 0;
                        while ( isEnd > 0 ){
                            clock++;
                            /// pipeline
                            copyArr(IF_ID_out->Rs, ID_EX_in->Rs, 5);
                            copyArr(IF_ID_out->Rt, ID_EX_in->Rt, 5);
                            copyArr(IF_ID_out->pc, ID_EX_in->pc, 32);
                            copyArr(IF_ID_out->regDst1, ID_EX_in->regDst1, 5);
                            copyArr(IF_ID_out->regDst2, ID_EX_in->regDst2, 5);

                            copyArr(ID_EX_out->readReg2, EX_MEM_in->read_data2, 32);
                            copyArr(mux(ID_EX_out->regDst1, ID_EX_out->regDst2, ID_EX_out->regDst), EX_MEM_in->muxout, 5);
                            copyArr(ID_EX_out->WB, EX_MEM_in->wb, 2);
                            copyArr(ID_EX_out->M, EX_MEM_in->mem, 3);

                            copyArr(EX_MEM_out->alu_result, MEM_WB_in->alu_result, 32);
                            copyArr(EX_MEM_out->muxout, MEM_WB_in->muxout, 5);
                            copyArr(EX_MEM_out->wb, MEM_WB_in->wb, 2);

                            /// stage 1
                            copyArr(pc, insMem_in->readAddress, 32);
                            copyArr(pc, pcAdder_in->pc, 32);

                            /// stage 2
                            copyArr(IF_ID_out->Rs, HD_in->IF_ID_Rs, 5);
                            copyArr(IF_ID_out->Rt, HD_in->IF_ID_Rt, 5);
                            copyArr(ID_EX_out->Rt, HD_in->ID_EX_Rt, 5);
                            HD_in->ID_EX_memRead = ID_EX_out->M[1];
                            HD_out = HazardDet(HD_in);
                            pcWrite = HD_out->pcWrite;
                            IF_ID_in->WE = HD_out->IF_ID_write;

                            copyArr(IF_ID_out->controlUnit, cu_in->opcode, 6);
                            copyArr(IF_ID_out->readReg1, regFile_in->readReg1, 5);
                            copyArr(IF_ID_out->readReg2, regFile_in->readReg2, 5);
                            copyArr(IF_ID_out->address, extend_in, 16);

                            copyArr(IF_ID_out->pc, branchAdder_in->pc, 32);
                            copyArr(shiftLeft(Extends(IF_ID_out->address), 32, 2), branchAdder_in->branchAddress, 32);

                            /// stage 3
                            copyArr(ID_EX_out->aluOp, aluC_in->aluOp, 2);
                            copyArr(ID_EX_out->func, aluC_in->func, 6);
                            aluC_out = aluControl(aluC_in);
                            copyArr(ID_EX_out->Rs, FU_in->Rs, 5);
                            copyArr(ID_EX_out->Rt, FU_in->Rt, 5);
                            copyArr(EX_MEM_out->muxout, FU_in->EX_MEM_RegDst, 5);
                            copyArr(MEM_WB_out->muxout, FU_in->MEM_WB_RegDst, 5);
                            copyArr(EX_MEM_out->wb, FU_in->EX_MEM_WB, 2);
                            FU_in->MEM_WB_WB = MEM_WB_out->regWrite;
                            FU_out = FU(FU_in);

                            copyArr(aluC_out, alu_in->aluControl, 4);
                            copyArr(mux2(ID_EX_out->readReg1, mux(MEM_WB_out->alu_result, MEM_WB_out->readData, MEM_WB_out->memToReg), EX_MEM_out->alu_result, NULL, FU_out->aluIn1), alu_in->input1, 32);
                            copyArr(mux2(mux(ID_EX_out->readReg2, ID_EX_out->address, ID_EX_out->aluSrc), mux(MEM_WB_out->alu_result, MEM_WB_out->readData, MEM_WB_out->memToReg), EX_MEM_out->alu_result, NULL, FU_out->aluIn2), alu_in->input2, 32);

                            /// stage 4
                            copyArr(EX_MEM_out->alu_result, dataMem_in->address, 32);
                            copyArr(EX_MEM_out->read_data2, dataMem_in->writeData, 32);
                            dataMem_in->memRead = EX_MEM_out->memread;
                            dataMem_in->memWrite = EX_MEM_out->memwrite;

                            /// stage 5
                            copyArr(mux(MEM_WB_out->alu_result, MEM_WB_out->readData, MEM_WB_out->memToReg), regFile_in->writeData, 32);
                            copyArr(MEM_WB_out->muxout, regFile_in->writeReg, 5);
                            regFile_in->regWrite = MEM_WB_out->regWrite;

                            /// stage threads
                            stageHandles[0] = CreateThread(NULL, sizeof(struct insMem_t), insMem_tf, insMem_tin, 0, &id);
                            stageHandles[1] = CreateThread(NULL, sizeof(struct pcAdder_t), pcAdder_tf, pcAdder_tin, 0, &id);

                            stageHandles[2] = CreateThread(NULL, sizeof(struct CU_t), CU_tf, CU_tin, 0, &id);
                            stageHandles[3] = CreateThread(NULL, sizeof(struct regFile_t), regFile_tf, regFile_tin, 0, &id);
                            stageHandles[4] = CreateThread(NULL, sizeof(struct extend_t), extend_tf, extend_tin, 0, &id);
                            stageHandles[5] = CreateThread(NULL, sizeof(struct branchAdder_t), branchAdder_tf, branchAdder_tin, 0, &id);

                            stageHandles[6] = CreateThread(NULL, sizeof(struct alu_t), alu_tf, alu_tin, 0, &id);

                            stageHandles[7] = CreateThread(NULL, sizeof(struct dataMem_t), dataMem_tf, dataMem_tin, 0, &id);

                            WaitForMultipleObjects(8, stageHandles, TRUE, INFINITE);

                            /// assign outputs
                            insMem_out = insMem_tin->out;
                            pcAdder_out = pcAdder_tin->out;

                            cu_out = CU_tin->out;
                            regFile_out = regFile_tin->out;
                            extend_out = extend_tin->out;

                            branchAdder_out = branchAdder_tin->out;
                            alu_out = alu_tin->out;

                            dataMem_out = dataMem_tin->out;
                            ///

                            /// hazardDetection
                            pc_select = !mystrcmp(regFile_out->readData1, regFile_out->readData2, 32, 32) && cu_out->M[0];
                            if (pc_select){
                                IF_ID_in->flush = 1;
                                pcWrite = 1;
                                HD_out->CUmux = 1;
                            }
                            else{
                                IF_ID_in->flush = 0;
                            }

                            /// mux pc write
                            if (pcWrite == 1){
                                copyArr(mux(pcAdder_out->pc, branchAdder_out->pc , pc_select), pc, 32);
                            }

                            /// test
                            printf("line%d : ", lineNum);
                            prtArr(insMem_out->instruction, 32);

                            /// pipeline IF/ID
                            copyArr(insMem_out->instruction, IF_ID_in->instruction, 32);
                            copyArr(pcAdder_out->pc, IF_ID_in->pc,32);
                            IF_ID(IF_ID_in, IF_ID_out);

                            /// pipline ID/EX
                            copyArr(regFile_out->readData1, ID_EX_in->readData1, 32);
                            copyArr(regFile_out->readData2, ID_EX_in->readData2, 32);
                            copyArr(extend_out, ID_EX_in->address, 32);

                            if ( HD_out->CUmux == 1){
                                copyArr(decTobin(0, 4), ID_EX_in->EX, 4);
                                copyArr(decTobin(0, 3), ID_EX_in->M, 3);
                                copyArr(decTobin(0, 2), ID_EX_in->WB, 2);
                            }
                            else{
                                copyArr(cu_out->ex, ID_EX_in->EX, 4);
                                copyArr(cu_out->M, ID_EX_in->M, 3);
                                copyArr(cu_out->WB, ID_EX_in->WB, 2);
                            }
                            ID_EX(ID_EX_in, ID_EX_out);

                            /// pipeline EX/MEM
                            EX_MEM_in->alu_zero = alu_out->zero;
                            copyArr(alu_out->result, EX_MEM_in->alu_result, 32);
                            EX_MEM(EX_MEM_in, EX_MEM_out);

                            /// pipeline MEM/WB
                            copyArr(dataMem_out->dataRead, MEM_WB_in->readdata, 32);
                            MEM_WB(MEM_WB_in, MEM_WB_out);


                            /// is end ??
                            if ( !mystrcmp(insMem_out->instruction, decTobin(0,32), 32, 32) ){
                                isEnd--;
                            }
                            else if ( mystrcmp(insMem_out->instruction, decTobin(0,32), 32, 32) ){
                                isEnd = 5;
                            }


                            /// stage state
                            copyArr(stage[3], stage[4], 32);
                            copyArr(stage[2], stage[3], 32);
                            copyArr(stage[1], stage[2], 32);
                            copyArr(stage[0], stage[1], 32);
                            copyArr(insMem_out->instruction, stage[0], 32);

                            num--;
                            lineNum++;
                        }
                        printf("\nclock counter: %d\n",clock);
                        SetDlgItemTextA(hwnd, IDC_PC,"Done!");
                        int ans;
                        char arr[31];
                        for(i=0;i<32;i++)
                        {
                            ans=binTodec(registers[i],32,1);
                            itoa(ans,arr,10);
                            switch(i)
                            {
                            case 0:
                                SetDlgItemTextA(hwnd,IDC_PC2,arr);
                            break;
                            case 1:
                                SetDlgItemTextA(hwnd,IDC_PC3,arr);
                            break;
                            case 2:
                                SetDlgItemTextA(hwnd,IDC_PC4,arr);
                            break;
                            case 3:
                                SetDlgItemTextA(hwnd,IDC_PC5,arr);
                            break;
                            case 4:
                                SetDlgItemTextA(hwnd,IDC_PC6,arr);
                            break;
                            case 5:
                                SetDlgItemTextA(hwnd,IDC_PC7,arr);
                            break;
                            case 6:
                                SetDlgItemTextA(hwnd,IDC_PC8,arr);
                            break;
                            case 7:
                                SetDlgItemTextA(hwnd,IDC_PC9,arr);
                            break;
                            case 8:
                                SetDlgItemTextA(hwnd,IDC_PC10,arr);
                            break;
                            case 9:
                                SetDlgItemTextA(hwnd,IDC_PC11,arr);
                            break;
                            case 10:
                                SetDlgItemTextA(hwnd,IDC_PC12,arr);
                            break;
                            case 11:
                                SetDlgItemTextA(hwnd,IDC_PC13,arr);
                            break;
                            case 12:
                                SetDlgItemTextA(hwnd,IDC_PC14,arr);
                            break;
                            case 13:
                                SetDlgItemTextA(hwnd,IDC_PC15,arr);
                            break;
                            case 14:
                                SetDlgItemTextA(hwnd,IDC_PC16,arr);
                            break;
                            case 15:
                                SetDlgItemTextA(hwnd,IDC_PC17,arr);
                            break;
                            case 16:
                                SetDlgItemTextA(hwnd,IDC_PC18,arr);
                            break;
                            case 17:
                                SetDlgItemTextA(hwnd,IDC_PC19,arr);
                            break;
                            case 18:
                                SetDlgItemTextA(hwnd,IDC_PC20,arr);
                            break;
                            case 19:
                                SetDlgItemTextA(hwnd,IDC_PC21,arr);
                            break;
                            case 20:
                                SetDlgItemTextA(hwnd,IDC_PC22,arr);
                            break;
                            case 21:
                                SetDlgItemTextA(hwnd,IDC_PC23,arr);
                            break;
                            case 22:
                                SetDlgItemTextA(hwnd,IDC_PC24,arr);
                            break;
                            case 23:
                                SetDlgItemTextA(hwnd,IDC_PC25,arr);
                            break;
                            case 24:
                                SetDlgItemTextA(hwnd,IDC_PC26,arr);
                            break;
                            case 25:
                                SetDlgItemTextA(hwnd,IDC_PC27,arr);
                            break;
                            case 26:
                                SetDlgItemTextA(hwnd,IDC_PC28,arr);
                            break;
                            case 27:
                                SetDlgItemTextA(hwnd,IDC_PC29,arr);
                            break;
                            case 28:
                                SetDlgItemTextA(hwnd,IDC_PC30,arr);
                            break;
                            case 29:
                                SetDlgItemTextA(hwnd,IDC_PC31,arr);
                            break;
                            case 30:
                                SetDlgItemTextA(hwnd,IDC_PC32,arr);
                            break;
                            case 31:
                                SetDlgItemTextA(hwnd,IDC_PC33,arr);
                            break;
                            }
                        }
                        UpdateWindow(hwnd);
                    }
                }
				break;

				case IDC_RESET:
				{
				    SetDlgItemTextA(hwnd,IDC_PC,"Reset!");
				    SetDlgItemTextA(hwnd,IDC_Name,0);
				    SetDlgItemTextA(hwnd,IDC_PC2,0);
				    SetDlgItemTextA(hwnd,IDC_PC3,0);
				    SetDlgItemTextA(hwnd,IDC_PC4,0);
				    SetDlgItemTextA(hwnd,IDC_PC5,0);
				    SetDlgItemTextA(hwnd,IDC_PC6,0);
				    SetDlgItemTextA(hwnd,IDC_PC7,0);
				    SetDlgItemTextA(hwnd,IDC_PC8,0);
				    SetDlgItemTextA(hwnd,IDC_PC9,0);
				    SetDlgItemTextA(hwnd,IDC_PC10,0);
				    SetDlgItemTextA(hwnd,IDC_PC11,0);
				    SetDlgItemTextA(hwnd,IDC_PC12,0);
				    SetDlgItemTextA(hwnd,IDC_PC13,0);
				    SetDlgItemTextA(hwnd,IDC_PC14,0);
				    SetDlgItemTextA(hwnd,IDC_PC15,0);
				    SetDlgItemTextA(hwnd,IDC_PC16,0);
				    SetDlgItemTextA(hwnd,IDC_PC17,0);
				    SetDlgItemTextA(hwnd,IDC_PC18,0);
				    SetDlgItemTextA(hwnd,IDC_PC19,0);
				    SetDlgItemTextA(hwnd,IDC_PC20,0);
				    SetDlgItemTextA(hwnd,IDC_PC21,0);
				    SetDlgItemTextA(hwnd,IDC_PC22,0);
				    SetDlgItemTextA(hwnd,IDC_PC23,0);
				    SetDlgItemTextA(hwnd,IDC_PC24,0);
				    SetDlgItemTextA(hwnd,IDC_PC25,0);
				    SetDlgItemTextA(hwnd,IDC_PC26,0);
				    SetDlgItemTextA(hwnd,IDC_PC27,0);
				    SetDlgItemTextA(hwnd,IDC_PC28,0);
				    SetDlgItemTextA(hwnd,IDC_PC29,0);
				    SetDlgItemTextA(hwnd,IDC_PC30,0);
				    SetDlgItemTextA(hwnd,IDC_PC31,0);
				    SetDlgItemTextA(hwnd,IDC_PC32,0);
				    SetDlgItemTextA(hwnd,IDC_PC33,0);
				    UpdateWindow(hwnd);
				}
				break;
            }
            break;
		default:
			return FALSE;
	}
	return TRUE;
}

//**********************************************************************

BOOL CALLBACK ToolDlgProc_RUN_SIMULATION_STS(HWND hwnd, UINT Message1, WPARAM wParam, LPARAM lParam)
{
	switch(Message1)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_CLOSE_SIMULATION_STS:
					DestroyWindow(g_hToolbar_RUN_SIMULATION_STS);
				break;

				case IDC_RUN_STS:
				{
				    char Times[10];
				    GetDlgItemTextA(hwnd,IDC_TIME,Times,10);
				    if(!strcmp(Times,""))
                    {
                        MessageBox(NULL, "Please enter the time", "Error!",MB_ICONEXCLAMATION | MB_OK);
                    }
                    else
                    {
                        float Timei=0;
                        Timei=atof(Times);
                        //Start
                        char filename[MAXFILENAME];
                        GetDlgItemTextA(hwnd,IDC_Name_STS,filename,MAXFILENAME);
                        if(!strcmp(filename,""))
                        {
                            MessageBox(NULL, "Please enter the name", "Error!",MB_ICONEXCLAMATION | MB_OK);
                        }
                        else
                        {
                            /// stage 1
                            struct instructionMemory_input* insMem_in = (struct instructionMemory_input*) malloc(sizeof(struct instructionMemory_input));
                            struct instructionMemory_output* insMem_out;
                            struct insMem_t* insMem_tin = (struct insMem_t*) malloc(sizeof(struct insMem_t));
                            insMem_tin->in = insMem_in;
                            insMem_out = insMem_tin->out;


                            struct pcAdder_input* pcAdder_in = (struct pcAdder_input*) malloc(sizeof(struct pcAdder_input));
                            struct pcAdder_output* pcAdder_out;
                            struct pcAdder_t* pcAdder_tin = (struct pcAdder_t*) malloc(sizeof(struct pcAdder_t));
                            pcAdder_tin->in = pcAdder_in;
                            pcAdder_tin->out = pcAdder_out;

                            /// pipeline
                            struct IF_ID_input* IF_ID_in = (struct IF_ID_input*) malloc(sizeof(struct IF_ID_input));
                            struct IF_ID_output* IF_ID_out = (struct IF_ID_output*) malloc(sizeof(struct IF_ID_output));
                            struct IF_ID_t* IF_ID_tin = (struct IF_ID_t*) malloc(sizeof(struct IF_ID_t));
                            IF_ID_tin->in = IF_ID_in;
                            IF_ID_tin->out = IF_ID_out;

                            /// stage 2
                            struct registerFile_input* regFile_in = (struct registerFile_input*) malloc(sizeof(struct registerFile_input));
                            struct registerFile_output* regFile_out;
                            struct regFile_t* regFile_tin = (struct regFile_t*) malloc(sizeof(struct regFile_t));
                            regFile_tin->in = regFile_in;
                            regFile_tin->out = regFile_out;

                            int* extend_in = (int*) malloc(sizeof(int)*16);
                            int* extend_out;
                            struct extend_t* extend_tin = (struct extend_t*) malloc(sizeof(struct extend_t));
                            extend_tin->in = extend_in;
                            extend_tin->out = extend_out;

                            /// pipeline
                            struct ID_EX_input* ID_EX_in = (struct ID_EX_input*) malloc(sizeof(struct ID_EX_input));
                            struct ID_EX_output* ID_EX_out = (struct ID_EX_output*) malloc(sizeof(struct ID_EX_output));
                            struct ID_EX_t* ID_EX_tin = (struct ID_EX_t*) malloc(sizeof(struct ID_EX_t));
                            ID_EX_tin->in = ID_EX_in;
                            ID_EX_tin->out = ID_EX_out;

                            /// stage 3
                            struct alu_input* alu_in = (struct alu_input*) malloc(sizeof(struct alu_input));
                            struct alu_output* alu_out;
                            struct alu_t* alu_tin = (struct alu_t*) malloc(sizeof(struct alu_t));
                            alu_tin->in = alu_in;
                            alu_tin->out = alu_out;

                            struct branchAdder_input* branchAdder_in = (struct branchAdder_input*) malloc(sizeof(struct branchAdder_input));
                            struct branchAdder_output* branchAdder_out;
                            struct branchAdder_t* branchAdder_tin = (struct branchAdder_t*) malloc(sizeof(struct branchAdder_t));
                            branchAdder_tin -> in = branchAdder_in;
                            branchAdder_tin -> out = branchAdder_out;

                            int* shl_in = (int*) malloc(sizeof(int)*32);
                            int* shl_out;
                            struct shl_t* shl_tin = (struct shl_t*) malloc(sizeof(struct shl_t));
                            shl_tin->in = shl_in;
                            shl_tin->out = shl_out;

                            /// pipeline
                            struct EX_MEM_input* EX_MEM_in = (struct EX_MEM_input*) malloc(sizeof(struct EX_MEM_input));
                            struct EX_MEM_output* EX_MEM_out = (struct EX_MEM_output*) malloc(sizeof(struct EX_MEM_output));
                            struct EX_MEM_t* EX_MEM_tin = (struct EX_MEM_t*) malloc(sizeof(struct EX_MEM_t));
                            EX_MEM_tin -> in = EX_MEM_in;
                            EX_MEM_tin -> out = EX_MEM_out;

                            /// stage 4
                            struct dataMemory_input* dataMem_in = (struct dataMemory_input*) malloc(sizeof(struct dataMemory_input));
                            struct dataMemory_output* dataMem_out;
                            struct dataMem_t* dataMem_tin = (struct dataMem_t*) malloc(sizeof(struct dataMem_t));
                            dataMem_tin->in = dataMem_in;
                            dataMem_tin->out = dataMem_out;

                            /// pipeline
                            struct MEM_WB_input* MEM_WB_in = (struct MEM_WB_input*) malloc(sizeof(struct MEM_WB_input));
                            struct MEM_WB_output* MEM_WB_out = (struct MEM_WB_output*) malloc(sizeof(struct MEM_WB_output));
                            struct MEM_WB_t* MEM_WB_tin = (struct MEM_WB_t*) malloc(sizeof(struct MEM_WB_t));
                            MEM_WB_tin->in = MEM_WB_in;
                            MEM_WB_tin->out = MEM_WB_out;

                            /// Control
                            struct CU_input* cu_in = (struct CU_input*) malloc(sizeof(struct CU_input));
                            struct CU_output* cu_out;
                            struct CU_t* CU_tin = (struct CU_t*) malloc(sizeof(struct CU_t));
                            CU_tin->in = cu_in;
                            CU_tin->out = cu_out;

                            struct aluControl_input* aluC_in = (struct aluControl_input*) malloc(sizeof(struct aluControl_input));
                            struct aluControl_output* aluC_out;
                            struct aluControl_t* aluControl_tin = (struct aluControl_t*) malloc(sizeof(struct aluControl_t));
                            aluControl_tin->in = aluC_in;
                            aluControl_tin->out = aluC_out;

                            struct ForwardingUnit_input* FU_in = (struct ForwardingUnit_input*) malloc(sizeof(struct ForwardingUnit_input));
                            struct ForwardingUnit_output* FU_out;

                            struct HazardDet_input* HD_in = (struct HazardDet_input*) malloc(sizeof(struct HazardDet_input));
                            struct HazardDet_output* HD_out;


                            /// initial
                            copyArr(decTobin(0,32), pc, 32);
                            int stage[5][32];
                            char stageString[5][33];
                            int pc_select = 0;
                            int pcWrite = 1;
                            int lineNum = 0;
                            DWORD id;
                            int isEnd = 5;
                            int i;
                            int *make_zero;
                            make_zero = decTobin(0,32);
                            for (i=0;i<32;i++){
                                copyArr(make_zero, registers[i], 32);
                            }

                            for (i=0;i<MEMORY_SIZE;i++){
                                copyArr(make_zero, DataMemory[i], 32);
                                copyArr(make_zero, InstructionMemory[i], 32);
                            }
                            assembler(filename, "out.o");

                            HANDLE stageHandles[10];
                            HANDLE pipeHandles[5];

                            copyArr(decTobin(0,160), stage, 160);

                            IF_ID_in->flush = ID_EX_in->flush = EX_MEM_in->flush = MEM_WB_in->flush = 1;
                            IF_ID(IF_ID_in, IF_ID_out);
                            ID_EX(ID_EX_in, ID_EX_out);
                            EX_MEM(EX_MEM_in, EX_MEM_out);
                            MEM_WB(MEM_WB_in, MEM_WB_out);
                            IF_ID_in->flush = ID_EX_in->flush = EX_MEM_in->flush = MEM_WB_in->flush = 0;

                            long num = readIns("out.o", 0);
                            int clock = 0;
                            while ( isEnd > 0 )
                            {
                                clock++;
                                /// pipeline
                                copyArr(IF_ID_out->Rs, ID_EX_in->Rs, 5);
                                copyArr(IF_ID_out->Rt, ID_EX_in->Rt, 5);
                                copyArr(IF_ID_out->pc, ID_EX_in->pc, 32);
                                copyArr(IF_ID_out->regDst1, ID_EX_in->regDst1, 5);
                                copyArr(IF_ID_out->regDst2, ID_EX_in->regDst2, 5);

                                copyArr(ID_EX_out->readReg2, EX_MEM_in->read_data2, 32);
                                copyArr(mux(ID_EX_out->regDst1, ID_EX_out->regDst2, ID_EX_out->regDst), EX_MEM_in->muxout, 5);
                                copyArr(ID_EX_out->WB, EX_MEM_in->wb, 2);
                                copyArr(ID_EX_out->M, EX_MEM_in->mem, 3);

                                copyArr(EX_MEM_out->alu_result, MEM_WB_in->alu_result, 32);
                                copyArr(EX_MEM_out->muxout, MEM_WB_in->muxout, 5);
                                copyArr(EX_MEM_out->wb, MEM_WB_in->wb, 2);

                                /// stage 1
                                copyArr(pc, insMem_in->readAddress, 32);
                                copyArr(pc, pcAdder_in->pc, 32);

                                /// stage 2
                                copyArr(IF_ID_out->Rs, HD_in->IF_ID_Rs, 5);
                                copyArr(IF_ID_out->Rt, HD_in->IF_ID_Rt, 5);
                                copyArr(ID_EX_out->Rt, HD_in->ID_EX_Rt, 5);
                                HD_in->ID_EX_memRead = ID_EX_out->M[1];
                                HD_out = HazardDet(HD_in);
                                pcWrite = HD_out->pcWrite;
                                IF_ID_in->WE = HD_out->IF_ID_write;

                                copyArr(IF_ID_out->controlUnit, cu_in->opcode, 6);
                                copyArr(IF_ID_out->readReg1, regFile_in->readReg1, 5);
                                copyArr(IF_ID_out->readReg2, regFile_in->readReg2, 5);
                                copyArr(IF_ID_out->address, extend_in, 16);

                                copyArr(IF_ID_out->pc, branchAdder_in->pc, 32);
                                copyArr(shiftLeft(Extends(IF_ID_out->address), 32, 2), branchAdder_in->branchAddress, 32);

                                /// stage 3
                                copyArr(ID_EX_out->aluOp, aluC_in->aluOp, 2);
                                copyArr(ID_EX_out->func, aluC_in->func, 6);
                                aluC_out = aluControl(aluC_in);
                                copyArr(ID_EX_out->Rs, FU_in->Rs, 5);
                                copyArr(ID_EX_out->Rt, FU_in->Rt, 5);
                                copyArr(EX_MEM_out->muxout, FU_in->EX_MEM_RegDst, 5);
                                copyArr(MEM_WB_out->muxout, FU_in->MEM_WB_RegDst, 5);
                                copyArr(EX_MEM_out->wb, FU_in->EX_MEM_WB, 2);
                                FU_in->MEM_WB_WB = MEM_WB_out->regWrite;
                                FU_out = FU(FU_in);

                                copyArr(aluC_out, alu_in->aluControl, 4);
                                copyArr(mux2(ID_EX_out->readReg1, mux(MEM_WB_out->alu_result, MEM_WB_out->readData, MEM_WB_out->memToReg), EX_MEM_out->alu_result, NULL, FU_out->aluIn1), alu_in->input1, 32);
                                copyArr(mux2(mux(ID_EX_out->readReg2, ID_EX_out->address, ID_EX_out->aluSrc), mux(MEM_WB_out->alu_result, MEM_WB_out->readData, MEM_WB_out->memToReg), EX_MEM_out->alu_result, NULL, FU_out->aluIn2), alu_in->input2, 32);

                                /// stage 4
                                copyArr(EX_MEM_out->alu_result, dataMem_in->address, 32);
                                copyArr(EX_MEM_out->read_data2, dataMem_in->writeData, 32);
                                dataMem_in->memRead = EX_MEM_out->memread;
                                dataMem_in->memWrite = EX_MEM_out->memwrite;

                                /// stage 5
                                copyArr(mux(MEM_WB_out->alu_result, MEM_WB_out->readData, MEM_WB_out->memToReg), regFile_in->writeData, 32);
                                copyArr(MEM_WB_out->muxout, regFile_in->writeReg, 5);
                                regFile_in->regWrite = MEM_WB_out->regWrite;

                                /// stage threads
                                stageHandles[0] = CreateThread(NULL, sizeof(struct insMem_t), insMem_tf, insMem_tin, 0, &id);
                                stageHandles[1] = CreateThread(NULL, sizeof(struct pcAdder_t), pcAdder_tf, pcAdder_tin, 0, &id);

                                stageHandles[2] = CreateThread(NULL, sizeof(struct CU_t), CU_tf, CU_tin, 0, &id);
                                stageHandles[3] = CreateThread(NULL, sizeof(struct regFile_t), regFile_tf, regFile_tin, 0, &id);
                                stageHandles[4] = CreateThread(NULL, sizeof(struct extend_t), extend_tf, extend_tin, 0, &id);
                                stageHandles[5] = CreateThread(NULL, sizeof(struct branchAdder_t), branchAdder_tf, branchAdder_tin, 0, &id);

                                stageHandles[6] = CreateThread(NULL, sizeof(struct alu_t), alu_tf, alu_tin, 0, &id);

                                stageHandles[7] = CreateThread(NULL, sizeof(struct dataMem_t), dataMem_tf, dataMem_tin, 0, &id);

                                WaitForMultipleObjects(8, stageHandles, TRUE, INFINITE);

                                /// assign outputs
                                insMem_out = insMem_tin->out;
                                pcAdder_out = pcAdder_tin->out;

                                cu_out = CU_tin->out;
                                regFile_out = regFile_tin->out;
                                extend_out = extend_tin->out;

                                branchAdder_out = branchAdder_tin->out;
                                alu_out = alu_tin->out;

                                dataMem_out = dataMem_tin->out;
                                ///

                                /// hazardDetection
                                pc_select = !mystrcmp(regFile_out->readData1, regFile_out->readData2, 32, 32) && cu_out->M[0];
                                if (pc_select){
                                    IF_ID_in->flush = 1;
                                    pcWrite = 1;
                                    HD_out->CUmux = 1;
                                }
                                else{
                                    IF_ID_in->flush = 0;
                                }

                                /// mux pc write
                                if (pcWrite == 1){
                                    copyArr(mux(pcAdder_out->pc, branchAdder_out->pc , pc_select), pc, 32);
                                }

                                /// test
                                printf("line%d : ", lineNum);
                                prtArr(insMem_out->instruction, 32);

                                /// pipeline IF/ID
                                copyArr(insMem_out->instruction, IF_ID_in->instruction, 32);
                                copyArr(pcAdder_out->pc, IF_ID_in->pc,32);
                                IF_ID(IF_ID_in, IF_ID_out);

                                /// pipline ID/EX
                                copyArr(regFile_out->readData1, ID_EX_in->readData1, 32);
                                copyArr(regFile_out->readData2, ID_EX_in->readData2, 32);
                                copyArr(extend_out, ID_EX_in->address, 32);

                                if ( HD_out->CUmux == 1){
                                    copyArr(decTobin(0, 4), ID_EX_in->EX, 4);
                                    copyArr(decTobin(0, 3), ID_EX_in->M, 3);
                                    copyArr(decTobin(0, 2), ID_EX_in->WB, 2);
                                }
                                else{
                                    copyArr(cu_out->ex, ID_EX_in->EX, 4);
                                    copyArr(cu_out->M, ID_EX_in->M, 3);
                                    copyArr(cu_out->WB, ID_EX_in->WB, 2);
                                }
                                ID_EX(ID_EX_in, ID_EX_out);

                                /// pipeline EX/MEM
                                EX_MEM_in->alu_zero = alu_out->zero;
                                copyArr(alu_out->result, EX_MEM_in->alu_result, 32);
                                EX_MEM(EX_MEM_in, EX_MEM_out);

                                /// pipeline MEM/WB
                                copyArr(dataMem_out->dataRead, MEM_WB_in->readdata, 32);
                                MEM_WB(MEM_WB_in, MEM_WB_out);


                                /// is end ??
                                if ( !mystrcmp(insMem_out->instruction, decTobin(0,32), 32, 32) ){
                                    isEnd--;
                                }
                                else if ( mystrcmp(insMem_out->instruction, decTobin(0,32), 32, 32) ){
                                    isEnd = 5;
                                }


                                /// stage state
                                copyArr(stage[3], stage[4], 32);
                                copyArr(stage[2], stage[3], 32);
                                copyArr(stage[1], stage[2], 32);
                                copyArr(stage[0], stage[1], 32);
                                copyArr(insMem_out->instruction, stage[0], 32);

                                num--;
                                lineNum++;
                                int j,n;
                                for(j=0;j<5;j++)
                                {
                                    for(n=0;n<32;n++)
                                    {
                                        if(stage[j][n]==1)
                                        {
                                            stageString[j][n]='1';
                                        }
                                        else if(stage[j][n]==0)
                                        {
                                            stageString[j][n]='0';
                                        }
                                    }
                                }
                                stageString[0][32]='\0';
                                stageString[1][32]='\0';
                                stageString[2][32]='\0';
                                stageString[3][32]='\0';
                                stageString[4][32]='\0';
                                SetDlgItemTextA(hwnd, IDC_STAGE1,stageString[0]);
                                SetDlgItemTextA(hwnd, IDC_STAGE2,stageString[1]);
                                SetDlgItemTextA(hwnd, IDC_STAGE3,stageString[2]);
                                SetDlgItemTextA(hwnd, IDC_STAGE4,stageString[3]);
                                SetDlgItemTextA(hwnd, IDC_STAGE5,stageString[4]);
                                int ans,i;
                                char arr[31];
                                itoa(clock,arr,10);
                                SetDlgItemTextA(hwnd, IDC_PC_STS,arr);
                                for(i=0;i<32;i++)
                                {
                                    ans=binTodec(registers[i],32,1);
                                    itoa(ans,arr,10);
                                    switch(i)
                                    {
                                    case 0:
                                        SetDlgItemTextA(hwnd,IDC_PC2_STS,arr);
                                    break;
                                    case 1:
                                        SetDlgItemTextA(hwnd,IDC_PC3_STS,arr);
                                    break;
                                    case 2:
                                        SetDlgItemTextA(hwnd,IDC_PC4_STS,arr);
                                    break;
                                    case 3:
                                        SetDlgItemTextA(hwnd,IDC_PC5_STS,arr);
                                    break;
                                    case 4:
                                        SetDlgItemTextA(hwnd,IDC_PC6_STS,arr);
                                    break;
                                    case 5:
                                        SetDlgItemTextA(hwnd,IDC_PC7_STS,arr);
                                    break;
                                    case 6:
                                        SetDlgItemTextA(hwnd,IDC_PC8_STS,arr);
                                    break;
                                    case 7:
                                        SetDlgItemTextA(hwnd,IDC_PC9_STS,arr);
                                    break;
                                    case 8:
                                        SetDlgItemTextA(hwnd,IDC_PC10_STS,arr);
                                    break;
                                    case 9:
                                        SetDlgItemTextA(hwnd,IDC_PC11_STS,arr);
                                    break;
                                    case 10:
                                        SetDlgItemTextA(hwnd,IDC_PC12_STS,arr);
                                    break;
                                    case 11:
                                        SetDlgItemTextA(hwnd,IDC_PC13_STS,arr);
                                    break;
                                    case 12:
                                        SetDlgItemTextA(hwnd,IDC_PC14_STS,arr);
                                    break;
                                    case 13:
                                        SetDlgItemTextA(hwnd,IDC_PC15_STS,arr);
                                    break;
                                    case 14:
                                        SetDlgItemTextA(hwnd,IDC_PC16_STS,arr);
                                    break;
                                    case 15:
                                        SetDlgItemTextA(hwnd,IDC_PC17_STS,arr);
                                    break;
                                    case 16:
                                        SetDlgItemTextA(hwnd,IDC_PC18_STS,arr);
                                    break;
                                    case 17:
                                        SetDlgItemTextA(hwnd,IDC_PC19_STS,arr);
                                    break;
                                    case 18:
                                        SetDlgItemTextA(hwnd,IDC_PC20_STS,arr);
                                    break;
                                    case 19:
                                        SetDlgItemTextA(hwnd,IDC_PC21_STS,arr);
                                    break;
                                    case 20:
                                        SetDlgItemTextA(hwnd,IDC_PC22_STS,arr);
                                    break;
                                    case 21:
                                        SetDlgItemTextA(hwnd,IDC_PC23_STS,arr);
                                    break;
                                    case 22:
                                        SetDlgItemTextA(hwnd,IDC_PC24_STS,arr);
                                    break;
                                    case 23:
                                        SetDlgItemTextA(hwnd,IDC_PC25_STS,arr);
                                    break;
                                    case 24:
                                        SetDlgItemTextA(hwnd,IDC_PC26_STS,arr);
                                    break;
                                    case 25:
                                        SetDlgItemTextA(hwnd,IDC_PC27_STS,arr);
                                    break;
                                    case 26:
                                        SetDlgItemTextA(hwnd,IDC_PC28_STS,arr);
                                    break;
                                    case 27:
                                        SetDlgItemTextA(hwnd,IDC_PC29_STS,arr);
                                    break;
                                    case 28:
                                        SetDlgItemTextA(hwnd,IDC_PC30_STS,arr);
                                    break;
                                    case 29:
                                        SetDlgItemTextA(hwnd,IDC_PC31_STS,arr);
                                    break;
                                    case 30:
                                        SetDlgItemTextA(hwnd,IDC_PC32_STS,arr);
                                    break;
                                    case 31:
                                        SetDlgItemTextA(hwnd,IDC_PC33_STS,arr);
                                    break;
                                    }
                                }
                                UpdateWindow(hwnd);
                                Sleep(Timei*1000);
                            }
                            printf("\nclock counter: %d\n",clock);
                            //END
                        }
                    }
                }
                break;
                case IDC_RESET:
                {
                        SetDlgItemTextA(hwnd,IDC_Name_STS,0);
                        SetDlgItemTextA(hwnd,IDC_TIME,0);
                        SetDlgItemTextA(hwnd,IDC_PC_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC2_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC3_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC4_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC5_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC6_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC7_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC8_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC9_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC10_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC11_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC12_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC13_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC14_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC15_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC16_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC17_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC18_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC19_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC20_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC21_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC22_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC23_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC24_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC25_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC26_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC27_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC28_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC29_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC30_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC31_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC32_STS,0);
                        SetDlgItemTextA(hwnd,IDC_PC33_STS,0);
                        UpdateWindow(hwnd);
                }
                break;
        }
        break;
        default:
            return FALSE;
	}
	return TRUE;
}

//*******************************************************************************

//Run commends of the help window
BOOL CALLBACK ToolDlgProc_HELP(HWND hwnd, UINT Message1, WPARAM wParam, LPARAM lParam)
{
	switch(Message1)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_CLOSE_HELP:
					DestroyWindow(g_hToolbar_HELP);
				break;
			}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

//***********************************************************************

//Run commends of the cradit window
BOOL CALLBACK ToolDlgProc_CRADIT(HWND hwnd, UINT Message1, WPARAM wParam, LPARAM lParam)
{
	switch(Message1)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_CLOSE_CRADIT:
					DestroyWindow(g_hToolbar_CRADIT);
				break;
			}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

//**************************************************************************

//Open File
void DoFileOpen(HWND hwnd)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";

	if(GetOpenFileName(&ofn))
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		LoadTextFileToEdit(hEdit, szFileName);
	}
}

//*****************************************************************

BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize;

		dwFileSize = GetFileSize(hFile, NULL);
		if(dwFileSize != 0xFFFFFFFF)
		{
			LPSTR pszFileText;

			pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if(pszFileText != NULL)
			{
				DWORD dwRead;

				if(ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
				{
					pszFileText[dwFileSize] = 0; // Add null terminator
					if(SetWindowText(hEdit, pszFileText))
						bSuccess = TRUE; // It worked!
				}
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength;

		dwTextLength = GetWindowTextLength(hEdit);
		// No need to bother if there's no text.
		if(dwTextLength > 0)
		{
			LPSTR pszText;
			DWORD dwBufferSize = dwTextLength + 1;

			pszText = (LPSTR)GlobalAlloc(GPTR, dwBufferSize);
			if(pszText != NULL)
			{
				if(GetWindowText(hEdit, pszText, dwBufferSize))
				{
					DWORD dwWritten;

					if(WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
						bSuccess = TRUE;
				}
				GlobalFree(pszText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

void DoFileSave(HWND hwnd)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if(GetSaveFileName(&ofn))
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		SaveTextFileFromEdit(hEdit, szFileName);
	}
}

//***********************************************************************************

//Run commends of the Edit window
BOOL CALLBACK ToolDlgProc_EDIT(HWND hwnd, UINT Message1, WPARAM wParam, LPARAM lParam)
{
	switch(Message1)
	{
		    case WM_INITDIALOG:
				{
					HFONT hfDefault;
					HWND hEdit;

			        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
						WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
						5, 5, 440, 200, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
					if(hEdit == NULL)
						MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

					hfDefault = GetStockObject(DEFAULT_GUI_FONT);
					SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

					DoFileOpen(hwnd);
				}
			break;

			case WM_SIZE:
				{
					HWND hEdit;
					RECT rcClient;

					GetClientRect(hwnd, &rcClient);

					hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
					SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
				}
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			    case IDC_SAVE_EDIT:
					DoFileSave(hwnd);
				break;
				case IDC_CLOSE_EDIT:
					DestroyWindow(g_hToolbar_EDIT);
				break;

				case IDC_OPEN_EDIT:
				    printf("Ok");
					DoFileOpen(hwnd);
				break;
			}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

//***********************************************************************************

//Run commends of the main window
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case ID_FILE_EXIT:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;

				case ID_FILE_EDIT:
					//Create Edit window
					g_hToolbar_EDIT = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR_EDIT),
						hwnd, ToolDlgProc_EDIT);
					if(g_hToolbar_EDIT != NULL)
					{
						ShowWindow(g_hToolbar_EDIT, SW_SHOW);
					}
					else
					{
						MessageBox(hwnd, "CreateDialog returned NULL", "Warning!",
							MB_OK | MB_ICONINFORMATION);
					}
				break;

				case ID_RUN_SIMULATION_STS:
					g_hToolbar_RUN_SIMULATION_STS = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR_RUN_SIMULATION_STS),
						hwnd, ToolDlgProc_RUN_SIMULATION_STS);
					if(g_hToolbar_RUN_SIMULATION_STS != NULL)
					{
						ShowWindow(g_hToolbar_RUN_SIMULATION_STS, SW_SHOW);
					}
					else
					{
						MessageBox(hwnd, "CreateDialog returned NULL", "Warning!",
							MB_OK | MB_ICONINFORMATION);
					}
				break;

				case ID_HELP_HELP:
					//Create help window
					g_hToolbar_HELP = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR_HELP),
						hwnd, ToolDlgProc_HELP);
					if(g_hToolbar_HELP != NULL)
					{
						ShowWindow(g_hToolbar_HELP, SW_SHOW);
					}
					else
					{
						MessageBox(hwnd, "CreateDialog returned NULL", "Warning!",
							MB_OK | MB_ICONINFORMATION);
					}
				break;

				case ID_HELP_CRADIT:
					//Create cradit window
					g_hToolbar_CRADIT = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR_CRADIT),
						hwnd, ToolDlgProc_CRADIT);
					if(g_hToolbar_CRADIT != NULL)
					{
						ShowWindow(g_hToolbar_CRADIT, SW_SHOW);
					}
					else
					{
						MessageBox(hwnd, "CreateDialog returned NULL", "Warning!",
							MB_OK | MB_ICONINFORMATION);
					}
				break;

				case ID_RUN_SIMULATION:
					g_hToolbar_RUN_SIMULATION = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR_RUN_SIMULATION),
						hwnd, ToolDlgProc_RUN_SIMULATION);
					if(g_hToolbar_RUN_SIMULATION != NULL)
					{
						ShowWindow(g_hToolbar_RUN_SIMULATION, SW_SHOW);
					}
					else
					{
						MessageBox(hwnd, "CreateDialog returned NULL", "Warning!",
							MB_OK | MB_ICONINFORMATION);
					}
				break;

			}
		case WM_CREATE:
		{
			UINT ret;
			BITMAP bm;

			g_hbmBall = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BALL));
			if(g_hbmBall == NULL)
				MessageBox(hwnd, "Could not load IDB_BALL!", "Error", MB_OK | MB_ICONEXCLAMATION);

			g_hbmMask = CreateBitmapMask(g_hbmBall, RGB(0, 0, 0));
			if(g_hbmMask == NULL)
				MessageBox(hwnd, "Could not create mask!", "Error", MB_OK | MB_ICONEXCLAMATION);

			GetObject(g_hbmBall, sizeof(bm), &bm);

			ZeroMemory(&g_ballInfo, sizeof(g_ballInfo));
			g_ballInfo.width = bm.bmWidth;
			g_ballInfo.height = bm.bmHeight;

			g_ballInfo.dx = BALL_MOVE_DELTA;
			g_ballInfo.dy = BALL_MOVE_DELTA;

			ret = SetTimer(hwnd, ID_TIMER, 50, NULL);
			if(ret == 0)
				MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_PAINT:
		{
			RECT rcClient;
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &rcClient);
			DrawBall(hdc, &rcClient);

			EndPaint(hwnd, &ps);
		}
		break;
		case WM_TIMER:
		{
			RECT rcClient;
			HDC hdc = GetDC(hwnd);

			GetClientRect(hwnd, &rcClient);

			UpdateBall(&rcClient);
			DrawBall(hdc, &rcClient);

			ReleaseDC(hwnd, hdc);
		}
		break;
		case WM_DESTROY:
			KillTimer(hwnd, ID_TIMER);

			DeleteObject(g_hbmBall);
			DeleteObject(g_hbmMask);

			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

//*********************************************************************************

//Main function of programm
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MYMENU);
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"MIPS Simulation",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 620, 480,
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

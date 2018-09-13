#include "header.h"
#include "string.h"

void strsplit(char *source,char *separators,char result[][32]);
void assembler(char infile[],char outfile[]);
int *decTobin(long num, int len);

void assembler(char infile[],char outfile[]){
    FILE *in = fopen(infile,"r");
    FILE *out = fopen(outfile,"w");
    char in_line[33];
    char out_line[33];
    char c = fgetc(in);
    int i = 0,j = 0;
    char res[10][32];
    while(c != EOF){
        i = 0;
        while(c != '\n' && c != EOF){
            in_line[i++] = c;
            c = fgetc(in);
        }
        in_line[i] = '\0';
        printf("%s\n",in_line);
        strsplit(in_line," ",res);

        /// checking first part
        if(strcmp(res[0],"add") == 0){
            //printf("add");
            asm_rtype(res,out_line);
            asm_add(out_line);
        }
        if(strcmp(res[0],"sub") == 0){
            //printf("sub");
            asm_rtype(res,out_line);
            asm_sub(out_line);
        }
        if(strcmp(res[0],"lw") == 0){
            //printf("lw");
            asm_lw(res,out_line);
        }
        if(strcmp(res[0],"sw") == 0){
            //printf("sw");
            asm_sw(res,out_line);
        }
        if(strcmp(res[0],"addi") == 0){
            //printf("addi");
            asm_addi(res, out_line);
        }
        if(strcmp(res[0],"movei") == 0){
            //printf("addi");
            asm_movei(res, out_line);
        }
        if(strcmp(res[0],"mover") == 0){
            //printf("add");
            asm_mover(res,out_line);
            asm_add(out_line);
        }
        if(strcmp(res[0],"beq") == 0){
            //printf("beq");
            asm_beq(res,out_line);
        }
        if(strcmp(res[0],"j") == 0){
            //printf("j");
            asm_j(res,out_line);
        }
        if(strcmp(res[0],"slt") == 0){
            //printf("slt");
            asm_rtype(res,out_line);
            asm_slt(out_line);
        }
        if(strcmp(res[0],"and") == 0){
            //printf("and");
            asm_rtype(res,out_line);
            asm_and(out_line);
        }
        if(strcmp(res[0],"or") == 0){
            //printf("or");
            asm_rtype(res,out_line);
            asm_or(out_line);
        }
        out_line[32] = '\0';
        int i;
        for(i = 0;i < 32;i ++){
            fprintf(out,"%d",out_line[i]);
        }
        if ( (c = fgetc(in)) != EOF){
            fprintf(out,"\n");
        }
    }
    fclose(in);
    fclose(out);
}

void asm_rtype(char in[][32],char ans[]){
    ///opcode
    ans[0] = 1;
    ans[1] = 0;
    ans[2] = 0;
    ans[3] = 0;
    ans[4] = 0;
    ans[5] = 0;

    ///rs
    int *regNum;
    int num;
    char temp[32];
    int i;
    for(i = 1;in[2][i] != '\0';i++)
        temp[i-1] = in[2][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 6] = regNum[i];
        //printf("%d ",regNum[i]);
    }
    printf("\n");

    ///rt
    for(i = 1;in[3][i] != '\0';i++)
        temp[i-1] = in[3][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 11] = regNum[i];
    }
    ///rd
    for(i = 1;in[1][i] != '\0';i++)
        temp[i-1] = in[1][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 16] = regNum[i];
    }
    ///shift amount
    for(i = 21;i < 26;i ++){
        ans[i] = 0;
    }
}

void asm_add(char ans[]){
    ///function
    ans[26] = 1;
    ans[27] = 0;
    ans[28] = 0;
    ans[29] = 0;
    ans[30] = 0;
    ans[31] = 0;
}

void asm_sub(char ans[]){
    ///function
    ans[26] = 1;
    ans[27] = 0;
    ans[28] = 0;
    ans[29] = 0;
    ans[30] = 1;
    ans[31] = 0;

}
void asm_and(char ans[]){
    ///function
    ans[26] = 1;
    ans[27] = 0;
    ans[28] = 0;
    ans[29] = 1;
    ans[30] = 0;
    ans[31] = 0;

}

void asm_or(char ans[]){
    ///function
    ans[26] = 1;
    ans[27] = 0;
    ans[28] = 0;
    ans[29] = 1;
    ans[30] = 0;
    ans[31] = 1;

}

void asm_slt(char ans[]){
    ///function
    ans[26] = 1;
    ans[27] = 0;
    ans[28] = 1;
    ans[29] = 0;
    ans[30] = 1;
    ans[31] = 0;

}

void asm_lw(char in[][32],char ans[]){
    ///opcode
    ans[0] = 0;
    ans[1] = 0;
    ans[2] = 0;
    ans[3] = 1;
    ans[4] = 1;
    ans[5] = 1;

    ///rs
    int *regNum;
    int num;
    char temp[32];
    int i;
    for(i = 0;in[2][i] != '$';i ++){
    	;
    }
    int j;
	i++;
	for(j = 0;in[2][i] != ')';i++,j++){
		temp[j] = in[2][i];
	}
	temp[j] = '\0';
	num = atoi(temp);
	regNum = decTobin(num,5);
	for(i = 0;i < 5;i ++){
		ans[i + 6] = regNum[i];
	}

    ///rt
    for(i = 1;in[1][i] != '\0';i++)
        temp[i-1] = in[1][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 11] = regNum[i];
    }
    ///address
    for(i = 0;in[2][i] != '(';i++){
		temp[i] = in[2][i];
	}
	temp[i] = '\0';
	num = atoi(temp);
	regNum = decTobin(num,16);
	for(i = 0;i < 16;i ++){
		ans[i + 16] = regNum[i];
	}
}

void asm_sw(char in[][32],char ans[]){
    ///opcode
    ans[0] = 0;
    ans[1] = 0;
    ans[2] = 1;
    ans[3] = 1;
    ans[4] = 1;
    ans[5] = 1;

    ///rs
    int *regNum;
    int num;
    char temp[32];
    int i;
    for(i = 0;in[2][i] != '$';i ++){
    	;
    }
    int j;
	i++;
	for(j = 0;in[2][i] != ')';i++,j++){
		temp[j] = in[2][i];
	}
	temp[j] = '\0';
	num = atoi(temp);
	regNum = decTobin(num,5);
	for(i = 0;i < 5;i ++){
		ans[i + 6] = regNum[i];
	}

    ///rt
    for(i = 1;in[1][i] != '\0';i++)
        temp[i-1] = in[1][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 11] = regNum[i];
    }
    ///address
    for(i = 0;in[2][i] != '(';i++){
		temp[i] = in[2][i];
	}
	temp[i] = '\0';
	num = atoi(temp);
	regNum = decTobin(num,16);
	for(i = 0;i < 16;i ++){
		ans[i + 16] = regNum[i];
	}
}

void asm_beq(char in[][32],char ans[]){
    ///opcode
    ans[0] = 0;
    ans[1] = 1;
    ans[2] = 0;
    ans[3] = 1;
    ans[4] = 1;
    ans[5] = 1;

    ///rs
    int *regNum;
    int num;
    char temp[32];
    int i;
    for(i = 1;in[2][i] != '\0';i++)
        temp[i-1] = in[2][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 6] = regNum[i];
    }

    ///rt
    for(i = 1;in[1][i] != '\0';i++)
        temp[i-1] = in[1][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 11] = regNum[i];
    }
    ///address
    for(i = 0;in[3][i] != '\0';i++){
        temp[i] = in[3][i];
    }
    temp[i] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,16);
    for(i = 0;i < 16;i ++){
        ans[i + 16] = regNum[i];
    }
}


void asm_addi(char in[][32],char ans[]){
    ///opcode
    ans[0] = 0;
    ans[1] = 0;
    ans[2] = 0;
    ans[3] = 0;
    ans[4] = 1;
    ans[5] = 1;

    ///rs
    int *regNum;
    int num;
    char temp[32];
    int i;
    for(i = 1;in[2][i] != '\0';i++)
        temp[i-1] = in[2][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 6] = regNum[i];
    }

    ///rt
    for(i = 1;in[1][i] != '\0';i++)
        temp[i-1] = in[1][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 11] = regNum[i];
    }
    ///address
    for(i = 0;in[3][i] != '\0';i++){
        temp[i] = in[3][i];
    }
    temp[i] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,16);
    for(i = 0;i < 16;i ++){
        ans[i + 16] = regNum[i];
    }
}

void asm_movei(char in[][32],char ans[]){
    ///opcode
    ans[0] = 0;
    ans[1] = 0;
    ans[2] = 0;
    ans[3] = 0;
    ans[4] = 1;
    ans[5] = 1;

    ///rs
    int *regNum;
    int num;
    char temp[32];
    int i;
    for(i = 0;i < 5;i ++){
        ans[i + 6] = 0;
    }

    ///rt
    for(i = 1;in[1][i] != '\0';i++)
        temp[i-1] = in[1][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 11] = regNum[i];
    }
    ///immidiate
    for(i = 0;in[2][i] != '\0';i++){
        temp[i] = in[2][i];
    }
    temp[i] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,16);
    for(i = 0;i < 16;i ++){
        ans[i + 16] = regNum[i];
    }
}

void asm_mover(char in[][32],char ans[]){
    ///opcode
    ans[0] = 1;
    ans[1] = 0;
    ans[2] = 0;
    ans[3] = 0;
    ans[4] = 0;
    ans[5] = 0;

    ///rs
    int *regNum;
    int num;
    char temp[32];
    int i;
    for(i = 1;in[2][i] != '\0';i++)
        temp[i-1] = in[2][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 6] = regNum[i];
        //printf("%d ",regNum[i]);
    }
    printf("\n");

    ///rt
    for(i = 0;i < 5;i ++){
        ans[i + 11] = 0;
    }
    ///rd
    for(i = 1;in[1][i] != '\0';i++)
        temp[i-1] = in[1][i];
    temp[i-1] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,5);
    for(i = 0;i < 5;i ++){
        ans[i + 16] = regNum[i];
    }
    ///shift amount
    for(i = 21;i < 26;i ++){
        ans[i] = 0;
    }
}
void asm_j(char in[][32],char ans[]){
    ///opcode
    ans[0] = 0;
    ans[1] = 0;
    ans[2] = 0;
    ans[3] = 0;
    ans[4] = 1;
    ans[5] = 0;

    int *regNum;
    int num;
    char temp[32];
    int i;

    ///address
    for(i = 0;in[1][i] != '\0';i++)
        temp[i] = in[1][i];
    temp[i] = '\0';
    num = atoi(temp);
    regNum = decTobin(num,26);
    for(i = 0;i < 26;i ++){
        ans[i + 6] = regNum[i];
    }
}

void strsplit(char *source,char *separators,char result[][32]){
    int sign = 0 ,i = 0,k = 0,j;
    int *temp = separators;
    for ( ; *source != '\0' ; source++){
        for ( separators = temp; *separators != '\0'; separators++)
            if ( *source == *separators){
                sign = 1;
                if ( k > 0){
                    result[i][k] = '\0';
                    i++;
                    k = 0;
                }
                break;
            }
        if ( sign != 1)
            result[i][k++] = *source;
        sign = 0;
    }
    result[i][k] = '\0';
}

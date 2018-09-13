#include "header.h"
void big_add(int num1[],int num2[],int len,int ans[]){
    int i = len - 1;
    int temp2 = 0,temp;
    for(len --;len >= 0;len --){
        temp = num2[len] + num1[len] + temp2;
        if(temp>1){
            ans[i--] = temp-2;
            temp2=1;
        }
        else{
            ans[i--] = temp;
            temp2=0;
        }
    }
    if(temp2 == 1)
        ans[i--] = 1;
}


long binTodec(int *arr, int n, int sign){
    long num = 0;
    int i;
    for(i=n-1;i>0;i--){
        num += (long)pow((double)2,n-i-1) * arr[i];
    }
    if(sign == 1){
        num -= (long)pow((double)2,n-1) * arr[0];
    }
    else{
        num += (long)pow((double)2,n-1) * arr[0];
    }
    return num;
}

int *decTobin(long num, int len){
    int *temp = (int*) malloc(sizeof(int) * len);
    int i = len-1;
    int holder, j;
    if (num > 0){
        holder = num;
    }
    else{
        holder = -num;
    }
    while ( holder > 0){
        temp[i--] = holder % 2;
        holder = holder / 2;
    }
    for (j=0;j<=i;j++){
        temp[j] = 0;
    }
    if ( num < 0 ){
        i = len-1;
        while ( temp[i--] == 0 );
        while ( i >= 0){
            temp[i] = !temp[i--];
        }
    }
    return temp;
}

int mystrcmp(int *s,int *t,int len1,int len2){
    if(len1 != len2){
        return 1;
    }
    int i;
    for(i = 0;i < len1 && *s == *t;s ++, t ++,i ++);
    int ans;
    ans = (i == len1) ? 0 : 1;
    return ans;
}

int *mux(int* data1, int* data2, int select){
    return select?data2:data1;
}

int *mux2(int* data1, int* data2, int* data3, int* data4, int* select){
    if ( select[0] == 0){
        if ( select[1] == 0 ){
            return data1;
        }
        return data2;
    }
    else{
        if( select[1] == 0 ){
            return data3;
        }
        return data4;
    }
}

long readIns(char *fileName, long from){
    FILE *file=fopen(fileName,"r");
    int i = 0, j;
    char c;
    while (!feof(file)){
        for(j=0; j<32 && (!feof(file));j++){
            InstructionMemory[from+i][j] =(int) fgetc(file) - '0';
        }
        fgetc(file);
        i+=4;
    }
    fclose(file);
    return i/4;
}

void copyArr(int *from, int *to, int len){
    int i;
    for(i=0;i<len;i++){
        to[i] = from[i];
    }
}

void prtArr(int* arr, int len){
    int i;
    for(i=0;i<len;i++){
        printf("%d", arr[i]);
    }
    printf("\n");
}

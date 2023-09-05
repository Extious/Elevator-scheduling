#include <stdlib.h>
#include<stdio.h>
#include"level1.2.h"
#include"level1.3.h"

void myPrint(int position,int time,int perNum){
    printf("%d %d %d\n",position,time,perNum);
    return;
}

void onePerson(int A,int B,int C){
    int time,position,perNum;
    time=0;
    perNum=0;
    position=C;
    if(A>=0&&A<=10&&B>=0&&B<=10&&C>=0&&C<=10){
        myPrint(position,time,perNum);
        time+=abs(position-A);
        position=A;
        perNum++;
        myPrint(position,time,perNum);
        time+=abs(B-A);
        position=B;
        perNum--;
        myPrint(position,time,perNum);
    }else{
        printf("error");
    }
    return;
}

void onePerReFile(){
    int A,B,C;
    FILE* fp;
    if ((fp = fopen("onePerReFile.txt", "r")) == NULL)
    {
        printf("打开文件失败！\n");
        exit(EXIT_FAILURE);
    }
    fscanf(fp,"elevator %d\n%d %d\n",&C,&A,&B);
    onePerson(A,B,C);
    return;
}

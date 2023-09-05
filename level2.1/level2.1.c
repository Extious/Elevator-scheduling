#include <stdlib.h>
#include<stdio.h>
#include"level2.1.h"
#define MAXREQUEST 10
#define LINE_MAX 20
#define MAX_SIZE 10

typedef struct Elevator{
    int time;
    int position;
    int perNum;
    //电梯正在运行方向：1->up,0->down
    int status;
}Elevator;
/*
总人员记录
per[][0]:start_floor;
per[][1]:end_floor;
per[][2]:enter_time;
*/
int per[MAXREQUEST][3];
//总人数
int Num;
//电梯属性
Elevator e;
//每层上电梯性质:若该层存在请求则stayIn[floor]==1,否则为0
int stayIn[MAX_SIZE+1];
//每层下电梯性质
int stayOut[MAX_SIZE+1];
//总请求数
int reqNum;

//增加floor层进电梯请求
void enStayIn(int floor){
    stayIn[floor]=1;
    return;
}

//增加floor层出电梯请求
void enStayOut(int floor){
    stayOut[floor]=1;
    return;
}

//减去floor层进电梯请求
void deStayIn(int floor){
    stayIn[floor]=0;
    return;
}

//减去floor层出电梯请求
void deStayOut(int floor){
    stayOut[floor]=0;
    return;
}

//等待响应的最大楼层
int maxFloor(){
    int maxfloor=1;
    for(int i=10;i>=1;i--){
        if(stayIn[i]==1||stayOut[i]==1){
            maxfloor=i;
            break;
        }
    }
    return maxfloor;
}
//等待响应的最小楼层
int minFloor(){
    int minfloor=10;
    for(int i=1;i<=10;i++){
        if(stayIn[i]==1||stayOut[i]==1){
            minfloor=i;
            break;
        }
    }
    return minfloor;
}

void printStayIn(){
    for(int i=1;i<=10;i++){
        if(stayIn[i]==1){
            printf("floor for in:%d\n",i);
        }
    }
}

void printStayOut(){
    for(int i=1;i<=10;i++){
        if(stayOut[i]==1){
            printf("floor for out:%d\n",i);
        }
    }
}

void myPrint(int position,int time,int perNum){
    printf("%d %d %d\n",position,time,perNum);
    return;
}

int search(int floor){
    for(int i=0;i<Num;i++){
        if(per[i][0]==floor){
            return per[i][1];
        }
    }
    return 0;
}

void timeToReq(){
    for(int i=0;i<Num;i++){
        if(e.time==per[i][2]){
            enStayIn(per[i][0]);
            printf("at %d ,%d floor request for the elevator.\n",e.time,per[i][0]);
            reqNum++;
        }
    }
    return;
}

//主要电梯调度处理
void mainDeal(){
    //判断是否进入电梯
    int flag=1;
    e.time=0;
    e.perNum=0;
    e.status=1;
    myPrint(e.position,e.time,e.perNum);
    timeToReq();
    while(reqNum>0){
        e.status=0;
        for(;e.position>=minFloor();e.position--){
            printf("current position: %d\n",e.position);
            if(stayIn[e.position]==1){
                if(e.perNum<4){
                    e.perNum++;
                    deStayIn(e.position);
                    myPrint(e.position,e.time,e.perNum);
                    enStayOut(search(e.position));
                }
            }
            if(stayOut[e.position]==1){
                e.perNum--;
                deStayOut(e.position);
                myPrint(e.position,e.time,e.perNum);
                reqNum--;
            }
            e.time++;
            timeToReq();
        }
        e.position++;
        e.status=1;
        for(;e.position<=maxFloor();e.position++){
            printf("current position: %d\n",e.position);
            if(stayIn[e.position]==1){
                if(e.perNum<4){
                    e.perNum++;
                    deStayIn(e.position);
                    myPrint(e.position,e.time,e.perNum);
                    enStayOut(search(e.position));
                }
            }
            if(stayOut[e.position]==1){
                e.perNum--;
                deStayOut(e.position);
                myPrint(e.position,e.time,e.perNum);
                reqNum--;
            }
            e.time++;
            timeToReq();
        }
        e.position--;
    }
}

//读取文件内容并解析
void anyTimeReFile(){
    int start;
    int end;
    int enter_time;
    char buf[LINE_MAX];
    
    FILE* fp;
    if ((fp = fopen("anyTimeReFile.txt", "r")) == NULL)
    {
        printf("open file error!\n");
        exit(EXIT_FAILURE);
    }
    //读取第一行：elevator 1
    if (fgets(buf,LINE_MAX,fp)){
        sscanf(buf,"elevator %d",&e.position);
    }
    //读取其他行内容
    while(fgets(buf,LINE_MAX,fp)){
        sscanf(buf,"%d %d %d",&start,&end,&enter_time);
        per[Num][0]=start;
        per[Num][1]=end;
        per[Num][2]=enter_time;
        Num++;
    }
    printf("position:%d\n",e.position);
    for (int i=0;i<Num;i++){
        printf("person from %d to %d at %d\n",per[i][0],per[i][1],per[i][2]);
    }
    printf("the elevator begin running...\n");
    mainDeal();
    return;
}
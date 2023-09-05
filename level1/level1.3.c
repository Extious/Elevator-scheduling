#include <stdlib.h>
#include<stdio.h>
#include"level1.2.h"
#include"level1.3.h"

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

//总人员记录
int per[MAXREQUEST][2];
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

int search(int floor){
    for(int i=0;i<Num;i++){
        if(per[i][0]==floor){
            return per[i][1];
        }
    }
    return 0;
}

//主要电梯调度处理
void mainDeal(){
    //判断是否进入电梯
    int flag=1;
    e.time=0;
    e.perNum=0;
    e.status=1;
    myPrint(e.position,e.time,e.perNum);
    while(reqNum>0){
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
        }
        e.position--;
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
        }
        e.position++;
    }
}

//读取文件内容并解析
void sameTimeReFile(){
    int start;
    int end;
    char buf[LINE_MAX];
    
    FILE* fp;
    if ((fp = fopen("sameTimeReFile.txt", "r")) == NULL)
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
        sscanf(buf,"%d %d",&start,&end);
        per[Num][0]=start;
        per[Num][1]=end;
        Num++;
    }
    printf("position:%d\n",e.position);
    for (int i=0;i<Num;i++){
        printf("person from %d to %d\n",per[i][0],per[i][1]);
    }
    printf("the elevator begin running...\n");
    sameTimeDeal();
    return;
}

//进入电梯请求处理
void sameTimeDeal(){
    for(int i=0;i<Num;i++){
        enStayIn(per[i][0]);
        reqNum++;
    }
    printStayIn();
    mainDeal();
    return;
}
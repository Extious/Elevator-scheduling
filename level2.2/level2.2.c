#include <stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<windows.h>
#include<time.h>

#define MAXREQUEST 10
#define LINE_MAX 20
#define MAX_SIZE 10

pthread_mutex_t mutex;

typedef struct Elevator{
    int position;
    int perNum;
    //电梯正在运行方向：1->up,-1->down,0->free
    int status;
}Elevator;
/*
总人员记录
per[][0]:start_floor;
per[][1]:end_floor;
per[][2]:direction;
per[][3]:request_time;
*/
int per[MAXREQUEST][4];
//总人数
int Num;
//电梯属性
Elevator e1;
Elevator e2;
//每层上电梯性质:若该层存在请求则stayIn[floor]==1,否则为0
int stayIn1[MAX_SIZE+1];
int stayIn2[MAX_SIZE+1];
//每层下电梯性质
int stayOut1[MAX_SIZE+1];
int stayOut2[MAX_SIZE+1];
//总请求数
int reqNum1;
int reqNum2;
//时间
int mytime;
//控制run_e1和run_e2以此调度
int flag=1;

//将请求加入进电梯集合
void addStayIn1(int floor){
    stayIn1[floor]=1;
    printf("addStayIn1:%d\n",floor);
    reqNum1++;
    return;
}
void addStayIn2(int floor){
    stayIn2[floor]=1;
    printf("addStayIn2:%d\n",floor);
    reqNum2++;
    return;
}

//将请求加入出电梯集合
void addStayOut1(int floor){
    stayOut1[floor]=1;
    printf("addStayOut1:%d\n",floor);
    reqNum1++;
    return;
}
void addStayOut2(int floor){
    stayOut2[floor]=1;
    printf("addStayOut2:%d\n",floor);
    reqNum2++;
    return;
}

//减去某进电梯的请求
void deStayIn1(int floor){
    stayIn1[floor]=0;
    printf("deStayIn1:%d\n",floor);
    reqNum1--;
    return;
}
void deStayIn2(int floor){
    stayIn2[floor]=0;
    printf("deStayIn2:%d\n",floor);
    reqNum2--;
    return;
}

//减去某出电梯的请求
void deStayOut1(int floor){
    stayOut1[floor]=0;
    printf("deStayOut1:%d\n",floor);
    reqNum1--;
    return;
}
void deStayOut2(int floor){
    stayOut2[floor]=0;
    printf("deStayOut2:%d\n",floor);
    reqNum2--;
    return;
}

//等待响应的最大楼层
int maxFloor1(){
    int maxfloor=1;
    for(int i=10;i>=1;i--){
        if(stayIn1[i]==1||stayOut1[i]==1){
            maxfloor=i;
            break;
        }
    }
    return maxfloor;
}
int maxFloor2(){
    int maxfloor=1;
    for(int i=10;i>=1;i--){
        if(stayIn2[i]==1||stayOut2[i]==1){
            maxfloor=i;
            break;
        }
    }
    return maxfloor;
}
//等待响应的最小楼层
int minFloor1(){
    int minfloor=10;
    for(int i=1;i<=10;i++){
        if(stayIn1[i]==1||stayOut1[i]==1){
            minfloor=i;
            break;
        }
    }
    return minfloor;
}
int minFloor2(){
    int minfloor=10;
    for(int i=1;i<=10;i++){
        if(stayIn2[i]==1||stayOut2[i]==1){
            minfloor=i;
            break;
        }
    }
    return minfloor;
}

//标准输出格式：id position time perNum
void myPrint(int eNum,int position,int time,int perNum){
    printf("%d %d %d %d\n",eNum,position,time,perNum);
    return;
}

//根据start_floor查找出对应的end_floor
int search(int floor){
    for(int i=0;i<Num;i++){
        if(per[i][0]==floor){
            return per[i][1];
        }
    }
    return 0;
}

//判断符号是否相同
int IdentifySign(int i,int j){
    if(i>0&&j>0){
        return 1;
    }else if(i>0&&j<0){
        return 0;
    }else if(i<0&&j>0){
        return 0;
    }else if(i<0&&j<0){
        return 1;
    }
}

//根据符号判断电梯运行方向
int getStatus(int i){
    if(i<0){
        return -1;
    }else{
        return 1;
    }
}

//启动1号电梯
void *run_e1(void* args){
    while(1){
        while(flag!=1){
            Sleep(1);
        }
        pthread_mutex_lock(&mutex);
        printf("time = %d , reqNum1 = %d , reqNum2 = %d , enter run_e1:\n",mytime,reqNum1,reqNum2);
        if(reqNum1>0){
            printf("e1 begin to run one floor at %d and the direction is %d\n",e1.position,e1.status);
            switch(e1.status){
                case 1:
                if(e1.position<=maxFloor1()){
                    printf("e1 current position: %d\n",e1.position);
                    if(stayIn1[e1.position]==1){
                        if(e1.perNum<4){
                            e1.perNum++;
                            deStayIn1(e1.position);
                            myPrint(1,e1.position,mytime,e1.perNum);
                            addStayOut1(search(e1.position));
                        }
                    }
                    if(stayOut1[e1.position]==1){
                        e1.perNum--;
                        deStayOut1(e1.position);
                        myPrint(1,e1.position,mytime,e1.perNum);
                    }
                }
                mytime++;
                e1.position++;
                if(e1.position>maxFloor1()){
                    mytime--;
                    e1.position--;
                    e1.status=-e1.status;
                }
                break;
                case -1:
                if(e1.position>=minFloor1()){
                    printf("e1 current position: %d\n",e1.position);
                    if(stayIn1[e1.position]==1){
                        if(e1.perNum<4){
                            e1.perNum++;
                            deStayIn1(e1.position);
                            myPrint(1,e1.position,mytime,e1.perNum);
                            addStayOut1(search(e1.position));
                        }
                    }
                    if(stayOut1[e1.position]==1){
                        e1.perNum--;
                        deStayOut1(e1.position);
                        myPrint(1,e1.position,mytime,e1.perNum);
                    }
                }
                mytime++;
                e1.position--;
                if(e1.position<minFloor1()){
                    mytime--;
                    e1.position++;
                    e1.status=-e1.status;
                }
                break;
                case 0:
            }
        }else{
            e1.status=0;
        }
        pthread_mutex_unlock(&mutex);
        flag=2;
        Sleep(10);
    }
}

//启动2号电梯
void *run_e2(void* args){
    while(1){
        while(flag!=2){
            Sleep(1);
        }
        pthread_mutex_lock(&mutex);
        printf("time = %d , reqNum1 = %d , reqNum2 = %d , enter run_e2:\n",mytime,reqNum1,reqNum2);
        if(reqNum2>0){
            printf("e2 begin to run one floor at %d and the direction is %d\n",e2.position,e2.status);
            switch(e2.status){
                case 1:
                if(e2.position<=maxFloor2()){
                    printf("e2 current position: %d\n",e2.position);
                    if(stayIn2[e2.position]==1){
                        if(e2.perNum<4){
                            e2.perNum++;
                            deStayIn2(e2.position);
                            myPrint(2,e2.position,mytime,e2.perNum);
                            addStayOut2(search(e2.position));
                        }
                    }
                    if(stayOut2[e2.position]==1){
                        e2.perNum--;
                        deStayOut2(e2.position);
                        myPrint(2,e2.position,mytime,e2.perNum);
                    }
                }
                if(e1.status==0){
                    mytime++;
                }
                e2.position++;
                if(e2.position>maxFloor2()){
                    if(e1.status==0){
                        mytime--;
                    }
                    e2.position--;
                    e2.status=-e2.status;
                }
                break;
                case -1:
                if(e2.position>=minFloor2()){
                    printf("e2 current position: %d\n",e2.position);
                    if(stayIn2[e2.position]==1){
                        if(e2.perNum<4){
                            e2.perNum++;
                            deStayIn2(e2.position);
                            myPrint(2,e2.position,mytime,e2.perNum);
                            addStayOut2(search(e2.position));
                        }
                    }
                    if(stayOut2[e2.position]==1){
                        e2.perNum--;
                        deStayOut1(e2.position);
                        myPrint(2,e2.position,mytime,e2.perNum);
                    }
                }
                if(e1.status==0){
                    mytime++;
                }
                e2.position--;
                if(e2.position<minFloor2()){
                    if(e1.status==0){
                        mytime--;
                    }
                    e2.position++;
                    e2.status=-e2.status;
                }
                break;
                case 0:
            }
        }else{
            e2.status=0;
        }
        pthread_mutex_unlock(&mutex);
        flag=1;
        Sleep(10);
    }   
}

//启动电梯调度函数
void *elevatorController(void* args){
    int begin=0;
    while(1){
        pthread_mutex_lock(&mutex);
        printf("time = %d , reqNum1 = %d , reqNum2 = %d , enter elevatorController:\n",mytime,reqNum1,reqNum2);
        for(int i=begin;i<Num;i++){
            if(mytime==per[i][3]){//监测到有人在此时按电梯
                if(e1.status==0&&e2.status==0){//若两电梯都空闲
                    printf("the two elevators are free.\n");
                    if(abs(e1.position-per[i][0])<=abs(e2.position-per[i][0])){//判断哪个电梯距离请求者金
                        addStayIn1(per[i][0]);
                        e1.status=getStatus(per[i][0]-e1.position);
                    }else{
                        addStayIn2(per[i][0]);
                        e2.status=getStatus(per[i][0]-e2.position);
                    }
                }else if(e1.status==0){//若只有1号电梯空闲
                    printf("only the e1 is free.\n");
                    if(e2.status==per[i][2]){//判断2号电梯的方向和请求者方向是否相同
                        if(IdentifySign(e2.status,per[i][0]-e2.position)){//判断方向相同时是否可以不改变方向到达
                            addStayIn2(per[i][0]);
                        }else{
                            addStayIn1(per[i][0]);
                            e1.status=getStatus(per[i][0]-e1.position);
                        }
                    }else{
                        addStayIn1(per[i][0]);
                        e1.status=getStatus(i-e1.position);
                    }
                }else if(e2.status==0){//若只有2号电梯空闲
                    printf("only the e2 is free.\n");
                    if(e1.status==per[i][2]){
                        if(IdentifySign(e1.status,per[i][0]-e1.position)){
                            addStayIn1(per[i][0]);
                        }else{
                            addStayIn2(per[i][0]);
                            e2.status=getStatus(i-e2.position);
                        }
                    }else{
                        addStayIn2(per[i][0]);
                        e2.status=getStatus(i-e2.position);
                    }
                }else{//若都不空闲
                    if(e1.status==per[i][2]){//判断1号电梯的方向和请求者方向是否相同
                        if(IdentifySign(e1.status,per[i][0]-e1.position)){//判断方向相同时是否可以不改变方向到达
                            addStayIn1(per[i][0]);
                        }
                    }else if(e2.status==per[i][2]){//判断2号电梯的方向和请求者方向是否相同
                        if(IdentifySign(e2.status,per[i][0]-e2.position)){//判断方向相同时是否可以不改变方向到达
                            addStayIn2(per[i][0]);
                        }
                    }else{
                        printf("no elevator is free.\n");
                    }
                }
                begin=i+1;
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
        Sleep(10);
    }
}

//读取文件内容并解析
void twoDealReFile(){
    int start;
    int end;
    int direction;
    int request_time;
    char buf[LINE_MAX];
    FILE* fp;
    if ((fp = fopen("twoElevatorDealFile.txt", "r")) == NULL)
    {
        printf("open file error！\n");
        exit(EXIT_FAILURE);
    }
    //读取第一行：elevator 1
    if (fgets(buf,LINE_MAX,fp)){
        sscanf(buf,"elevator1 %d",&e1.position);
    }
    if (fgets(buf,LINE_MAX,fp)){
        sscanf(buf,"elevator2 %d",&e2.position);
    }
    //读取其他行内容
    while(fgets(buf,LINE_MAX,fp)){
        sscanf(buf,"%d %d %d %d",&start,&end,&direction,&request_time);
        per[Num][0]=start;
        per[Num][1]=end;
        per[Num][2]=direction;
        per[Num][3]=request_time;
        printf("per:%d %d %d %d\n",per[Num][0],per[Num][1],per[Num][2],per[Num][3]);
        Num++;
    }
    for (int i=0;i<Num;i++){
        if(per[i][2]==1){
            printf("person up from %d to %d at %d\n",per[i][0],per[i][1],per[i][3]);
        }else if(per[i][2]==-1){
            printf("person down from %d to %d at %d\n",per[i][0],per[i][1],per[i][3]);
        }
    }
    printf("elevator begin running:\n");
    return;
}

int main(){
    twoDealReFile();
    pthread_mutex_init(&mutex,NULL);
    pthread_t t1;
	pthread_t t2;
    pthread_t t3;
	pthread_create(&t1, NULL, elevatorController, NULL);
	pthread_create(&t2, NULL, run_e1, NULL);
    pthread_create(&t3, NULL, run_e2, NULL);
    getchar();
	return 0;
}
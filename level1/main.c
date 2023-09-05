#include <stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<windows.h>
#include"level1.2.h"
#include"level1.3.h"

int main(int argc, char * argv[]){
    if(argc>1){
        if(strcmp("level1.3",argv[1])==0){
            sameTimeReFile();
        }
        if(strcmp("level1.2",argv[1])==0){
            onePerReFile();
        }
    }else{
        printf("please stdin the level\n");
    }
    return 0;
}
#ifndef _DATA_STRUCT_
#define _DATA_STRUCT_

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "globalVar.h"

typedef struct employee
{
    char firstName[NAME_LEN];
    char lastName[NAME_LEN];
    int empId;
    char contactNo[MOB_NO_LEN];
    float experience;
    bool projectAllocated;
    int orderSeq;
}employee;

typedef struct queryString
{
    char query[MAX_ARR_SIZE];

}queryString;

typedef struct queryResult
{
    char result[MAX_ARR_SIZE];

}queryResult;

typedef struct clientInfo
{
    pid_t ClientPID;
    char userName[USER_NAME_LEN];
    char password[USER_PASS_LEN];

}clientInfo;

typedef struct serverAck
{
    char msg[MAX_ARR_SIZE];
}serverAck;

typedef struct watchDog
{
    pthread_t tid;
    int msgId;
    int key;
    time_t lstCmdSent; //last command response sent to the client
    struct watchDog *next;
}watchDog;


typedef union data
{
    employee Employee;                      //EMP
    clientInfo ClientInfo;                  //CLIENT_INFO
    queryResult QueryResult;                //QUERY
    queryString QueryString;                //QUERY_STRING
    serverAck ServerAck;                    //SERVER_ACK
    watchDog WatchDog;
}data;

typedef struct dataPack
{
    data Data;
    int structId;                           //EMP or CLIENT_INFO or QUERY or SERVER_ACK
}dataPack;

typedef struct msgPacket
{
    dataPack DataPack;
    int batchSize;
    char endOfPacket;

}msgPacket;

typedef struct msgQueue
{
    long msgType;
    msgPacket msgPk;

}msgQueue;

typedef struct threadArg
{
    clientInfo *ClientInfo;
    msgQueue *MsgQueue;
}threadArg;

typedef struct node
{
    dataPack *DataPack;
    struct node *next;
}node;

dataPack *_WATCH_DOC_ELE_HEAD_;


#endif //_DATA_STRUCT_
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
#include <signal.h>
 

#include "globalVar.h"
#include "helpingFunc.h"


typedef struct employee
{
    char firstName[NAME_LEN];
    char lastName[NAME_LEN];
    int empId;
    char contactNo[MOB_NO_LEN];
    float experience;
    bool projectAllocated;
    int orderSeq;
    char PK[PK_SIZE];
    char skillSet[NO_OF_SKILLS][SKILLS_LEN];
}employee;

typedef struct queryString
{
    char query[MAX_ARR_SIZE];
    employee Employee;                      //EMP
}queryString;

typedef struct queryResult
{
    char result[MAX_ARR_SIZE];
    employee Employee;                      //EMP

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
    int clientMsgId;
    int serverMsgId;
    int clientKey;
    int serverKey;
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
    bool blank;
    time_t timeStamp;
    int structId;                           //EMP or CLIENT_INFO or QUERY or SERVER_ACK
}dataPack;

typedef struct msgPacket
{
    dataPack DataPack;
    bool batchData;
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
    int serverMsgId;
    watchDog WatchDog;
    msgQueue *MsgQueue;
}threadArg;

typedef struct node
{
    dataPack *DataPack;
    struct node *next;
}node;

node *_WATCH_DOC_ELE_HEAD_ = NULL;
volatile int _SERVER_MSG_ID_;
node *_EMP_DB_DATA_LIST_ = NULL;
volatile int _LAST_EMP_ID_=0;



#endif //_DATA_STRUCT_
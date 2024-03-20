#ifndef _DATA_STRUCT_
#define _DATA_STRUCT_

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define KEY 123
#define NAME_LEN 100
#define MOB_NO_LEN 11
#define MAX_BATCH_SIZE 100
#define MAX_ARR_SIZE 100
#define USER_NAME_LEN 10
#define USER_PASS_LEN 10

#define SERVER_CONNECTED "OK"
#define SERVER_NOT_CONNECTED "NOK"
#define SERVER_UNKNOWN_ERROR "FAILED"

enum enumMsgType{CLIENT_START=1};
enum enumStructID{EMP_INFO,CLIENT_INFO,QUERY_RESULT,QUERY_STRING,SERVER_ACK};
enum enumServerAck{OK,ERROR};

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

typedef struct msgPacket
{
    employee Employee;                            //EMP
    clientInfo ClientInfo;                   //CLIENT_INFO
    queryResult QueryResult;                //QUERY
    queryString QueryString;                //QUERY_STRING
    serverAck ServerAck;                    //SERVER_ACK
    int structId;                           //EMP or CLIENT_INFO or QUERY or SERVER_ACK
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
    employee *emp;
    struct node *next;
}node;

#endif //_DATA_STRUCT_
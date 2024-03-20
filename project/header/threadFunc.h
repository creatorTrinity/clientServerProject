#ifndef _THREAD_FUNC_H_
#define _THREAD_FUNC_H_

#include "dataStruct.h"

void prepareClientResponse(char *msg,char *query)
{
    strcpy(msg,query);
}

void *threadWatchDog(void *arg)
{

    return NULL;
}
void *myThreadFunc(void *arg)
{
    threadArg *ThreadArgs = (threadArg *)arg;
    int msgRet;
    int msgId;
    int key;
    char result[MAX_ARR_SIZE];

    msgQueue mq;
    msgPacket MsgPack;

    employee Employee;
    clientInfo ClientInfo;
    queryResult QueryResult;
    queryString QueryString;
    serverAck ServerAck;

    //key = ThreadArgs->ClientInfo->ClientPID;
    unsigned long int threadId = pthread_self();
    printf(" thread id = %lu\n",threadId);
    printf(" Started serving the client id = %d\n",ThreadArgs->ClientInfo->ClientPID);

    strcpy(ServerAck.msg, SERVER_CONNECTED);
    MsgPack.ServerAck = ServerAck;
    MsgPack.structId = SERVER_ACK;
    MsgPack.endOfPacket = 0;
    mq.msgType = CLIENT_START;
    mq.msgPk = MsgPack;
    printf(" sending ACK message from thread id = %lu\n", threadId);
    msgRet = msgsnd(msgId, &mq, sizeof(mq.msgPk ), 0);
    if(msgRet==-1)
    {
        perror("myThreadFunc:::msg Send error: ");
        printf("Server msg not send\n");
        return NULL;
    }

    key = ThreadArgs->ClientInfo->ClientPID;
    printf("server Thread creating new message queue with Id =%d\n",key);
    msgId = msgget(key,0666 | IPC_CREAT);

    while(1)
    {
        MsgPack.QueryString = QueryString;
        MsgPack.structId = QUERY_STRING;
        MsgPack.endOfPacket = 0;
        mq.msgType = key;
        mq.msgPk = MsgPack;

        if( msgrcv(msgId ,&mq, sizeof(mq.msgPk), 0, 0 ) != -1 )
        {
            printf("mq.msgPk.structId = %d\n",mq.msgPk.structId);
            if(mq.msgPk.structId == EMP_INFO )
            {
                Employee = (mq.msgPk.Employee);
                printf(" EMP ID = %d\n",Employee.empId);
            }
            else if(mq.msgPk.structId == CLIENT_INFO )
            {
                ClientInfo = (mq.msgPk.ClientInfo);
                printf(" client PID = %d\n",ClientInfo.ClientPID);
            }
            else if(mq.msgPk.structId == QUERY_RESULT )
            {
                QueryResult = (mq.msgPk.QueryResult);
                printf(" client QueryResult = %s\n",QueryResult.result);
            }
            else if(mq.msgPk.structId == QUERY_STRING )
            {
                QueryString = (mq.msgPk.QueryString);
                if(strcmp(mq.msgPk.QueryString.query,"exit") == 0 )
                {
                    printf("thread received client EXIT request\n");
                    break;
                }
                else
                {
                    printf(" Thread received  client QueryString = %s\n",QueryString.query);
                }
            }
            else
            {
                printf("WHILE myThreadFunc:::Server ignoring Bad request from client\n");
                printf("Ignoring the request\n");
            }

            /* This function will create the query response of a client*/
            prepareClientResponse(result,QueryString.query);
            /* This function will create the query response of a client*/
            strcpy(QueryResult.result,result);


            MsgPack.QueryResult = QueryResult;
            MsgPack.structId = QUERY_RESULT;
            MsgPack.endOfPacket = 0;
            mq.msgType = key;
            mq.msgPk = MsgPack;


            msgRet = msgsnd(msgId, &mq, sizeof(mq.msgPk ), 0);
            if(msgRet==-1)
            {
                perror("msg Send error: ");
                printf("Server msg not send\n");
                break;
            }
        }
        else
        {
            perror("Message receive Error :");
            break;
        }
    }
    printf("Thread <%lu> has finished it's work \n",threadId);
    pthread_exit(NULL);
}

pthread_t assignWorkToThreads(clientInfo *ClientInfo)
{
    pthread_t tid;
    int ret;
    threadArg *ThreadArgs = (threadArg *)malloc(sizeof (ThreadArgs));
    ThreadArgs->ClientInfo = ClientInfo;
    ThreadArgs->MsgQueue = (msgQueue *)malloc(sizeof (msgQueue));
    ThreadArgs->MsgQueue->msgType = ClientInfo->ClientPID;

    ret = pthread_create(&tid, NULL, myThreadFunc, (void*)ThreadArgs);
    if( ret == -1 )
    {
        printf("Fail to create thread, Exiting...\n");
    }
    return tid;
}


#endif //_THREAD_FUNC_H_
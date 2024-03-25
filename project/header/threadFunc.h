#ifndef _THREAD_FUNC_H_
#define _THREAD_FUNC_H_

#include "dataStruct.h"

void prepareClientResponse(char *msg,char *query)
{
    strcpy(msg,query);
}

void updateTimeToThread(threadArg **ThreadArgs)
{
    /*this part will be updated everytime if the server thread communicate with client*/
    time(&currentTime);
    (*ThreadArgs)->WatchDog.lstCmdSent = currentTime;
    updateWatchDog((*ThreadArgs));
    /*this part will be updated everytime the thread communicate woth client*/
}
void updateWatchDog(threadArg *ThreadArgs)
{
    dataPack *DataPack = (dataPack *)malloc(sizeof (dataPack));
    DataPack->Data.WatchDog = ThreadArgs->WatchDog;
    addNode(&_WATCH_DOC_ELE_HEAD_,DataPack);
}
void *threadWatchDog(void *arg)
{
    /*time_t currentTime;
    time(&currentTime);
    int ret;
    watchDog *WatchDog;
    while(1)
    {
        for( int i=0; i < MAX_CLIENT_CONNECTION; i++)
        {
            WatchDog = _WATCH_DOC_THREAD_ELEMENT_[i];
            if( currentTime - WatchDog.lstCmdSent > CONNECTION_TIME_OUT_SECONDS )
            {
                printf("main(): sending cancellation request\n");
                ret = pthread_cancel(WatchDog.tid);
                if (ret != 0)
                {
                    handle_error_en(s, "pthread_cancel");
                }
                else
                {
                    free(WatchDog);
                    WatchDog = NULL;
                }
            }
        }
        printf("\nwatch DOG is going for sleep\n");
        sleep(CONNECTION_TIME_OUT_SECONDS);
    }*/
    return NULL;
}
void *myThreadFunc(void *arg)
{
    threadArg *ThreadArgs = (threadArg *)arg;
    time_t currentTime;
    int msgRet;
    int msgId;
    int key;
    char result[MAX_ARR_SIZE];
    watchDog *WatchDog;

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
    MsgPack.DataPack.Data.ServerAck = ServerAck;
    MsgPack.DataPack.structId = SERVER_ACK;
    MsgPack.endOfPacket = 0;
    mq.msgType = CLIENT_START;
    mq.msgPk = MsgPack;
    msgId = msgget(KEY,0666 | IPC_CREAT);
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

    /*updating the watch Dog attributes in the linked list _WATCH_DOC_ELE_HEAD_*/
    ThreadArgs->WatchDog.tid = threadId;
    ThreadArgs->WatchDog.clientMsgId = msgId;
    ThreadArgs->WatchDog.serverMsgId = ThreadArgs.serverMsgId;
    ThreadArgs->WatchDog.clientKey = key;
    ThreadArgs->WatchDog.serverKey = KEY;

    /*this part will be updated everytime if the server thread communicate with client*/
    updateTimeToThread(&hreadArgs);
    /*this part will be updated everytime if the server thread communicate with client*/

    while(1)
    {
        MsgPack.DataPack.Data.QueryString = QueryString;
        MsgPack.DataPack.structId = QUERY_STRING;
        MsgPack.endOfPacket = 0;
        mq.msgType = key;
        mq.msgPk = MsgPack;

        if( msgrcv(msgId ,&mq, sizeof(mq.msgPk), 0, 0 ) != -1 )
        {
            printf("mq.msgPk.structId = %d\n",mq.msgPk.DataPack.structId);

            if(mq.msgPk.DataPack.structId == EMP_INFO )
            {
                Employee = (mq.msgPk.DataPack.Data.Employee);
                printf(" EMP ID = %d\n",Employee.empId);
            }
            else if(mq.msgPk.DataPack.structId == CLIENT_INFO )
            {
                ClientInfo = (mq.msgPk.DataPack.Data.ClientInfo);
                printf(" client PID = %d\n",ClientInfo.ClientPID);
            }
            else if(mq.msgPk.DataPack.structId == QUERY_RESULT )
            {
                QueryResult = (mq.msgPk.DataPack.Data.QueryResult);
                printf(" client QueryResult = %s\n",QueryResult.result);
            }
            else if(mq.msgPk.DataPack.structId == QUERY_STRING )
            {
                QueryString = (mq.msgPk.DataPack.Data.QueryString);
                if(strcmp(mq.msgPk.DataPack.Data.QueryString.query,"exit") == 0 )
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


            MsgPack.DataPack.Data.QueryResult = QueryResult;
            MsgPack.DataPack.structId = QUERY_RESULT;
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
            /*this part will be updated everytime if the server thread communicate with client*/
            updateTimeToThread(&hreadArgs);
            /*this part will be updated everytime if the server thread communicate with client*/
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

pthread_t assignWorkToThreads(int serverMsgId, clientInfo *ClientInfo)
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
        return ret;
    }
    return tid;
}


#endif //_THREAD_FUNC_H_
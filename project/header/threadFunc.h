#ifndef _THREAD_FUNC_H_
#define _THREAD_FUNC_H_

#include "dataStruct.h"
#include "linkedList.h"
#include "fileRW.h"
#include "sortBubble.h"

void prepareClientResponse(char *msg,char *query)
{
    strcpy(msg,query);
    sortLinkedList(&_EMP_DB_DATA_LIST_,query);
    printLinkedList(_EMP_DB_DATA_LIST_);
}

void updateWatchDog(threadArg *ThreadArgs)
{
    dataPack *DataPack = (dataPack *)malloc(sizeof (dataPack));
    DataPack->Data.WatchDog = ThreadArgs->WatchDog;
    addNode(&_WATCH_DOC_ELE_HEAD_,DataPack);
}

void updateTimeToThread(threadArg **ThreadArgs)
{
    time_t currentTime;
    /*this part will be updated everytime if the server thread communicate with client*/
    time(&currentTime);
    (*ThreadArgs)->WatchDog.lstCmdSent = currentTime;
    updateWatchDog(*ThreadArgs);
    /*this part will be updated everytime the thread communicate woth client*/
}

void *threadWatchDog()
{
    time_t currentTime;

    int ret;
    watchDog WatchDog;
    node *p,*q,*r;

    while (1)
    {
        p = _WATCH_DOC_ELE_HEAD_;
        if( p == NULL)
        {
            printf("\nwatch DOG is going for sleep no node \n");
            sleep(WATCHDOG_SLEEP_SECONDS);
            continue;
        }

        while(p != NULL)
        {
            time(&currentTime);
            if( p->DataPack == NULL )
            {
                break;
            }

            WatchDog = p->DataPack->Data.WatchDog;
            printf("currentTime - WatchDog.lstCmdSent = %ld\n",currentTime - WatchDog.lstCmdSent);
            if( (currentTime - WatchDog.lstCmdSent ) > CONNECTION_TIME_OUT_SECONDS )
            {
                printf("main(): sending cancellation request\n");
                ret = pthread_cancel(WatchDog.tid);
                if (ret != 0)
                {
                    perror( "Thread pthread_cancel ERROR!!! \n");
                }
                else
                {
                    removeWatchDogNode(&_WATCH_DOC_ELE_HEAD_,p);
                    printf("Thread cancellation request success %lu\n",WatchDog.tid);
                }
            }
            p = p->next;
        }
        printf("\nwatch DOG is going for sleep\n");
        sleep(WATCHDOG_SLEEP_SECONDS);
    }
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
    time(&currentTime);
    MsgPack.DataPack.timeStamp = currentTime;
    MsgPack.DataPack.structId = SERVER_ACK;
    MsgPack.endOfPacket = 0;
    mq.msgType = CLIENT_START;
    mq.msgPk = MsgPack;
    //msgId = msgget(KEY,0666 | IPC_CREAT);
    printf(" sending ACK message from thread id = %lu\n", threadId);

    key = ThreadArgs->ClientInfo->ClientPID;
    printf("server Thread creating new message queue with Id =%d\n",key);
    msgId = msgget(key,0666 | IPC_CREAT);

    msgRet = msgsnd(msgId, &mq, sizeof(mq.msgPk ), 0);

    if(msgRet==-1)
    {
        perror("myThreadFunc:::msg Send error: ");
        printf("Server msg not send\n");
        return NULL;
    }



    /*updating the watch Dog attributes in the linked list _WATCH_DOC_ELE_HEAD_*/
    ThreadArgs->WatchDog.tid = threadId;
    ThreadArgs->WatchDog.clientMsgId = msgId;
    ThreadArgs->WatchDog.serverMsgId = ThreadArgs->serverMsgId;
    ThreadArgs->WatchDog.clientKey = key;
    ThreadArgs->WatchDog.serverKey = KEY;

    /*this part will be updated everytime if the server thread communicate with client*/
    //updateTimeToThread(&ThreadArgs);
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
            time(&currentTime);
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
                    printf("thread received client ( %d ) EXIT request\n",key);
                    break;
                }
                else
                {
                    printf(" Thread received  client ( %d ) QueryString = %s\n",key, QueryString.query);
                }
            }
            else
            {
                printf("WHILE myThreadFunc:::Server ignoring Bad request from client\n");
                printf("Ignoring the request\n");

                /*strcpy(ServerAck.msg, SERVER_PCK_ERR);
                MsgPack.DataPack.Data.ServerAck = ServerAck;
                time(&currentTime);
                MsgPack.DataPack.timeStamp = currentTime;
                MsgPack.DataPack.structId = SERVER_ACK;
                MsgPack.endOfPacket = 0;
                mq.msgType = CLIENT_START;
                mq.msgPk = MsgPack;
                //msgId = msgget(KEY,0666 | IPC_CREAT);
                printf(" sending ACK message from thread id = %lu\n", threadId);

                msgRet = msgsnd(msgId, &mq, sizeof(mq.msgPk ), 0);
                if(msgRet==-1)
                {
                    perror("WHILE myThreadFunc:::msg Send error: ");
                    printf("WHILE myThreadFunc:::Server msg not send\n");
                    break;
                }*/
                continue;
            }

            /* This function will create the query response of a client*/
            prepareClientResponse(result,QueryString.query);
            /* This function will create the query response of a client*/
            strcpy(QueryResult.result,result);

            time(&currentTime);
            MsgPack.DataPack.timeStamp = currentTime;
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
            //updateTimeToThread(&ThreadArgs);
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

pthread_t assignWorkToWatchDog()
{
    pthread_t tid;
    int ret;

    ret = pthread_create(&tid, NULL, threadWatchDog, NULL);
    if( ret == -1 )
    {
        printf("Fail to create Watch Dog thread, Exiting...\n");
        return ret;
    }
    return tid;
}

pthread_t assignWorkToThreads(int serverMsgId, clientInfo *ClientInfo)
{
    pthread_t tid;
    int ret;
    threadArg *ThreadArgs = (threadArg *)malloc(sizeof (ThreadArgs));
    ThreadArgs->ClientInfo = ClientInfo;
    ThreadArgs->MsgQueue = (msgQueue *)malloc(sizeof (msgQueue));
    ThreadArgs->MsgQueue->msgType = ClientInfo->ClientPID;
    ThreadArgs->serverMsgId = serverMsgId;

    ret = pthread_create(&tid, NULL, myThreadFunc, (void*)ThreadArgs);
    if( ret == -1 )
    {
        printf("Fail to create thread, Exiting...\n");
        return ret;
    }
    return tid;
}


#endif //_THREAD_FUNC_H_
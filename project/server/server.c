#include "../header/dataStruct.h"
#include "../header/threadFunc.h"

void ServerAck(char msg[],int ack)
{
    strcpy(msg,"SERVER_CONNECTED");
    if(ack == OK)
        strcpy(msg,SERVER_CONNECTED);
    else if(ack == ERROR)
        strcpy(msg,SERVER_NOT_CONNECTED);
    else
        strcpy(msg,SERVER_UNKNOWN_ERROR);
}

void freeServerResources()
{
    /*Destroying the message queue initially*/
    printf("destroying message queue ( id = %d )\n",_SERVER_MSG_ID_);
    msgctl(_SERVER_MSG_ID_, IPC_RMID, NULL);
    /*Destroying the message queue initially*/
    /*need to release all resources of server allocated during execution*/
}

void handle_sigint(int sig)
{
    printf("Caught signal %d\n",sig);
    freeServerResources();
}

int init()
{
    int ret = 0;
    pthread_t tid;

    //signal(SIGINT,handle_sigint);
    //tid = assignWorkToWatchDog();

    if( tid == -1)
    {
        return ret;
    }
    else
    {
        return ret;
    }
}



int main()
{
    int serverMsgId;
    int ret = 0;
    time_t currentTime;
    msgQueue mq;
    clientInfo ClientInfo;
    serverMsgId = msgget(KEY,0666 | IPC_CREAT);
    mq.msgType = CLIENT_INFO;
    _SERVER_MSG_ID_ = serverMsgId;
    printf("server message id = %d\n",_SERVER_MSG_ID_);

    //ret = init();
    //data insert into empDB
    fileInsertData();
    //fileReadData();
    empDBLinkedList();
    //printLinkedList();


    if( ret == -1)
    {
        printf("Server INIT process failed!!! Exiting server\n");
        return ret;
    }

    time(&currentTime);
    sleep(4);
    printf("\n!!!Server started!!\n");
    while(1)
    {

        if( msgrcv(serverMsgId ,&mq, sizeof(mq.msgPk), 0, 0 ) != -1 )
        {
            printf("currentTime =%lu\n",currentTime);
            printf("mq.msgPk.DataPack.timeStamp =%lu\n",mq.msgPk.DataPack.timeStamp);
            if(mq.msgPk.DataPack.timeStamp < currentTime)
            {
                printf("the Old message is ignored\n");
                continue;
            }
            printf("qqqqqqqqq ==== mq.msgPk.structId = %d\n",mq.msgPk.DataPack.structId);
            if(mq.msgType == CLIENT_START && mq.msgPk.DataPack.structId == CLIENT_INFO)
            {
                ClientInfo = (mq.msgPk.DataPack.Data.ClientInfo);
                printf(" client ID = %d\n",ClientInfo.ClientPID);

                assignWorkToThreads(serverMsgId,&ClientInfo);
            }
            else
            {
                printf("Server ignoring Bad request from client\n");
                printf("Ignoring the request\n");
            }
        }
        else
        {
            perror("Message receive Error :");
            break;
        }
    }
    //msgctl(msgId, IPC_RMID, NULL);
    return 0;
}


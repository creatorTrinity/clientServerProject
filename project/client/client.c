#include "../header/dataStruct.h"

void serverConnect( int clientMsgId, int clientPID,char *serverResponse)
{
    int msgRet;
    int msgId = clientMsgId;
    int key = clientPID;
    char clientQueryMsg[MAX_ARR_SIZE];
    msgQueue mq;
    clientInfo ClientInfo;
    queryString QueryString;
    msgPacket MsgPack;
    queryResult QueryResult;
    char ch;
    time_t currentTime;



    while(1)
    {
        /*start taking input from client and send the request to the server*/

        printf("Please enter the query for server\n");
        scanf("%s",clientQueryMsg);

        strcpy(QueryString.query,clientQueryMsg);
        /*start taking input from client and send the request to the server*/

        MsgPack.DataPack.Data.QueryString = QueryString;
        MsgPack.DataPack.structId = QUERY_STRING;
        MsgPack.endOfPacket = 0;
        mq.msgType = key;
        mq.msgPk = MsgPack;
        time(&currentTime);
        MsgPack.DataPack.timeStamp = currentTime;
        printf("\ncurrentTime =%lu\n",currentTime);
        msgRet = msgsnd(msgId, &mq, sizeof(mq.msgPk ), 0);
        if(msgRet==-1)
        {
            perror("msg Send error: ");
            printf("Client msg not send\n");
            strcpy(serverResponse,"Client msg not send");
            msgctl(msgId, IPC_RMID, NULL);
            return;
        }

        if( strcmp(clientQueryMsg,"exit") == 0 )
        {
            break;
        }

        MsgPack.DataPack.Data.QueryResult = QueryResult;
        MsgPack.DataPack.structId = QUERY_RESULT;
        MsgPack.endOfPacket = 0;
        mq.msgType = key;
        mq.msgPk = MsgPack;

        msgRet = msgrcv(msgId ,&mq, sizeof(mq.msgPk), 0, 0 );

        if( msgRet !=-1 )
        {
            printf("mq.msgPk.DataPack.structId = %d\n", mq.msgPk.DataPack.structId);
            if(mq.msgPk.DataPack.structId == QUERY_RESULT )
            {
                QueryResult = (mq.msgPk.DataPack.Data.QueryResult);
                printf(" client QueryResult = %s\n",QueryResult.result);
            }
            else if(mq.msgPk.DataPack.structId == SERVER_ACK)
            {
                /*DO resend the packet again*/
            }
            else
            {
                printf("\n\n !!!client not connected!!!\n\n");
                //printf("press 'c' for reconnection with server\n");
                break;
            }
        }
    }
}
int main()
{
    int msgId;
    int msgRet;
    int key;
    int clientPID;
    time_t currentTime;

    msgQueue mq;
    clientInfo ClientInfo;
    serverAck ServerAck;
    msgPacket MsgPack;
    char serverResponse[10];

    clientPID = ClientInfo.ClientPID = getpid();
    printf("Client PID = %d \n",ClientInfo.ClientPID);
    MsgPack.DataPack.Data.ClientInfo = ClientInfo;
    time(&currentTime);
    MsgPack.DataPack.timeStamp = currentTime;
    MsgPack.DataPack.structId = CLIENT_INFO;
    MsgPack.endOfPacket = 0;
    mq.msgType = CLIENT_START;
    mq.msgPk = MsgPack;

    printf("\ncurrentTime =%lu\n",currentTime);

    msgId = msgget(KEY,0666 | IPC_CREAT);
    printf("msgId = %d \n",msgId);

    msgRet = msgsnd(msgId, &mq, sizeof(mq.msgPk ), 0);
    if(msgRet==-1)
    {
        perror("msg Send error: ");
        printf("Client msg not send\n");
    }

    key = clientPID;
    printf("Client creating new message queue with Id =%d\n",key);
    msgId = msgget(key,0666 | IPC_CREAT);

    if( msgrcv(msgId ,&mq, sizeof(mq.msgPk), 0, 0 )!=-1 )
    {
        ServerAck = (mq.msgPk.DataPack.Data.ServerAck);
        printf(" Server ACK = %s\n",ServerAck.msg);
        if(strcmp(ServerAck.msg,SERVER_CONNECTED) == 0 )
        {
            printf("server connected successfully\n");
            serverConnect(msgId, clientPID, serverResponse);
        }
        else if(strcmp(ServerAck.msg,SERVER_NOT_CONNECTED) == 0 )
        {
            /*MAY BE AUTHENTICATION ERROR*/
            printf("Authentication error at server response = %s \n",ServerAck.msg);

        }
        else if(strcmp(ServerAck.msg,SERVER_UNKNOWN_ERROR) == 0 )
        {

        }
    }
    else
    {
        perror("Message receive Error :");
    }
}



#include "../header/dataStruct.h"

void serverConnect( int clientPID,char *serverResponse)
{
    int msgRet;
    int msgId;
    int key;
    char clientQueryMsg[MAX_ARR_SIZE];
    msgQueue mq;
    clientInfo ClientInfo;
    queryString QueryString;
    msgPacket MsgPack;
    queryResult QueryResult;

    key = clientPID;
    printf("Client creating new message queue with Id =%d\n",key);
    msgId = msgget(key,0666 | IPC_CREAT);

    while(1)
    {
        /*start taking input from client and send the request to the server*/

        printf("Please enter the query for server\n");
        scanf("%s",clientQueryMsg);

        strcpy(QueryString.query,clientQueryMsg);
        /*start taking input from client and send the request to the server*/

        MsgPack.QueryString = QueryString;
        MsgPack.structId = QUERY_STRING;
        MsgPack.endOfPacket = 0;
        mq.msgType = key;
        mq.msgPk = MsgPack;

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

        MsgPack.QueryResult = QueryResult;
        MsgPack.structId = QUERY_RESULT;
        MsgPack.endOfPacket = 0;
        mq.msgType = key;
        mq.msgPk = MsgPack;

        msgRet = msgrcv(msgId ,&mq, sizeof(mq.msgPk), 0, 0 );

        if( msgRet !=-1 )
        {
            if(mq.msgPk.structId == QUERY_RESULT )
            {
                QueryResult = (mq.msgPk.QueryResult);
                printf(" client QueryResult = %s\n",QueryResult.result);
            }
            else
            {
                printf(" client result not defined\n");
            }
        }
    }
}
int main()
{

    int msgId;
    int msgRet;
    int clientPID;
    msgQueue mq;
    clientInfo ClientInfo;
    serverAck ServerAck;
    msgPacket MsgPack;
    char serverResponse[10];

    clientPID = ClientInfo.ClientPID = getpid();
    printf("Client PID = %d \n",ClientInfo.ClientPID);
    MsgPack.ClientInfo = ClientInfo;
    MsgPack.structId = CLIENT_INFO;
    MsgPack.endOfPacket = 0;
    mq.msgType = CLIENT_START;
    mq.msgPk = MsgPack;



    msgId = msgget(KEY,0666 | IPC_CREAT);
    printf("msgId = %d \n",msgId);
    while(1)
    {
        msgRet = msgsnd(msgId, &mq, sizeof(mq.msgPk ), 0);
        if(msgRet==-1)
        {
            perror("msg Send error: ");
            printf("Client msg not send\n");
            break;
        }


        if( msgrcv(msgId ,&mq, sizeof(mq.msgPk), 0, 0 )!=-1 )
        {
            ServerAck = (mq.msgPk.ServerAck);
            printf(" Server ACK = %s\n",ServerAck.msg);
            if(strcmp(ServerAck.msg,SERVER_CONNECTED) == 0 )
            {
                printf("server connected successfully\n");
                serverConnect(clientPID,serverResponse);
                break;
            }
            else if(strcmp(ServerAck.msg,SERVER_NOT_CONNECTED) == 0 )
            {
                /*MAY BE AUTHENTICATION ERROR*/
                printf("unknown error at server response = %s \n",ServerAck.msg);
                continue;
            }
            else if(strcmp(ServerAck.msg,SERVER_UNKNOWN_ERROR) == 0 )
            {
                break;
            }
        }
        else
        {
            perror("Message receive Error :");
            break;
        }
        getchar();
    }
}



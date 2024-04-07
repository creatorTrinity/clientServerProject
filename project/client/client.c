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
        printf("\n\n!!!Please choose the below options!!!\n");
        printf("\n---------------------------------------------------\n");
        printf("\nEnter 1.1 ->> to sort the Emp records by first name");
        printf("\nEnter 1.2 ->> to sort the Emp records by last name");
        printf("\nEnter 1.3 ->> to sort the Emp records by emp ID");
        printf("\nEnter 1.4 ->> to sort the Emp records by year of experience");
        printf("\nEnter 2.1 ->> to search the Emp records  by first name (e.g-> 2.1-RAM)");
        printf("\nEnter 2.2 ->> to search the Emp records  by last name  (e.g-> 2.2-DEV)");
        printf("\nEnter 2.3 ->> to search the Emp records  by emp ID     (e.g-> 2.3-4578)");
        printf("\nEnter 3.0 ->> to search the Emp records  by skill set  (e.g-> 3.0-JAVA)");
        printf("\nEnter 4.0 ->> to search the Emp records  by exp        (e.g-> 4.0-2.5)");

        printf("\n\n--------Options for Add Update Delete -------------------------\n\n");
        printf("\nTo add Emp record <add-firstName lastName contactNo experience projectAllocated skillSet>");
        printf("\nExample input <add-KOUSHIK HALDER 9836900070 18 no c,c++,python,java,.net>");

        printf("\nTo add Emp record <upd-firstName lastName contactNo experience projectAllocated skillSet>");
        printf("\nExample input <upd-KOUSHIK HALDER 9836900070 18 no c,c++,python,java,.net>");

        printf("\nTo add Emp record <del-empid>");
        printf("\nExample input <del-4578>");

        printf("\n\n--------Options for Add Update Delete -------------------------\n\n");

        printf("Please enter the option for server\n");
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



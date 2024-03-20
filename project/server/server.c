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

int main()
{
    int msgId;
    msgQueue mq;
    clientInfo ClientInfo;
    msgId = msgget(KEY,0666 | IPC_CREAT);
    mq.msgType = CLIENT_INFO;
    while(1)
    {
        if( msgrcv(msgId ,&mq, sizeof(mq.msgPk), 0, 0 ) != -1 )
        {
            printf("mq.msgPk.structId = %d\n",mq.msgPk.structId);
            if(mq.msgType == CLIENT_START )
            {
                ClientInfo = (mq.msgPk.ClientInfo);
                printf(" client ID = %d\n",ClientInfo.ClientPID);

                assignWorkToThreads(&ClientInfo);
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


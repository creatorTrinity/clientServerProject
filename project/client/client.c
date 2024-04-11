#include "../header/dataStruct.h"

//<add-KOUSHIK-HALDER-9836900070-18-no-c,c++,python,java,.net>
void getEmpData(employee *Employee, char *inputStr)
{
    char *temp;
    float var;
    int var1;
    int indx = 0;

    temp = strtok(inputStr,"-");
    //ignoring the add or upd key word
    temp = strtok(NULL,"-");
    strcpy(Employee->firstName,temp);

    temp = strtok(NULL,"-");
    strcpy(Employee->lastName,temp);

    temp = strtok(NULL,"-");
    var1 = atoi(temp);
    Employee->empId = var1;


    temp = strtok(NULL,"-");
    strcpy(Employee->contactNo,temp);

    temp = strtok(NULL,"-");
    var = atof(temp);
    Employee->experience = var;

    temp = strtok(NULL,"-");
    if(strcmp(temp,"yes") == 0)
       Employee->projectAllocated = true;
    else
        Employee->projectAllocated = false;


    while( 1 )
    {
        temp = strtok(NULL,",");
        if( temp == NULL)
           break;
        strcpy(Employee->skillSet[indx++],temp);
    }
}
void serverConnect( int clientMsgId, int clientPID,char *serverResponse)
{
    int msgRet;
    int msgId = clientMsgId;
    int key = clientPID;
    char clientQueryMsg[MAX_ARR_SIZE];
    msgQueue mq;
    employee Employee;
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
        printf("\nTo add Emp record <add-firstName-lastName-empId-contactNo-experience-projectAllocated-skillSet>");
        printf("\nExample input <add-KOUSHIK-HALDER-0-9836900070-18-no-c,c++,python,java,.net>");

        printf("\nTo add Emp record <upd-firstName lastName contactNo experience projectAllocated skillSet>");
        printf("\nExample input <upd-KOUSHIK-HALDER-31-9836900070-18-no-c,c++,python,java,.net>");

        printf("\nTo add Emp record <del-empid>");
        printf("\nExample input <del-4578>");

        printf("\n\n--------Options for Add Update Delete -------------------------\n\n");
        printf("\n\nEnter exit to terminate\n");

        printf("Please enter the option for server\n");
        scanf("%s",clientQueryMsg);
        if( strlen(clientQueryMsg) > 4 )
        {
            if( strncmp(clientQueryMsg,"add",3)==0 || strncmp(clientQueryMsg,"upd",3)==0)
            {
                getEmpData( &Employee, clientQueryMsg);
                QueryString.Employee = Employee;
            }
        }

        strcpy(QueryString.query,clientQueryMsg);
        /*start taking input from client and send the request to the server*/

        MsgPack.DataPack.Data.QueryString = QueryString;
        MsgPack.DataPack.structId = QUERY_STRING;
        MsgPack.endOfPacket = 0;
        mq.msgType = 1;
        mq.msgPk = MsgPack;
        time(&currentTime);
        MsgPack.DataPack.timeStamp = currentTime;
        printf("\ncurrentTime =%lu\n",currentTime);
        printf("\nclient sending the request to server =%s\n",clientQueryMsg);
        msgRet = msgsnd(msgId, &mq, sizeof(mq.msgPk ), 0);
        if(msgRet==-1)
        {
            perror("msg Send error: ");
            printf("Client msg not send\n");
            strcpy(serverResponse,"Client msg not send");
            ///*msgctl(msgId, IPC_RMID, NULL);*/
            break;
        }

        if( strcmp(clientQueryMsg,"exit") == 0 )
        {
            break;
        }

        MsgPack.DataPack.Data.QueryResult = QueryResult;
        MsgPack.DataPack.structId = QUERY_RESULT;
        MsgPack.endOfPacket = 0;
        //mq.msgType = key;
        mq.msgPk = MsgPack;

        msgRet = msgrcv(msgId ,&mq, sizeof(mq.msgPk), 0, 0 );

        if( msgRet !=-1 )
        {
            printf("mq.msgPk.DataPack.structId = %d\n", mq.msgPk.DataPack.structId);
            if(mq.msgPk.DataPack.structId == QUERY_RESULT )
            {
                QueryResult = (mq.msgPk.DataPack.Data.QueryResult);
                printf(" client QueryResult = %s\n",QueryResult.result);
                printf("\n");
                while(mq.msgPk.batchData == true )
                {
                    Employee = mq.msgPk.DataPack.Data.QueryResult.Employee;
                    printf("Employee ID         = %d \n",Employee.empId);
                    printf("Employee first name = %s \n",Employee.firstName);
                    printf("Employee last name = %s \n",Employee.lastName);
                    printf("Employee experience = %f \n",Employee.experience);
                    for(int i = 0; i < NO_OF_SKILLS; i++)
                    {
                        if( strlen(Employee.skillSet[i]) > 0)
                        {
                            printf("Employee skillSet %d = %s \n",i+1, Employee.skillSet[i]);
                        }
                    }
                    //printf("press enter to check the next record\n");
                    printf("\n");

                    memset(&QueryResult,0,sizeof (QueryResult));
                    MsgPack.DataPack.Data.QueryResult = QueryResult;
                    MsgPack.DataPack.structId = QUERY_RESULT;
                    MsgPack.endOfPacket = 0;
                    //mq.msgType = key;
                    mq.msgPk = MsgPack;
                    
                    msgRet = msgrcv(msgId ,&mq, sizeof(mq.msgPk), 0, 0 );
                    if(mq.msgPk.batchData == true)
                    {
                        printf("\nclient received the last node from server\n");
                    }

                    if( msgRet == -1 )
                    {
                        printf("\n !!! server message receive error !!!\n");
                        break;
                    }
                }
                printf("\nserver last message = %s\n",mq.msgPk.DataPack.Data.QueryResult.result);
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
        else if( strcmp(ServerAck.msg,SERVER_IGNORE_MSG) == 0 )
        {
            printf("server received IGNORE_MSG connected successfully\n");
            serverConnect(msgId, clientPID, serverResponse);
            //serverConnect(msgId, clientPID, serverResponse);
        }
    }
    else
    {
        perror("Message receive Error :");
    }
}



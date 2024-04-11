#ifndef _QUERY_OPERATION_
#define _QUERY_OPERATION_

#include "dataStruct.h"
#include "srchRecords.h"
#include "linkedList.h"
#include "fileRW.h"

pthread_mutex_t lock;

void prepareClientResponse(queryResult *QueryResult,
                           queryString *QueryString,
                           node **_EMP_DB_DATA_CONTAINER_LIST_,
                           int *oprType)
{

    char query[MAX_ARR_SIZE];
    char retMsg[MAX_ARR_SIZE];
    //node *returnNode = NULL;
    int empId = 0;
    char *token;
    dataPack DataPack;
    char queryStr[MAX_ARR_SIZE] = "\0";
    char dataToSearch[MAX_ARR_SIZE] = "\0";
    node *temp_CONTAINER_LIST_ = *_EMP_DB_DATA_CONTAINER_LIST_;

    strcpy(query,QueryString->query);

    token = strtok(query,"-");
    if(token != NULL)
    {
        strcpy(queryStr,token);
        printf("query str from client = %s \n",queryStr);
    }

    token = strtok(NULL,"-");
    if(token != NULL)
    {
        strcpy(dataToSearch,token);
        printf("dataToSearch str from client = %s \n",dataToSearch);
    }

    if (   strcmp(queryStr, QUERY_OPTION_1_1) == 0 /*sorted by first name*/
        || strcmp(queryStr, QUERY_OPTION_1_2) == 0 /*sorted by last name*/
        || strcmp(queryStr, QUERY_OPTION_1_3) == 0 /*sorted by emp ID*/
        || strcmp(queryStr, QUERY_OPTION_1_4) == 0 /*sorted by year of experience*/
       )
    {

        *oprType = 0;
        sortLinkedList(&_EMP_DB_DATA_LIST_,queryStr);
        printLinkedList(_EMP_DB_DATA_LIST_);
    }
    else if(    strcmp(queryStr, QUERY_OPTION_2_1) == 0  /*search by first name*/
             || strcmp(query, QUERY_OPTION_2_2) == 0     /*search by last name*/
             || strcmp(query, QUERY_OPTION_2_3) == 0     /*search by emp ID*/
             || strcmp(query, QUERY_OPTION_3_0) == 0     /*search records by particular skill set*/
             || strcmp(query, QUERY_OPTION_4_0) == 0     /*search records by year of experience*/
           )
    {

        *oprType = 1;
        printf("\ncalling the searchRecords\n");
        searchRecords(_EMP_DB_DATA_LIST_, &temp_CONTAINER_LIST_,queryStr,dataToSearch);
        printContainerLinkedList(temp_CONTAINER_LIST_);
    }
    else if(    strcmp(queryStr, QUERY_OPTION_ADD_RECORD) == 0     /*add one record */
                || strcmp(query, QUERY_OPTION_UPADTE_RECORD) == 0  /*update record using empID*/
                || strcmp(query, QUERY_OPTION_DEL_RECORD) == 0     /*delete record using empID*/
            )
    {
        *oprType = 2;
        DataPack.structId = EMP_INFO;
        memset(&DataPack.Data.Employee, 0,sizeof (employee));
        memcpy(&DataPack.Data.Employee,&QueryString->Employee,sizeof (employee));
        if(digits_only(dataToSearch))
        {
            if(strcmp(query, QUERY_OPTION_UPADTE_RECORD) == 0 )  /*update record using empID*/
            {
                empId = DataPack.Data.Employee.empId;
            }
            else
            {
                empId = atoi(dataToSearch);
            }
            
            pthread_mutex_lock(&lock);
            addUpdateDeleteNode(&_EMP_DB_DATA_LIST_,
                                             &DataPack,
                                             queryStr,
                                             empId,
                                             retMsg);

            fileCommitData(_EMP_DB_DATA_LIST_);
            strcpy(QueryResult->result,retMsg);
            pthread_mutex_unlock(&lock);
        }
        else
        {
            sprintf(QueryResult->result,"<%s> operation need valid emp ID", queryStr);
            printf("\nUnhandled query option\n");
        }
    }
    else
    {
        strcpy(QueryResult->result,"Unhandled query option");
        printf("\nUnhandled query option\n");
    }
}

#endif//#ifndef _QUERY_OPERATION_
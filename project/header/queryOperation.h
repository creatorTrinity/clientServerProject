#ifndef _QUERY_OPERATION_
#define _QUERY_OPERATION_

#include "dataStruct.h"
#include "srchRecords.h"
#include "linkedList.h"

void prepareClientResponse(char *msg, char *query, node *_EMP_DB_DATA_CONTAINER_LIST_)
{

    char *token;
    char queryStr[MAX_ARR_SIZE] = "\0";
    char dataToSearch[MAX_ARR_SIZE] = "\0";
    //strcpy(msg,query);
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

        printf("\ncalling the searchRecords\n");
        searchRecords(_EMP_DB_DATA_LIST_, &_EMP_DB_DATA_CONTAINER_LIST_,queryStr,dataToSearch);
        printContainerLinkedList(_EMP_DB_DATA_CONTAINER_LIST_);
    }
    else
    {
        printf("\nUnhandled query option\n");
    }
}

#endif//#ifndef _QUERY_OPERATION_
#ifndef _QUERY_OPERATION_
#define _QUERY_OPERATION_

#include "dataStruct.h"

void prepareClientResponse(char *msg,char *query)
{

    strcpy(msg,query);

    if (   strcmp(query, QUERY_OPTION_1_1) == 0 /*sorted by first name*/
        || strcmp(query, QUERY_OPTION_1_2) == 0 /*sorted by last name*/
        || strcmp(query, QUERY_OPTION_1_3) == 0 /*sorted by emp ID*/
        || strcmp(query, QUERY_OPTION_1_4) == 0 /*sorted by year of experience*/
       )
    {

        sortLinkedList(&_EMP_DB_DATA_LIST_,query);
        printLinkedList(_EMP_DB_DATA_LIST_);
    }
    else if( strcmp(query, QUERY_OPTION_2_1) == 0 )
    {
        /*search by first name*/


    }
    else if( strcmp(query, QUERY_OPTION_2_2) == 0 )
    {
        /*search by last name*/

    }
    else if( strcmp(query, QUERY_OPTION_2_3) == 0 )
    {
        /*search by emp ID*/

    }
    else if( strcmp(query, QUERY_OPTION_3_0) == 0 )
    {
        /*search records by particular skill set*/

    }
    else if( strcmp(query, QUERY_OPTION_4_0) == 0 )
    {
        /*search records by no of experience*/

    }
    else
    {
        printf("\nUnhandled query option\n");
    }
}

#endif//#ifndef _QUERY_OPERATION_
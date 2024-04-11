#ifndef _SORT_BUBBLE_
#define _SORT_BUBBLE_

#include "dataStruct.h"

bool compareFunc(node *p,node *q, const char *str)
{
    if( strcmp(str, QUERY_OPTION_1_1) == 0 )
    {
        /*sort by first name*/
        if( strcmp( p->DataPack->Data.Employee.firstName, q->DataPack->Data.Employee.firstName) >= 0 )
            return true;
        else
            return false;
    }
    else if( strcmp(str, QUERY_OPTION_1_2) == 0 )
    {
        /*sort by last name*/
        if( strcmp( p->DataPack->Data.Employee.lastName, q->DataPack->Data.Employee.lastName) >= 0 )
            return true;
        else
            return false;
    }
    else if( strcmp(str, QUERY_OPTION_1_3) == 0 )
    {
        /*sort by emp ID*/
        if( p->DataPack->Data.Employee.empId >= q->DataPack->Data.Employee.empId )
            return true;
        else
            return false;
    }
    else if( strcmp(str, QUERY_OPTION_1_4) == 0 )
    {
        /*sort by year of experience*/
        if( p->DataPack->Data.Employee.experience >= q->DataPack->Data.Employee.experience )
            return true;
        else
            return false;
    }
    else if( strcmp(str, QUERY_OPTION_2_1) == 0 )
    {
        /*search by first name*/
        return false;
    }
    else if( strcmp(str, QUERY_OPTION_2_2) == 0 )
    {
        /*search by last name*/
        return false;
    }
    else if( strcmp(str, QUERY_OPTION_2_3) == 0 )
    {
        /*search by emp ID*/
        return false;
    }
    else if( strcmp(str, QUERY_OPTION_3_0) == 0 )
    {
        /*search records by particular skill set*/
        return false;
    }
    else if( strcmp(str, QUERY_OPTION_4_0) == 0 )
    {
        /*search records by no of experience*/
        return false;
    }
    else
    {
        printf("\nUnhandled query option\n");
    }
}

node* bubbleSort(node **head, const char *str)
{
    node *p,*q,*r;
    int c;
    p = *head;
    q = p;
    dataPack DataPack;

    while(p != NULL)
    {
        q = p->next;
        while(q != NULL)
        {
            if( compareFunc(p,q,str) )
            {
                memcpy(&DataPack, q->DataPack, sizeof (dataPack));
                memcpy(q->DataPack, p->DataPack, sizeof (dataPack));
                memcpy(p->DataPack, &DataPack, sizeof (dataPack));
            }
            q = q->next;
        }
        p = p->next;
    }
    return *head;
}

#endif //_SORT_BUBBLE_
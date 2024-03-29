#ifndef _SORT_BUBBLE_
#define _SORT_BUBBLE_

#include "dataStruct.h"

/*void bubbleSort(int *a, int n)
{

    int c;
    for(int i=0;i<n;i++)
    {
       for(int j=i+1;j<n;j++)
       {
          if( a[i] >= a[j] )
          {
              c = a[j];
              a[j] = a[i];
              a[i] = c;
          }
       }
    }
    return ;
}*/
bool compareFunc(node *p,node *q, const char *str)
{
    if( strcmp(str, QUERY_OPTION_1_1) == 0 )
    {
        //sort by first name
        if( strcmp( p->DataPack->Data.Employee.firstName, q->DataPack->Data.Employee.firstName) >= 0 )
            return true;
        else
            return false;
    }
    else if( strcmp(str, QUERY_OPTION_1_2) == 0 )
    {
        //sort by second name
        if( strcmp( p->DataPack->Data.Employee.lastName, q->DataPack->Data.Employee.lastName) >= 0 )
            return true;
        else
            return false;
    }
    else if( strcmp(str, QUERY_OPTION_1_3) == 0 )
    {
        //sort by emp id
        if( p->DataPack->Data.Employee.empId, q->DataPack->Data.Employee.empId >= 0 )
            return true;
        else
            return false;
    }
    else if( strcmp(str, QUERY_OPTION_2_1) == 0 )
    {
        //search by first name
        return false;
    }
    else if( strcmp(str, QUERY_OPTION_2_2) == 0 )
    {
        //search by last name
        return false;
    }
    else if( strcmp(str, QUERY_OPTION_2_3) == 0 )
    {
        //search by emp id
        return false;
    }
    else if( strcmp(str, QUERY_OPTION_3_0) == 0 )
    {
        //List all the record with particular skill set
        return false;
    }
    else if( strcmp(str, QUERY_OPTION_4_0) == 0 )
    {
        // List all the records based on no of years of experience
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

    while(p != NULL)
    {
        q = p->next;
        while(q != NULL)
        {
            if( compareFunc(p,q,str) )
            {
                r = q;
                q->DataPack = p->DataPack;
                p->DataPack = r->DataPack;
            }
            q = q->next;
        }
        p = p->next;
    }
    return *head;
}

#endif //_SORT_BUBBLE_
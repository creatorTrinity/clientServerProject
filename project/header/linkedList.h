#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#include "dataStruct.h"
#include "sortBubble.h"

node* addNode(node **head,dataPack *DataPack)
{
    node *p,*q,*r;
    p = (node *)malloc(sizeof(node));
    dataPack *TempDataPack = NULL;

    TempDataPack = (dataPack *)malloc(sizeof (dataPack));
    memcpy(TempDataPack, DataPack, sizeof (dataPack));
    p->DataPack = TempDataPack;
    p->next = NULL;

    if ( *head == NULL)
    {
        *head = p;
    }
    else
    {
        q = *head;
        while(q->next != NULL)
        {
            q = q->next;
        }
        q->next = p;
    }
    return *head;
}

node* addUpdateDeleteNode(node **head,
                          dataPack *DataPack,
                          char *queryStr,
                          int empId,
                          char *msg)
{
    node *p, *q;
    int indx = 0;
    p = *head;
    q = NULL;
    while(p != NULL)
    {
        indx++;
        if( p->DataPack->Data.Employee.empId == empId )
        {
            break;
        }
        q = p;
        p = p->next;
    }

    if ( p == NULL)
    {
        printf("record not found for the EMPID = %d\n",empId);
        sprintf(msg,"Record not found for the EMPID = <%d>\n",empId);
        return NULL;
    }

    if( strcmp(queryStr,QUERY_OPTION_ADD_RECORD) == 0)
    {
        addNode(&_EMP_DB_DATA_LIST_, DataPack);
        sprintf(msg,"Record added for the EMPID = <%d>\n",empId);
    }
    else if( strcmp(queryStr,QUERY_OPTION_UPADTE_RECORD) == 0 )
    {
        memcpy(&p->DataPack, &p->DataPack, sizeof (dataPack));
        sprintf(msg,"Record Updated for the EMPID = <%d>\n",empId);
    }
    else if( strcmp(queryStr,QUERY_OPTION_DEL_RECORD) == 0)
    {
        if( indx == 1)
        {
            p = *head;
            *head = (*head)->next;
            free(p);
            p = NULL;
        }
        else
        {
            q->next = p->next;
            free(p);
            p = NULL;
        }
        sprintf(msg,"Record deleted for the EMPID = <%d>\n",empId);
    }
}

node* copyLinkedList(node **dest, node *src)
{
   node *p,*q,*r;
    if(src == NULL)
    {
        printf("\nthe Linked list is empty\n");
        dest = NULL;
        return NULL;
    }

    q = (node *)malloc(sizeof(node));
    q->DataPack = (dataPack *)malloc(sizeof (dataPack));
    q->DataPack->blank = true;
    q->next = NULL;
    *dest = q;

    p = src;
    p = p->next;

    while(p != NULL)
    {
        r = (node *)malloc(sizeof(node));
        r->DataPack = (dataPack *)malloc(sizeof (dataPack));
        r->DataPack->blank = true;
        r->next = NULL;

        q->next = r;
        q = r;
        p = p->next;
    }
    return *dest;
}
void printLinkedList(node *_EMP_DB_DATA_LIST_)
{
    node *p;
    employee Employee;
    if(_EMP_DB_DATA_LIST_ == NULL)
    {
        printf("\nthe Linked list is empty\n");
        return;
    }
    p = _EMP_DB_DATA_LIST_;
    printf("\n--------------------------------------\n");
    printf("\nGOING TO PRINT THE EMPDATA LINKED LIST\n");
    printf("\n--------------------------------------\n");
    while(p != NULL)
    {
        if(p->DataPack->structId == EMP_INFO)
        {
            Employee = p->DataPack->Data.Employee;
            printf("Employee ID         = %d \n",Employee.empId);
            printf("Employee first name = %s \n",Employee.firstName);
            printf("Employee last name = %s \n",Employee.lastName);
            printf("Employee experience = %f \n",Employee.experience);
            printf("Employee skillSet 0 = %s \n",Employee.skillSet[0]);
            printf("Employee skillSet 1 = %s \n",Employee.skillSet[1]);
            printf("Employee skillSet 2 = %s \n",Employee.skillSet[2]);
            printf("Employee skillSet 3 = %s \n",Employee.skillSet[3]);
            printf("\n");
        }
        else
        {
            printf("\nunknown data structure\n");
        }
        p = p->next;
    }
    return;
}

void printContainerLinkedList(node *_EMP_DB_DATA_CONTAINER_LIST_)
{
    node *p;
    employee Employee;
    p = _EMP_DB_DATA_CONTAINER_LIST_;

    if(p == NULL)
    {
        printf("\nthe Linked list is empty\n");
        return;
    }
    if( p->DataPack->blank == true)
    {
        printf("\nthe Linked list is empty\n");
        return;
    }

    printf("\n--------------------------------------\n");
    printf("\nGOING TO PRINT THE EMPDATA CONTAINER LINKED LIST\n");
    printf("\n--------------------------------------\n");
    while(p != NULL )
    {
        if( p->DataPack->blank == true)
            break;
        if(p->DataPack->structId == EMP_INFO)
        {
            Employee = p->DataPack->Data.Employee;
            printf("Employee ID         = %d \n",Employee.empId);
            printf("Employee first name = %s \n",Employee.firstName);
            printf("Employee last name = %s \n",Employee.lastName);
            printf("Employee experience = %f \n",Employee.experience);
            printf("Employee skillSet 0 = %s \n",Employee.skillSet[0]);
            printf("Employee skillSet 1 = %s \n",Employee.skillSet[1]);
            printf("Employee skillSet 2 = %s \n",Employee.skillSet[2]);
            printf("Employee skillSet 3 = %s \n",Employee.skillSet[3]);
            printf("\n");
        }
        else
        {
            printf("\nunknown data structure\n");
        }
        p = p->next;
    }
    return;
}

node* sortLinkedList(node **head,char *queryStr)
{
    node *p;
    employee Employee;
    if(_EMP_DB_DATA_LIST_ == NULL)
    {
        printf("\nthe Linked list is empty\n");
        return NULL;
    }
    p = _EMP_DB_DATA_LIST_;
    //printf("\nGOING TO PRINT THE EMPDATA LINKED LIST\n");
    /*Employee = p->DataPack->Data.Employee;
    printf("Employee first name = %s \n",Employee.firstName);
    printf("Employee last name = %s \n",Employee.lastName);
    printf("Employee experience = %f \n",Employee.experience);
    printf("Employee skillSet 0 = %s \n",Employee.skillSet[0]);
    printf("Employee skillSet 1 = %s \n",Employee.skillSet[1]);
    printf("Employee skillSet 2 = %s \n",Employee.skillSet[2]);
    printf("Employee skillSet 3 = %s \n",Employee.skillSet[3]);
    printf("\n");*/

    if(p->DataPack->structId == EMP_INFO)
    {
        bubbleSort(&p, queryStr);
    }
    else
    {
        printf("\nUnhandled query option\n");
    }
    return _EMP_DB_DATA_LIST_;
}


node* removeWatchDogNode(node **head,node *delNode)
{
    node *p,*q,*r;
    p = *head;
    int ind = 0;

    if(delNode == NULL)
    {
        return NULL;
    }
    else if( delNode->DataPack == NULL )
    {
        return NULL;
    }
    pthread_t tid = delNode->DataPack->Data.WatchDog.tid;

    ind = 0;
    while(p != NULL)
    {
        if ( p->DataPack->Data.WatchDog.tid  == delNode->DataPack->Data.WatchDog.tid)
        {
            if( ind == 0 )
            {
                q = p;
                p = p->next;
                free(q);
                q = NULL;
                *head = p;
                break;
            }
            else
            {
                q->next = p->next;
                free(p);
                p = NULL;
                break;
            }
            printf("\nwatchdog node removed from list index = %d \n",ind);
        }
        q = p;
        p = p->next;
        ind++;
    }
    return *head;
}

#endif //_LINKED_LIST_
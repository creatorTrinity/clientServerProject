#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#include "dataStruct.h"
#include "sortBubble.h"


node* addNode(node **head,dataPack *DataPack)
{
    node *p,*q,*r;
    p = (node *)malloc(sizeof(node));
    p->DataPack = DataPack;
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
    if(_EMP_DB_DATA_LIST_ == NULL)
    {
        printf("\nthe Linked list is empty\n");
        return;
    }
    p = _EMP_DB_DATA_LIST_;
    printf("\n--------------------------------------\n");
    printf("\nGOING TO PRINT THE EMPDATA LINKED LIST\n");
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
#ifndef _LINKED_LIST_
#define _LINKED_LIST_
#include "dataStruct.h"

node* addNode(node *head,dataPack *DataPack)
{
    watchDogNode *p,*q,*r;
    p = (watchDogNode *)malloc(sizeof(watchDogNode));
    p->dataPack = DataPack;
    p->next = NULL;

    if ( head == NULL)
    {
        head = p;
    }
    else
    {
        q = head;
        while(q->next != NULL)
        {
            q = q->next;
        }
        p->next = p;
    }
    return head;
}

node* removeNode(node *head,dataPack *DataPack)
{
    watchDogNode *p,*q,*r;
    p = (watchDogNode *)malloc(sizeof(watchDogNode));
    p->dataPack = DataPack;
    p->next = NULL;

    if ( head == NULL)
    {
        head = p;
    }
    else
    {
        q = head;
        while(q->next != NULL)
        {
            q = q->next;
        }
        p->next = p;
    }
    return head;
}

#endif //_LINKED_LIST_
#ifndef _LINKED_LIST_
#define _LINKED_LIST_
#include "dataStruct.h"

node* addNode(node **head,dataPack *DataPack)
{
    node *p,*q,*r;
    p = (node *)malloc(sizeof(node));
    p->dataPack = DataPack;
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

node* removeWatchDogNode(node **head,dataPack *delDataPack)
{
    node *p,*q,*r;
    p = *head;

    if(delDataPack== NULL)
    {
        return NULL;
        if( delDataPack->Data.WatchDog == NULL )
        {
            return NULL;
        }
    }


    pthread_t tid = delDataPack->Data.WatchDog.tid;
    q = p;
    while(p->next != NULL)
    {
        if ( p->DataPack->Data.WatchDog.tid ==  tid)
        {
            q->next = p->next;
            free(p);
            p = NULL;
            break;
        }
        q = p;
        p = p->next;
    }
    return *head;
}

#endif //_LINKED_LIST_
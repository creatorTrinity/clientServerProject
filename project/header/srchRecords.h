#ifndef _SRCH_RECORDS_
#define _SRCH_RECORDS_

#include "dataStruct.h"

void searchRecords(node *head, node **container, const char *queryStr,const char *dataToSearch)
{
    node *p,*q,*r;
    node *blank = NULL;
    char temp[MAX_ARR_SIZE];

    blank = *container;
    blank->DataPack->blank = true;
    p = head;

    while(p!=NULL)
    {
        if( strcmp(queryStr, QUERY_OPTION_2_1) == 0 )
        {
            /*search by first name*/
            if( strcmp(p->DataPack->Data.Employee.firstName, dataToSearch) == 0 )
            {
                blank->DataPack->structId = EMP_INFO;
                blank->DataPack->blank = false;
                memcpy(&blank->DataPack->Data.Employee, &p->DataPack->Data.Employee, sizeof (p->DataPack->Data.Employee));
            }
        }
        else if( strcmp(queryStr, QUERY_OPTION_2_2) == 0 )
        {
            /*search by last name*/
            if( strcmp(p->DataPack->Data.Employee.lastName, dataToSearch) == 0 )
            {
                blank->DataPack->structId = EMP_INFO;
                blank->DataPack->blank = false;
                memcpy(&blank->DataPack->Data.Employee, &p->DataPack->Data.Employee, sizeof (p->DataPack->Data.Employee));
            }
        }
        else if( strcmp(queryStr, QUERY_OPTION_2_3) == 0 )
        {
            /*search by emp ID*/
            sprintf(temp,"%d",p->DataPack->Data.Employee.empId);
            if( strcmp(temp, dataToSearch) == 0 )
            {
                blank->DataPack->structId = EMP_INFO;
                blank->DataPack->blank = false;
                memcpy(&blank->DataPack->Data.Employee, &p->DataPack->Data.Employee, sizeof (p->DataPack->Data.Employee));
            }
        }
        else if( strcmp(queryStr, QUERY_OPTION_3_0) == 0 )
        {
            /*search records by particular skill set*/
            bool flag = false;
            for(int i=0; i < NO_OF_SKILLS; i++)
            {
                if( strcmp(p->DataPack->Data.Employee.skillSet[i] , dataToSearch) == 0 )
                {
                    flag = true;
                }
            }
            if( flag == true )
            {
                blank->DataPack->structId = EMP_INFO;
                blank->DataPack->blank = false;
                memcpy(&blank->DataPack->Data.Employee, &p->DataPack->Data.Employee, sizeof (p->DataPack->Data.Employee));
            }
        }
        else if( strcmp(queryStr, QUERY_OPTION_4_0) == 0 )
        {
            /*search records by no of experience*/
            double exp = atof(dataToSearch);
            if( p->DataPack->Data.Employee.experience >= exp )
            {
                blank->DataPack->structId = EMP_INFO;
                blank->DataPack->blank = false;
                memcpy(&blank->DataPack->Data.Employee, &p->DataPack->Data.Employee, sizeof (p->DataPack->Data.Employee));
            }
        }
        else
        {
            printf("\nUnhandled query option\n");
        }
        p = p->next;
        if( blank->DataPack->blank == false )
        {
            blank = blank->next;
            if( blank != NULL )
            {
                blank->DataPack->blank = true;
            }
        }
    }
}
#endif //_SRCH_RECORDS_
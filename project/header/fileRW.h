#ifndef _FILE_RW_
#define _FILE_RW_

#include "dataStruct.h"
#include "linkedList.h"

void fillDummyData(employee *arrEmployee, int size)
{
    float expCount = 0.2;

    arrEmployee[0].empId = 1;
    strcpy(arrEmployee[0].contactNo , "9836900011");
    arrEmployee[0].experience = 10.5;
    strcpy(arrEmployee[0].firstName , "RAM");
    strcpy(arrEmployee[0].lastName , "DEV");
    arrEmployee[0].orderSeq = 0;
    arrEmployee[0].projectAllocated = true;


    memset(arrEmployee[0].skillSet,0,sizeof (arrEmployee[0].skillSet));
    strcpy(arrEmployee[0].skillSet[0],"C");
    strcpy(arrEmployee[0].skillSet[1],"C++");
    strcpy(arrEmployee[0].skillSet[2],"JAVA");
    strcpy(arrEmployee[0].skillSet[3],"PYTHON");
    sprintf(arrEmployee[0].PK,"%s%s%d",arrEmployee[0].firstName,arrEmployee[0].lastName,arrEmployee[0].empId);

    arrEmployee[1].empId = 2;
    strcpy(arrEmployee[1].contactNo , "9836900012");
    arrEmployee[1].experience = 2.5;
    strcpy(arrEmployee[1].firstName , "BAM");
    strcpy(arrEmployee[1].lastName , "LALA");
    arrEmployee[1].orderSeq = 1;
    arrEmployee[1].projectAllocated = false;


    memset(arrEmployee[1].skillSet,0,sizeof (arrEmployee[1].skillSet));
    strcpy(arrEmployee[1].skillSet[0],".NET");
    strcpy(arrEmployee[1].skillSet[1],"C#");
    strcpy(arrEmployee[1].skillSet[2],"JAVA");
    strcpy(arrEmployee[1].skillSet[3],"PYTHON");
    sprintf(arrEmployee[1].PK,"%s%s%d",arrEmployee[1].firstName,arrEmployee[1].lastName,arrEmployee[1].empId);

    for(int i=2; i < size; i++)
    {
        if(expCount > 4 )
        {
            expCount = 0.2;
        }

        arrEmployee[i].empId = i + 1;
        strcpy(arrEmployee[i].contactNo , "9836900012");
        arrEmployee[i].experience = 2.5 + expCount++;
        sprintf(arrEmployee[i].firstName , "BAM_%d",i);
        sprintf(arrEmployee[i].lastName , "LALA_%d",i);
        arrEmployee[i].orderSeq = 1;
        arrEmployee[i].projectAllocated = false;


        memset(arrEmployee[i].skillSet,0,sizeof (arrEmployee[i].skillSet));
        strcpy(arrEmployee[i].skillSet[0],".NET");
        strcpy(arrEmployee[i].skillSet[1],"C#");
        strcpy(arrEmployee[i].skillSet[2],"JAVA");
        strcpy(arrEmployee[i].skillSet[3],"PYTHON");
        sprintf(arrEmployee[i].PK,"%s%s%d",arrEmployee[1].firstName,arrEmployee[1].lastName,arrEmployee[1].empId);
    }
}
int fileInsertData()
{
    FILE *infile = NULL;
    int flag = false;
    long int fileSize = 0;

    infile = fopen(_EMPDB_PATH_,"r");
    if(infile == NULL)
    {
        perror("file opening error::\n");
        return -1;
    }

    fseek(infile,0L,SEEK_END);
    fileSize = ftell(infile);
    printf("empDB file size = %ld",fileSize);

    fclose(infile);
    if( fileSize > 0)
    {
        printf("\nfile is containing data\n");
        return 0;
    }

    employee Employee,Employee1;
    employee arrEmployee[30];
    fillDummyData(arrEmployee,30);

    infile = fopen(_EMPDB_PATH_,"wb");
    if(infile == NULL)
    {
        perror("file opening error::\n");
        return -1;
    }
    //flag = fwrite(&Employee,sizeof (employee), 1, infile);
    //flag = fwrite(&Employee1,sizeof (employee), 1, infile);
    flag = fwrite(arrEmployee,sizeof (employee), 30, infile);
    if(flag)
    {
        printf("\nfile write successful\n");
    }
    else
    {
        printf("\n!!!Error file write!!!\n");
    }
    fclose(infile);
    return 0;
}
int fileBackupData()
{
    FILE *fptr1, *fptr2;
    char newFilename[100], c;
    time_t currentTime;
    time(&currentTime);
    sprintf(newFilename,"%s_%lu",_EMPDB_PATH_,currentTime);

    printf("\n!!!Taking backup of the old empData  file = %s !!!\n",_EMPDB_PATH_);

    // Open one file for reading
    fptr1 = fopen(_EMPDB_PATH_, "rb");
    if (fptr1 == NULL)
    {
        printf("Cannot open file %s \n", _EMPDB_PATH_);
        return 1;
    }

    printf("\n!!!Taking backup of the old empData into the file = %s !!!\n",newFilename);

    // Open another file for writing
    fptr2 = fopen(newFilename, "wb");
    if (fptr2 == NULL)
    {
        printf("Cannot open file %s \n", newFilename);
        return 1;
    }

    // Read contents from file
    c = fgetc(fptr1);
    while (c != EOF)
    {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }

    printf("\nContents copied to %s", newFilename);

    fclose(fptr1);
    fclose(fptr2);
    return 0;
}


int fileCommitData(node *_EMP_DB_DATA_LIST_)
{
    FILE *infile;
    int flag;
    node *p;
    employee Employee;

    /*taking backup of the old emp database */
    if(fileBackupData() == 1)
    {
        return 1;
    }

    p = _EMP_DB_DATA_LIST_;
    if(p == NULL)
    {
        printf("\nthe Linked list is empty\n");
        return 0;
    }
    if( p->DataPack->blank == true)
    {
        printf("\nthe Linked list is empty\n");
        return 0;
    }
    infile = fopen(_EMPDB_PATH_,"wb");
    if(infile == NULL)
    {
        perror("file opening error::\n");
        return 1;
    }
    printf("\n--------------------------------------\n");
    printf("\nGOING TO COMMIT THE EMPDATA\n");
    printf("\n--------------------------------------\n");
    while(p != NULL )
    {
        if( p->DataPack->blank == true)
            break;
        if(p->DataPack->structId == EMP_INFO)
        {
            flag = fwrite(&(p->DataPack->Data.Employee),
                          sizeof (employee),
                          1,
                          infile);

            //Employee = p->DataPack->Data.Employee;
            /*printf("Employee ID         = %d \n",Employee.empId);
            printf("Employee first name = %s \n",Employee.firstName);
            printf("Employee last name = %s \n",Employee.lastName);
            printf("Employee experience = %f \n",Employee.experience);
            printf("Employee skillSet 0 = %s \n",Employee.skillSet[0]);
            printf("Employee skillSet 1 = %s \n",Employee.skillSet[1]);
            printf("Employee skillSet 2 = %s \n",Employee.skillSet[2]);
            printf("Employee skillSet 3 = %s \n",Employee.skillSet[3]);
            printf("\n");*/



            if(!flag)
            {
                printf("\n!!!fileCommitData::Error file write!!!\n");
                break;
            }
        }
        else
        {
            printf("\nunknown data structure\n");
        }
        p = p->next;
    }

    fclose(infile);
    if(flag)
    {
        printf("\nfileCommitData::file write successful\n");
        return 0;
    }
    else
    {
        return 1;
    }
}
int fileReadData()
{
    FILE *infile;
    int flag;
    employee Employee;
    infile = fopen(_EMPDB_PATH_,"rb");
    if(infile == NULL)
    {
        perror("file opening error::\n");
        return -1;
    }
    printf("\n\ngoing to read empDB data file\n\n");
    flag = fread(&Employee,sizeof (employee), 1, infile);
    printf("Employee first name = %s \n",Employee.firstName);
    printf("Employee last name = %s \n",Employee.lastName);

    flag = fread(&Employee,sizeof (employee), 1, infile);
    printf("Employee first name = %s \n",Employee.firstName);
    printf("Employee last name = %s \n",Employee.lastName);

    if(flag)
    {
        printf("\nfile read successful\n");
    }
    else
    {
        printf("\n!!!Error file read!!!\n");
    }
    fclose(infile);
}


int empDBLinkedList()
{
    FILE *infile;
    dataPack DataPack;
    int flag;
    employee Employee;
    infile = fopen(_EMPDB_PATH_,"rb");
    if(infile == NULL)
    {
        perror("file opening error::\n");
        return -1;
    }
    printf("\n\ngoing to read empDB data file\n\n");
    while( !feof(infile) /*&& !ferror(infile)*/ )
    {
        flag = fread(&Employee,sizeof (employee), 1, infile);
        if(flag)
        {
            printf("\nempdbLinkedList::file read successful\n");
        }
        else
        {
            break;
        }
        if( _LAST_EMP_ID_ < Employee.empId)
        {
           _LAST_EMP_ID_ = Employee.empId;
        }

        memcpy(&(DataPack.Data.Employee),&Employee,sizeof (Employee));
        DataPack.blank = false;
        DataPack.structId = EMP_INFO;
        addNode(&_EMP_DB_DATA_LIST_,&DataPack);
    }
    fclose(infile);
    //printLinkedList(_EMP_DB_DATA_LIST_);
}

#endif //_FILE_RW_
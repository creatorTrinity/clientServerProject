#ifndef _FILE_RW_
#define _FILE_RW_

#include "dataStruct.h"
#include "linkedList.h"


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
    Employee.empId = 1;
    strcpy(Employee.contactNo , "9836900011");
    Employee.experience = 4;
    strcpy(Employee.firstName , "RAM");
    strcpy(Employee.lastName , "DEV");
    Employee.orderSeq = 0;
    Employee.projectAllocated = true;
    Employee.experience = 10.5;

    memset(Employee.skillSet,0,sizeof (Employee.skillSet));
    strcpy(Employee.skillSet[0],"C");
    strcpy(Employee.skillSet[1],"C++");
    strcpy(Employee.skillSet[2],"JAVA");
    strcpy(Employee.skillSet[3],"PYTHON");
    sprintf(Employee.PK,"%s%s%d",Employee.firstName,Employee.lastName,Employee.empId);

    Employee1.empId = 2;
    strcpy(Employee1.contactNo , "9836900012");
    Employee1.experience = 4;
    strcpy(Employee1.firstName , "BAM");
    strcpy(Employee1.lastName , "LALA");
    Employee1.orderSeq = 1;
    Employee1.projectAllocated = false;
    Employee1.experience = 2.5;

    memset(Employee1.skillSet,0,sizeof (Employee1.skillSet));
    strcpy(Employee1.skillSet[0],".NET");
    strcpy(Employee1.skillSet[1],"C#");
    strcpy(Employee1.skillSet[2],"JAVA");
    strcpy(Employee1.skillSet[3],"PYTHON");
    sprintf(Employee1.PK,"%s%s%d",Employee1.firstName,Employee1.lastName,Employee1.empId);

    infile = fopen(_EMPDB_PATH_,"wb");
    if(infile == NULL)
    {
        perror("file opening error::\n");
        return -1;
    }
    flag = fwrite(&Employee,sizeof (employee), 1, infile);
    flag = fwrite(&Employee1,sizeof (employee), 1, infile);
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
    dataPack *DataPack;
    dataPack *BLNKDataPack;
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
        DataPack = (dataPack *)malloc(sizeof (dataPack));
        memcpy(&DataPack->Data.Employee,&Employee,sizeof (Employee));
        DataPack->blank = false;
        DataPack->structId = EMP_INFO;
        addNode(&_EMP_DB_DATA_LIST_,DataPack);
        BLNKDataPack = (dataPack *)malloc(sizeof (dataPack));
        BLNKDataPack->blank = true;
        addNode(&_EMP_DB_DATA_CONTAINER_LIST_,BLNKDataPack);


        /*printf("Employee first name = %s \n",Employee.firstName);
        printf("Employee last name = %s \n",Employee.lastName);
        printf("Employee experience = %f \n",Employee.experience);
        printf("Employee skillSet 0 = %s \n",Employee.skillSet[0]);
        printf("Employee skillSet 1 = %s \n",Employee.skillSet[1]);
        printf("Employee skillSet 2 = %s \n",Employee.skillSet[2]);
        printf("Employee skillSet 3 = %s \n",Employee.skillSet[3]);*/

    }
    fclose(infile);
    //printLinkedList(_EMP_DB_DATA_LIST_);
}

#endif //_FILE_RW_
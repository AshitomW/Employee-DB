
#include <stdio.h>
#include <sys/_endian.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "parser.h"
#include "common.h"



int read_employees(int fd, struct dbheader_t * dbheader, struct employee_t **employeesOut){

    if(fd < 0){
        printf("Got a bad file descriptor\n");
        return STATUS_ERROR;
    }


    int count = dbheader->count;



    struct employee_t * employees = calloc(count,sizeof(struct employee_t));
    if(employees == NULL){
        printf("Malloc Failed \n");
        return STATUS_ERROR;
    }


    read(fd, employees , count*sizeof(struct employee_t));
    int i = 0;
    for(;i<count;i++){
        employees[i].hours = ntohl(employees[i].hours);
    }

    *employeesOut = employees;
    return STATUS_SUCCESS;

}






int create_database_header(int fd,struct dbheader_t** headerOut){
    struct dbheader_t* header = calloc(1, sizeof(struct dbheader_t));
    
    if(header == NULL) {
        printf("Malloc Failed \n");
        return STATUS_ERROR;
    }

    header->version = 0x1;
    header->count = 0;
    header->magic = HEADER_MAGIC;
    header->filesize = sizeof(struct dbheader_t);
    

    *headerOut = header;


    return STATUS_SUCCESS;

}


int validate_database_header(int fd,struct dbheader_t** headerOut){
    if(fd < 0){
        printf("Bad file descriptor");
        return STATUS_ERROR;
    }      
    struct dbheader_t* dbheader = calloc(1, sizeof(struct dbheader_t));
    if(dbheader == NULL){
        printf("Malloc Failed To Create Database Header!\n");
        return STATUS_ERROR;
    }
    
    if(read(fd, dbheader,sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)){
        perror("read");
        free(dbheader);
        return STATUS_ERROR;
    }

    
    dbheader->version = ntohs(dbheader->version);
    dbheader->count = ntohs(dbheader->count);
    dbheader->magic = ntohl(dbheader->magic);
    dbheader->filesize = ntohl(dbheader->filesize);

    
    if(dbheader->version != 1){
        printf("Improper Header Version\n");
        free(dbheader);
        return -1;
    }
    if(dbheader->magic != HEADER_MAGIC){
        printf("Improper Header Magic\n");
        free(dbheader);
        return -1;
    }

    struct stat dbstat = {0};
    fstat(fd, &dbstat);
    if(dbheader->filesize != dbstat.st_size){
        printf("Corrupted Database");
        free(dbheader);
        return -1;
    }

    *headerOut = dbheader;

   return STATUS_SUCCESS; 
    
}




int add_employee(struct dbheader_t * dbheader, struct employee_t *employees, char *string){
    
    char *name = strtok(string,",");
    char *address = strtok(NULL, ",");
    char *hours = strtok(NULL,",");

  
    strncpy(employees[dbheader->count-1].name,name,sizeof(employees[dbheader->count-1].name));
    strncpy(employees[dbheader->count-1].address,address,sizeof(employees[dbheader->count-1].address));
    
    employees[dbheader->count-1].hours = atoi(hours);



    return STATUS_SUCCESS;
}



int output_file(int fd, struct dbheader_t* dbheader,struct employee_t* employees){

    if(fd < 0){
        printf("Bad File Descriptor!\n");
        return STATUS_ERROR;
    }
    
    int dbcount = dbheader->count;
    dbheader->magic = htonl(dbheader->magic);
    dbheader->filesize = htonl(sizeof(struct dbheader_t)+sizeof(struct employee_t) * dbheader->count);
    dbheader->count = htons(dbheader->count);
    dbheader->version = htons(dbheader->version);

    // write the header at the beginning of the file.
    lseek(fd,0,SEEK_SET);

    write(fd,dbheader,sizeof(struct dbheader_t));

   
    
    for(int i=0;i<dbcount;i++){
        employees[i].hours = htonl(employees[i].hours);
        write(fd,&employees[i],sizeof(struct employee_t));
    }



    return STATUS_SUCCESS;
    
}

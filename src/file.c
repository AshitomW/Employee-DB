#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "file.h"
#include "common.h"



int create_database_file(char *filename){
   
    int fd = open(filename,O_RDONLY);
    if(fd != -1){
        close(fd);
        printf("The File Already Exists !\n");
        return STATUS_ERROR;
    }


    fd = open(filename, O_RDWR | O_CREAT , 0644);
    if(fd == -1){
        perror("open");
        return STATUS_ERROR;
    }


    return fd;
}


int load_database_file(char *filename){

    int fd = open(filename, O_RDWR, 0644);
    if(fd == -1){
        perror("open");
        return STATUS_ERROR;
    }

    return fd;
    
}


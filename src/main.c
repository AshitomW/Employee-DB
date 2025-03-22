#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>


#include "common.h"
#include "file.h"
#include "parser.h"

struct option long_options []={
    {"help",no_argument,0,'h'},
    {"newfile",no_argument,0,'n'},
    {"add",optional_argument,0,'a'},
    {"filepath",required_argument,0,'f'},
    {0,0,0,0}
};


void printUsageInstructions(char *argv[]){
    printf("Usage : %s --file=<DB-FilePath> [-n]\n",argv[0]);
    printf("\t -f , --filepath \tSpecify the input filepath (required)\n");
    printf("\t -n , --newfile \tIndicates A New File");
}



int main(int argc, char *argv[])
{
    
    
    int opts;
    int option_index = 0;


    bool isNewFile = false;
    char *filepath = NULL;
    char *addString = NULL;
   

    int database_fd = -1;
    struct dbheader_t* dbheader = NULL; 
    struct employee_t* employees = NULL;



    while((opts = getopt_long(argc,argv,"hnf:a:",long_options,&option_index)) != -1){
    
        switch(opts){
        case 'h':
        printUsageInstructions(argv);
        return 0;
        break;
        case 'n':
            isNewFile = true;
            break;
         case 'f':
            filepath = optarg;
            break;
        case 'a':
            addString = optarg;
            break;
         case '?':
            break;
    
         default: 
                return -1;

        }

    }

    if(filepath == NULL){
        printf("Should Provide A FileName !\n");
        printUsageInstructions(argv);
        return 0;
        
    }

    printf("filepath : %s\n",filepath);
    printf("New File: %d\n",isNewFile);





    if(isNewFile){
       database_fd = create_database_file(filepath);
       if(database_fd == STATUS_ERROR){
            printf("Unable To Create Database File\n");
            return -1;
        }

      if(  create_database_header(database_fd,&dbheader)  == STATUS_ERROR){
            printf("Failed To Create Database Header !\n");
            return -1;
        }

    }else{
        database_fd = load_database_file(filepath);
        if(database_fd == STATUS_ERROR){
            printf("Unable To Load Database File\n");
            return -1;
        }
        if(validate_database_header(database_fd,&dbheader) == STATUS_ERROR){
            printf("Failed To Validate Database Header\n");
            return -1;
        }
    }



    if(read_employees(database_fd, dbheader,&employees) != STATUS_SUCCESS){
        printf("Failed To Read Employees Data !");
        return -1;
    }
    if(addString){
        dbheader->count++;
        if(realloc(employees, dbheader->count * sizeof(struct employee_t)) == NULL){
            return -1;
        } ;
        add_employee(dbheader,employees,addString);

    }
    output_file(database_fd, dbheader,employees);
      


    return 0;

}






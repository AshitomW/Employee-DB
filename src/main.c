#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>



void printInstructions(char *argv[]){
    printf("Usage: %s -n -f <database file>\n",argv[0]);
    printf("-n : create new database file \n");
    printf("-f : path to the database file (required)\n");
}



int main(int argc, char *argv[])
{
    char *filepath = NULL;
    bool newFile = false;
    int commands;
    while((commands = getopt(argc,argv,"nf:")) != -1){

        switch(commands){
            case 'n':
               newFile = true;
               break;
            case 'f':
                filepath = optarg;
                break;
            case '?':
                printf("Unknown Option -%c\n",commands);
                break;
            default:
                return -1;
        }

    }

    if(filepath == NULL){
        printf("Filepath is a required argument!\n");
        printInstructions(argv);
    }

    return 0;

}






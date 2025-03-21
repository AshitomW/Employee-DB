#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>




struct option long_options []={
    {"help",no_argument,0,'h'},
    {"newfile",no_argument,0,'n'},

    {"filepath",required_argument,0,'f'},
    {0,0,0,0}
};


void printUsageInstructions(char *argv[]){
    printf("Usage : %s --file=<DB-FilePath> [-n]\n",argv[0]);
    printf("\t -f , --filename \tSpecify the input filepath (required)\n");
    printf("\t -n , --newfile \tIndicates A New File");
}



int main(int argc, char *argv[])
{
    
    
    int opts;
    int option_index = 0;


    bool isNewFile = false;
    char *filepath = NULL;
    
    while((opts = getopt_long(argc,argv,"hnf:",long_options,&option_index)) != -1){
    
        switch(opts){
        case 'h':
        printUsageInstructions(argv);
        break;
        case 'n':
            isNewFile = true;
            break;
         case 'f':
            filepath = optarg;
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
        
    }

    printf("filepath : %s\n",filepath);
    printf("New File: %d\n",isNewFile);

    
    return 0;

}






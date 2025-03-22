#ifndef PARSER_H
#define PARSER_H

#define HEADER_MAGIC 0x43584852


struct dbheader_t{
    unsigned int magic;
    unsigned short version;
    unsigned short count;
    unsigned int filesize;
};


struct employee_t{
    char name[512];
    char address[512];
    unsigned int hours;
};

int create_database_header(int fd,struct dbheader_t** headerOut);
int validate_database_header(int fd,struct dbheader_t** headerOut);




int output_file(int fd, struct dbheader_t*);


#endif

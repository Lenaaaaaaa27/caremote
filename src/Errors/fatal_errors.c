//
// Created by Lena on 27/11/2023.
//
// this file handles fatal errors, displaying the error + error code, writing to the log file and stopping the program.

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../../includes/define.h"

// by Lena
//returns the error message associated with the error set in parameter
void error_content(int code){
    char* content;
    char* error_message;
    switch (code) {
        case 101 :
            content = "Error 101 : Echec SQL\n";
            break;
    }
    error_message = line_formatting(content);
    handle_error(error_message);
}

// by Lena
void handle_error(const char*error_message){
    write_log(error_message);
    printf("%s\n", error_message);
    exit(EXIT_FAILURE);
}

// by Arthur
//write the formatting error message to the log file
void write_log(const char *error_message) {
    FILE *log;
    log = fopen("../log-errors.txt","a");
    if(log != NULL) {
        fprintf(log,error_message);
        fclose(log);
    }
}

// by Arthur
// add the current date and time to the error message and returns the string
char * line_formatting(const char* content){
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char time[80];
    strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S - ", timeinfo);

    strcat(time,content);

    char * line = malloc(strlen(time) * sizeof(char));
    if(line != NULL)
        strcpy(line,time);
    return line;
}
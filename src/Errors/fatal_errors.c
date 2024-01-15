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
            content = "Error 101 : Failure SQL\n";
            break;
        case 105 :
            content = "Error 105 : memory allocation error\n";
            break;
        case 200 :
            content = "Error 200 : Car connection failure\n";
            break;
        case 300 :
            content = "Error 300 : Failure to export sessions\n";
            break;
        case 301 :
            content = "Error 301 : Failure to export configuration\n";
            break;
        case 310 :
            content = "Error 310 : Failed to open file for import\n";
            break;
        case 311 :
            content = "Error 311 : Failed to parse JSON export\n";
            break;
        case 312 :
            content = "Error 312 : The name attribute is invalid \n";
            break;
        case 313 :
            content = "Error 313 : The move_forward attribute is invalid \n";
            break;
        case 314 :
            content = "Error 314 : The move_backward attribute is invalid \n";
            break;
        case 315 :
            content = "Error 315 : The move_left attribute is invalid \n";
            break;
        case 316 :
            content = "Error 316 : the move_right attribute is invalid \n";
            break;
        case 317 :
            content = "Error 317 : The change_step_button attribute is invalid \n";
            break;
        case 318 :
            content = "Error 318 : The speed_step attribute is invalid \n";
            break;
        case 320 :
            content = "Error 320 : Error when opening the window  \n";
            break;
        default:
            content = "Error 0 : An unknown error has occurred\n";
            break;
    }
    error_message = line_formatting(content);
    write_log(error_message);
    free(error_message);
}

// by Arthur
//write the formatting error message to the log file
void write_log(const char *error_message) {
    FILE *log;
    log = fopen("log-errors.txt", "a");
    if (log == NULL) {
        log = fopen("log-errors.txt", "w");
    }
    if (log != NULL) {
        fprintf(log, "%s\n", error_message);
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
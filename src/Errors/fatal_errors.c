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
    char content[256];
    char error_message[300];

    switch (code) {
        case 101 :
            strcpy(content, "Error 101 : Failure SQL");
            break;
        case 105 :
            strcpy(content, "Error 105 : memory allocation error");
            break;
        case 200 :
            strcpy(content, "Error 200 : Car connection failure");
            break;
        case 300 :
            strcpy(content, "Error 300 : Failure to export sessions");
            break;
        case 301 :
            strcpy(content,"Error 301 : Failure to export configuration");
            break;
        case 310 :
            strcpy(content,"Error 310 : Failed to open file for import");
            break;
        case 311 :
            strcpy(content,"Error 311 : Failed to parse JSON export");
            break;
        case 312 :
            strcpy(content,"Error 312 : The name attribute is invalid ");
            break;
        case 313 :
            strcpy(content, "Error 313 : The move_forward attribute is invalid ");
            break;
        case 314 :
            strcpy(content,"Error 314 : The move_backward attribute is invalid ");
            break;
        case 315 :
            strcpy(content,"Error 315 : The move_left attribute is invalid ");
            break;
        case 316 :
            strcpy(content,"Error 316 : the move_right attribute is invalid ");
            break;
        case 317 :
            strcpy(content,"Error 317 : The change_step_button attribute is invalid ");
            break;
        case 318 :
            strcpy(content,"Error 318 : The speed_step attribute is invalid ");
            break;
        case 320 :
            strcpy(content,"Error 320 : Error when opening the window  ");
            break;
        case 400 :
            strcpy(content,"Error 400 : Failure to read config.txt");
            break;
        case 401 :
            strcpy(content,"Error 401 : ForceFeedBack hasn't the right data in config.txt");
            break;
        case 402 :
            strcpy(content,"Error 402 : CarName is empty in config.txt");
            break;
        case 403 :
            strcpy(content,"Error 403 : profile id in config.txt doesn't exist");
            break;
        case 404 :
            strcpy(content,"Error 404 : maxSessionTime hasn't the right data in config.txt");
            break;
        case 405 :
            strcpy(content,"Error 405 : profileUsername is empty in config.txt");
            break;
        case 406 :
            strcpy(content, "Error 406 : configurationName is empty in config.txt");
            break;
        case 407 :
            strcpy(content,"Error 407 : configurationSpeedStep hasn't the right data in config.txt");
            break;
        case 408 :
            strcpy(content,"Error 408 : One or numeral configuration key(s) haven't the right data(s) in config.txt");
            break;
        case 500:
            strcpy(content,"Error 500 : Connection failed");
            break;
        default:
            strcpy(content,"Error 0 : An unknown error has occurred");
            break;
    }

    line_formatting(content, error_message);
    write_log(error_message);
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
void line_formatting(const char* content, char *line){
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char time[44];
    strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S - ", timeinfo);

    // Concatenate the time and content
    strcpy(line, time);
    strcat(line, content);
}
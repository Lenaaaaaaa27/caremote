//
// Created by Lena on 27/11/2023.
//
#include<stdlib.h>

void handle_error(const char*);

void error_content(int code){
    char* content;

    switch (code) {
        case 101 :
            content = "101 : Echec SQL";
            break;
    }

    handle_error(content);
}

void handle_error(const char*error_message){
    printf("%s\n", error_message);
    exit(EXIT_FAILURE);
}
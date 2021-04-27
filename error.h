#ifndef ERROR_H
#define ERROR_H

#define ERROR_MESSAGE_LENGTH 64

typedef struct Error {
    int code;
    char message[ERROR_MESSAGE_LENGTH];
} Err;

#endif
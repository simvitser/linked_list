#ifndef _COMMON_H
#define _COMMON_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define EPS 1e-9
#define STATIC_STRLEN(s) (sizeof(s) - 1)
#define STATIC_LEN(s) ((int)sizeof(s) / (int)sizeof(s[0]))

#define BLACK_TEXT     "\033[30m"
#define RED_TEXT       "\033[31m"
#define GREEN_TEXT     "\033[32m"
#define YELLOW_TEXT    "\033[33m"
#define BLUE_TEXT      "\033[34m"
#define PURPLE_TEXT    "\033[35m"
#define LIGHTBLUE_TEXT "\033[36m"
#define WHITE_TEXT     "\033[37m"
#define STANDART_TEXT  "\033[0m"

#define STRDEF(x) #x
#define STR(x) STRDEF(x)

#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define printfError(mes, ...) printf(  RED_TEXT  "[ERROR] " STANDART_TEXT mes, ##__VA_ARGS__);
#define printfOK(mes, ...)    printf(GREEN_TEXT     "[OK] " STANDART_TEXT mes, ##__VA_ARGS__);

#define FS_ERROR -1

void clearInputBuffer();

bool isZero (double n);
bool isEqual(double a, double b);

double randDouble();

uint64_t getHash(const void *buffer, const size_t size);

uint64_t getHashString(const char *s);

uint64_t getHash8char(const char *s);

typedef struct {
    char *str;
    size_t len;
} String;

ssize_t readFileToBuffer(const char *filename, char **buffer);

size_t parseBuffer(char *buffer, const size_t buffer_len, String **arr);

ssize_t getFileSize(int file_descriptor);
#endif

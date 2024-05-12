#pragma once

typedef struct {
    int count;
    char** files;
} FileArray;

typedef struct {
    char*   filename;
    char*   path;
    int     count;
    char**  content;
} Unit;

FileArray*  get_files(char* path);
Unit*       parse_file(char* file, char* path);


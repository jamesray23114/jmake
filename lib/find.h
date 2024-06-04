#pragma once
/*********************************************************************
 * This file: (find.h) 
 *  - Is apart of the jmake project, which can be found here: https://github.com/jamesray23114/jmake.
 *  - Is licensed under the MIT License, a copy of which can be found in the LICENSE file.
 *  - Was created on May 11 2024. 
 * It:
 *  - Is used to find files ending in ".jmake" or are named "jmakefile".  
**********************************************************************/

// == project includes ==
#include <typedef.h>

// == standard includes ==
#include <cstdio>
#include <cstdlib>
#include <cstring>

// == system includes ==
// n/a

// ============================= 
// ======== definitions ========
// ============================= 

// class FileArray
//  - stores an array of file names
class FileArray { 
    
    public:
        uint count; // number of files in the array
        char** files; // array of file names

    // constructors
    FileArray(); 
    FileArray(uint count, char** files); 

    // log the content of the FileArray to the given file descriptor
    void logContent(FILE* fd); 

    // log the content of the FileArray to the given path, creating the file if it does not exist
    void logContent(const char* path); 
};

// class Unit
//  - stores the tokenized content of a file and its location
class Unit { 
    public:
        const char*     filename; // name of the file
        const char*     path; // path to the file
        uint            count; // number of tokens in the file
        char**          content; // array of tokens

    // constructorS
    Unit(const char* filename, const char* path, uint count, char** content);

    // log the content of the Unit to the given file descriptor
    void logContent(FILE* fd);

    // log the content of the Unit to the given path, creating the file if it does not exist
    void logContent(const char* path);
};

// function get_files
//  - find all files ending with .jmake or named jmakefile in the given path
FileArray*  get_files(const char* path);

// function lex_file
//  - run the lexer on the file at the given path, returning a Unit containing the tokenized content
Unit*       lex_file(const char* filename, const char* path);

#define EOP (char*) null // end of program
#define EOL (char*) 1    // end of line
#define EOT (char*) 2    // end of token

// =============================
// ====== implementations ======
// ============================= 

// == class FileArray ==
inline FileArray::FileArray() {
    this->count = 0u;
    this->files = (char**) null;
}

inline FileArray::FileArray(uint count, char** files) {
    this->count = count;
    this->files = files;
}

inline void FileArray::logContent(FILE* fd) {
    fprintf(fd, "FileArray contains %u files\n", this->count);
    fprintf(fd, "Files:\n");

    for (uint i = 0; i < this->count; i++) {
        fprintf(fd, "\t%s\n", this->files[i]);
    }
}

inline void FileArray::logContent(const char* path) {
    FILE* fd = fopen(path, "w");
    this->logContent(fd);
    fclose(fd);
}

// == class Unit ==
inline Unit::Unit(const char* filename, const char* path, uint count, char** content) {
    this->filename = filename;
    this->path = path;
    this->count = count;
    this->content = content;
}

inline void Unit::logContent(FILE* fd) {
    fprintf(fd, "Unit %s%s contains %u tokens\n", this->path, this->filename, this->count);
    fprintf(fd, "Tokens:\n");

    for (uint i = 0; i < this->count; i++) {
        if (this->content[i] == EOL) { fprintf(fd, "\tEOL\n"); continue; }
        if (this->content[i] == EOP) { fprintf(fd, "\tEOP\n"); continue; }
        if (this->content[i] == EOT) { fprintf(fd, "\tEOT\n"); continue; }

        fprintf(fd, "\t%s\n", this->content[i]);
    }
}

inline void Unit::logContent(const char* path) {
    FILE* fd = fopen(path, "w");
    this->logContent(fd);
    fclose(fd);
}
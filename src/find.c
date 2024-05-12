#include <typedef.h>
#include <find.h>
#include <parse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <dirent.h>

// find all files ending with .jmake or named jmakefile in the given path 
FileArray* get_files(char* path) {
    
    FileArray* files = malloc(sizeof(FileArray)); 

    files->count = 0;
    files->files = malloc(sizeof(char*) * 4); // reserve 4 files, realloc if needed
    int size = 4;

    // open the directory
    DIR* dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Could not open directory %s\n", path);
        exit(1);
    }

    // read the names of all files in the directory
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {

        if(entry->d_type == DT_REG) { // check if the entry is a regular file
            if (strcmp(entry->d_name, "jmakefile") == 0) { // check if the file is named jmakefile

                if (files->count - 1 == size) { // realloc
                    size *= 2;
                    files->files = realloc(files->files, sizeof(char*) * size);
                }

                files->files[files->count] = entry->d_name;
                files->count++;
            }

            char* ext = entry->d_name;

            // find the last dot in the file name, i.e "file.a.b" -> "b"
            // we dont want to match files like "file.jmake.lock" 
            while((ext = strrchr(ext, '.')) != NULL) { 
                ext++; // skip the dot
                
                if (files->count - 1 == size) { // realloc
                    size *= 2;
                    files->files = realloc(files->files, sizeof(char*) * size);
                }

                if (strcmp(ext, "jmake") == 0) { // check if the file ends with .jmake
                    files->files[files->count] = entry->d_name;
                    files->count++;
                }
            }
        }
    }

    return files;
}

static char* next(char** buf) {
    char* curtok = *buf;
    
    if (*curtok == '\0') {
        return NULL;
    }

    if (*curtok == '#') { // skip comments
        for(; *curtok != '\n'; curtok++) 
            if (*curtok == '\0') 
                return NULL;
        
        *buf = curtok;
        for(; isspace(**buf); (*buf)++); // skip whitespace

        return next(buf);
    }

    if (!isalnum(*curtok)) { 
        curtok++;
        goto end;
    }

    for(; !isspace(*curtok); curtok++) { // find the end of the token      
        if (!isalnum(*curtok)) { 
            break;
        }
    }

end: ;
    // copy the token into a new buffer
    int size = curtok - *buf;    
    curtok = malloc(size + 1);
    strncpy(curtok, *buf, size);
    curtok[size] = '\0';

    *buf += size;
    for(; isspace(**buf); (*buf)++); // skip whitespace

    return curtok;
}

Unit* parse_file(char* filename, char* path) {
    
    char* infile = malloc(strlen(path) + strlen(filename) + 1);
    infile = strcat(infile, path);
    infile = strcat(infile, filename);
    infile[strlen(path) + strlen(filename)] = '\0';

    Unit* unit = malloc(sizeof(Unit));
    unit->filename = filename;
    unit->path = path;
    unit->count = 0;
    unit->content = malloc(sizeof(char*) * 32); // reserve 32 tokens, realloc if needed
    int size = 32;

    FILE* file = fopen(infile, "r"); // open file for reading
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(1);
    }

    // get the size of the file
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // read the file into a buffer
    char* buffer = malloc(fsize + 1);
    fread(buffer, 1, fsize, file);
    fclose(file);
    buffer[fsize] = '\0';


    // parse the buffer into tokens
    char* token;
    while(token = next(&buffer)) {
        if (unit->count - 1 == size) {
            size *= 2;
            unit->content = realloc(unit->content, sizeof(char*) * size);
        }

        unit->content[unit->count] = token;
        unit->count++;
    }

    return unit;
}

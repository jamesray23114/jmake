/*********************************************************************
 * This file: (find.cc) 
 *  - Is apart of the jmake project, which can be found here: https://github.com/jamesray23114/jmake.
 *  - Is licensed under the MIT License, a copy of which can be found in the LICENSE file.
 *  - Was created on May 11 2024. 
 * It:
 *  - Impliments the functions declared in find.h.
**********************************************************************/

// == project includes ==
#include <typedef.h>
#include <find.h>
#include <parse.h>

// == standard includes ==
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

// == system includes ==
#include <dirent.h>

// =============================
// ======== definitions ========
// =============================

// function next
// - get the next token from the passed buffer
static char* next(char** buf);

// variable tokenended
// - flag to check if the token has ended, used in function next
static bool tokenended = false;

// =============================
// ====== implementations ======
// =============================
FileArray* get_files(const char* path) { // find all files ending with .jmake or named jmakefile in the given path 
    
    FileArray* t_files = new FileArray(); 

    t_files->count = 0;
    t_files->files = (char**) malloc(sizeof(char*) * 4); // reserve 4 files, realloc if needed
    uint size = 4;

    // open the directory
    DIR* dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Could not open directory %s\n", path);
        exit(1);
    }

    // read the names of all files in the directory
    dirent* entry;
    while ((entry = readdir(dir)) != NULL) {

        if(entry->d_type == DT_REG) { // check if the entry is a regular file
            if (strcmp(entry->d_name, "jmakefile") == 0) { // check if the file is named jmakefile

                if (t_files->count - 1u == size) { // realloc
                    size *= 2;
                    t_files->files = (char**) realloc(t_files->files, sizeof(char*) * size);
                }

                t_files->files[t_files->count] = entry->d_name;
                t_files->count++;
            }

            char* ext = entry->d_name;

            // find the last dot in the file name, i.e "file.a.b" -> "b"
            // we dont want to match files like "file.jmake.lock" 
            while((ext = strrchr(ext, '.')) != NULL) { 
                ext++; // skip the dot
                
                if (t_files->count - 1u == size) { // realloc
                    size *= 2;
                    t_files->files = (char**) realloc(t_files->files, sizeof(char*) * size);
                }

                if (strcmp(ext, "jmake") == 0) { // check if the file ends with .jmake
                    t_files->files[t_files->count] = entry->d_name;
                    t_files->count++;
                }
            }
        }
    }

    return t_files;
}

static char* next(char** buf) {
    char* curtok = *buf;
    
    // return End Of Token if we are at the end of the token
    if(tokenended) {
        tokenended = false;
    
        // determine if we are at the end of the token via whitespace
        if(isspace(*curtok) || *curtok == '\0') { // apperently the null character is not considered whitespace
            for(; isspace(**buf) && **buf != '\n'; (*buf)++); // skip whitespace, but not newlines as they will be handled by the next call to next
            return EOT;
        }
    }

    // return End Of Program if we are at the end of the buffer
    if (*curtok == '\0') {
        return EOP;
    }

    // return End Of Line if we are at the end of the line
    if (*curtok == '\n') {

        *buf = curtok + 1;
        for(; isspace(**buf); (*buf)++); // skip whitespace

        return EOL;
    }

    if (*curtok == '#') { // skip comments
        for(; *curtok != '\n'; curtok++) 
            if (*curtok == '\0') 
                return EOL; // EOP will be caught by the next call to next
        
        *buf = curtok;
        for(; isspace(**buf); (*buf)++); // skip whitespace

        return EOL;
    }

    // only return a single character if the current character is not alphanumeric
    if (!isalnum(*curtok)) { 
        tokenended = true;

        curtok++;
        goto end;
    }


    for(; !isspace(*curtok); curtok++) { // find the end of the token      
        tokenended = true;
        
        if (!isalnum(*curtok)) { 
            break;
        }
    }

end: ;
    // copy the token into a new buffer

    int size = curtok - *buf;    

    curtok = (char*) malloc(size + 1);
    strncpy(curtok, *buf, size);
    curtok[size] = '\0';

    *buf += size;
    //for(; isspace(**buf) && **buf != '\n'; (*buf)++); // skip whitespace, but not newlines
    
    return curtok;
}

Unit* lex_file(const char* filename, const char* path) {
    
    char* infile = (char*) malloc(strlen(path) + strlen(filename) + 1);
    infile = strcat(infile, path);
    infile = strcat(infile, filename);
    infile[strlen(path) + strlen(filename)] = '\0';

    Unit* unit = new Unit(
        filename,
        path,
        0, // count
        (char**) malloc(sizeof(char*) * 32) // buffer
                                            // reserve 32 tokens, realloc if needed
    );
    uint size = 32;

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
    char* buffer = (char*) malloc(fsize + 1);
    fread(buffer, 1, fsize, file);
    fclose(file);
    buffer[fsize] = '\0';

    // parse the buffer into tokens
    char* token;
    while((token = next(&buffer)) != EOP) { 
        if (unit->count - 1u == size) {
            size *= 2;
            unit->content = (char**) realloc(unit->content, sizeof(char*) * size);
        }

        unit->content[unit->count] = token;
        unit->count++;
    }

    if (unit->count - 1u == size) { // we have to recheck the size because the last token may have been added
        size *= 2;
        unit->content = (char**) realloc(unit->content, sizeof(char*) * size);
    }

    if (unit->content[unit->count - 1] != EOL) { // add an EOL token to the end of the file if there is not one
        unit->content[unit->count] = EOL;
        unit->count++;
    }

    unit->content[unit->count] = EOP; // add an EOP token to the end of the file
    unit->count++;

    return unit;
}

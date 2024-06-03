#include <typedef.h>
#include <find.h>
#include <parse.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include <dirent.h>

// find all files ending with .jmake or named jmakefile in the given path 
FileArray* get_files(const char* path) {
    
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
    
    if (*curtok == '\0') {
        return EOP;
    }

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
    curtok = (char*) malloc(size + 1);
    strncpy(curtok, *buf, size);
    curtok[size] = '\0';

    *buf += size;
    for(; isspace(**buf) && **buf != '\n'; (*buf)++); // skip whitespace, but not newlines

    return curtok;
}

Unit* parse_file(const char* filename, const char* path) {
    
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
    while((token = next(&buffer)) != EOP) { // 
        if (unit->count - 1u == size) {
            size *= 2;
            unit->content = (char**) realloc(unit->content, sizeof(char*) * size);
        }

        unit->content[unit->count] = token;
        unit->count++;
    }

    return unit;
}

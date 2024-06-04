/*********************************************************************
 * This file: (run.cc) 
 *  - Is apart of the jmake project, which can be found here: https://github.com/jamesray23114/jmake.
 *  - Is licensed under the MIT License, a copy of which can be found in the LICENSE file.
 *  - Was created on May 11 2024. 
 * It:
 *  - Implements the functions defined in run.h.
**********************************************************************/

// == project includes ==
#include <typedef.h>
#include <run.h>

// == standard includes ==
#include <cstdio>
#include <cstdlib>
#include <cstring>

// == system includes ==
// n/a

// =============================
// ======== definitions ========
// =============================
#define try_cmd(cmd)                         \
    if (strcmp(unit->content[0], #cmd) == 0) \
        return try_##cmd(unit, file);

static int next(Unit* unit, char* file) {

    uint64 start_pos = (uint64) unit->content;

    if (unit->content[0] == EOP) {
        unit->content++;
        return 0;
    }

    if (unit->content[0] == EOL) {
        fprintf(stderr, "Unexpected EOL\n");
        return 1;
    }

    try_cmd(run);
    try_cmd(compile);
    try_cmd(build);

    if ((uint64) unit->content == start_pos) {
        printf("Unknown command: %s\n", unit->content[0]);
        return 1;
    }

    return 0;
}

// =============================
// ====== implementations ======
// =============================
int script_exec(Unit unit) { 
    char* filepath = (char*) calloc(strlen(unit.path) + strlen(unit.filename) + 1, 1);
    filepath = strcat(filepath, unit.path);
    filepath = strcat(filepath, unit.filename);
    filepath[strlen(unit.path) + strlen(unit.filename)] = '\0';

    void* end = unit.content + unit.count;

    int ret = 0;
    while(((uint64) unit.content < (uint64) end)) if((ret = next(&unit, filepath))) return ret;
    
    return 0;
}

int try_run(Unit* unit, char* file) {
    char* noext = (char*) malloc(strrchr(file, '.') - file + 1);
    memmove(noext, file, strrchr(file, '.') - file);
    noext[strrchr(file, '.') - file] = '\0';

    char* cmd = NULL;

    if(unit->content[1] == EOL) { // no args
        cmd = (char*) calloc(strlen(noext) + 3, 1); // length of the filename + 3 for "./@\0"   

        strcat(cmd, "./");
        strcat(cmd, noext);
        strcat(cmd, "\0");

        unit->content += 2;
    }

    return system(cmd);
}

int try_compile(Unit* unit, char* file) {
    char* noext = (char*) malloc(strrchr(file, '.') - file + 1);
    memmove(noext, file, strrchr(file, '.') - file);
    noext[strrchr(file, '.') - file] = '\0';

    char* cmd = NULL;
    
    if(unit->content[1] == EOL) { // no args
        cmd = (char*) calloc(strlen(noext) * 2 + 16, 1); // 2x the length of the filename + 16 for "gcc -c @.c -o @.o\0" 
    
        strcat(cmd, "cc -c ");
        strcat(cmd, noext);
        strcat(cmd, ".c -o ");
        strcat(cmd, noext);
        strcat(cmd, ".o\0");

        unit->content += 2;
    }

    return system(cmd);
}

// todo: inpmement build with args
int try_build(Unit* unit, char* file) {

    char* noext = (char*) malloc(strrchr(file, '.') - file + 1);
    memmove(noext, file, strrchr(file, '.') - file);
    noext[strrchr(file, '.') - file] = '\0';

    char* cmd = NULL; 
    
    if(unit->content[1] == EOL) { // no args
        cmd = (char*) calloc(strlen(noext) * 2 + 11, 1); // 2x the length of the filename + 11 for "gcc @.c -o @\0" 
    
        strcat(cmd, "cc ");
        strcat(cmd, noext);
        strcat(cmd, ".c -o ");
        strcat(cmd, noext);
        strcat(cmd, "\0");

        unit->content += 2;
    }

    return system(cmd);
}


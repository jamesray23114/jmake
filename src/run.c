#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <typedef.h>
#include <run.h>

#define try_cmd(cmd)                         \
    if (strcmp(unit->content[0], #cmd) == 0) \
        return try_##cmd(unit, file);

int try_run(Unit* unit, char* file);
int try_compile(Unit* unit, char* file);
int try_build(Unit* unit, char* file);

static int next(Unit* unit, char* file) {
    
    try_cmd(run);
    try_cmd(compile);
    try_cmd(build);
    return 0;
}

int script_exec(Unit unit) {
    char* filepath = calloc(strlen(unit.path) + strlen(unit.filename) + 1, 1);
    filepath = strcat(filepath, unit.path);
    filepath = strcat(filepath, unit.filename);
    filepath[strlen(unit.path) + strlen(unit.filename)] = '\0';

    void* end = unit.content + unit.count;
    while((uint64) unit.content < (uint64) end) next(&unit, filepath);
    
    return 0;
}

int try_run(Unit* unit, char* file) {
    char* noext = malloc(strrchr(file, '.') - file + 1);
    memmove(noext, file, strrchr(file, '.') - file);
    noext[strrchr(file, '.') - file] = '\0';

    char* cmd = calloc(strlen(noext) + 3, 1); // length of the filename + 3 for "./@\0"   

    strcat(cmd, "./");
    strcat(cmd, noext);
    strcat(cmd, "\0");

    unit->content++;

    return system(cmd);
}

int try_compile(Unit* unit, char* file) {
    printf("todo: compile\n");
    return 0;
}

// todo: inpmement build with args
int try_build(Unit* unit, char* file) {

    char* noext = malloc(strrchr(file, '.') - file + 1);
    memmove(noext, file, strrchr(file, '.') - file);
    noext[strrchr(file, '.') - file] = '\0';

    char* cmd = calloc(strlen(noext) * 2 + 11, 1); // 2x the length of the filename + 11 for "gcc @.c -o @\0" 
    
    strcat(cmd, "gcc ");
    strcat(cmd, noext);
    strcat(cmd, ".c -o ");
    strcat(cmd, noext);
    strcat(cmd, "\0");

    unit->content++;

    return system(cmd);
}


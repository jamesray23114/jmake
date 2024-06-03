#pragma once

#include <find.h>

int script_exec(Unit unit); // run the unit, return 0 if successful, otherwise return the exit code of the failed process 

int try_run(Unit* unit, char* file);
int try_compile(Unit* unit, char* file);
int try_build(Unit* unit, char* file);
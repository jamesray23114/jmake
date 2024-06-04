#pragma once
/*********************************************************************
 * This file: (run.h) 
 *  - Is apart of the jmake project, which can be found here: https://github.com/jamesray23114/jmake.
 *  - Is licensed under the MIT License, a copy of which can be found in the LICENSE file.
 *  - Was created on May 11 2024.
 * It:
 * - Attempts to run the commands stored in the jmake file.
**********************************************************************/

// == project includes ==
#include <typedef.h>
#include <find.h>

// == standard includes ==
// n/a

// == system includes ==
// n/a

// =============================
// ======== definitions ========
// =============================

// function script_exec
// - run the unit, return 0 if successful, otherwise return the exit code of the failed process
int script_exec(Unit unit); 

//function try_run
// - try to run the given file, return 0 if successful, otherwise return the exit code of the failed process
int try_run(Unit* unit, char* file);

// function try_compile
// - try to compile the given file, return 0 if successful, otherwise return the exit code of the failed process
int try_compile(Unit* unit, char* file);

// function try_build
// - try to build the given file, return 0 if successful, otherwise return the exit code of the failed process
int try_build(Unit* unit, char* file);

// =============================
// ====== implementations ======
// =============================
// n/a
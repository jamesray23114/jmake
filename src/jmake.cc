/*********************************************************************
 * This file: (jmake.cc) 
 *  - Is apart of the jmake project, which can be found here: https://github.com/jamesray23114/jmake.
 *  - Is licensed under the MIT License, a copy of which can be found in the LICENSE file.
 *  - Was created on May 11 2024. 
 * It:
 * - Is the main file for the jmake project.
 * - Contains the entry point for jmake.
**********************************************************************/

// == project includes ==
#include <typedef.h>
#include <find.h>
#include <run.h>

// == standard includes ==
#include <cstdio>

// == system includes ==
// n/a

// =============================
// ======== definitions ========
// =============================
// n/a

// =============================
// ====== implementations ======
// =============================
int main(int argc, char** argv) {

    FileArray files = *get_files(".temp/");
    
    for (uint i = 0; i < files.count; i++) {
        Unit* u = lex_file(files.files[i], ".temp/");

        u->logContent(stdout);

        script_exec(*u);
    }
    
    return 0;
}

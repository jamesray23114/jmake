#include <stdio.h>

#include <typedef.h>
#include <find.h>
#include <run.h>

int main(int argc, char** argv) {

    FileArray files = *get_files(".temp/");
    
    for (uint i = 0; i < files.count; i++) {
        Unit u = *parse_file(files.files[i], ".temp/");

        script_exec(u);
    }
    
    return 0;
}

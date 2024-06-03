#pragma once



class FileArray {
    public:
        uint count;
        char** files;

    FileArray() {
        this->count = 0u;
        this->files = (char**) null;
    }

    FileArray(uint count, char** files) {
        this->count = count;
        this->files = files;
    }

    void logContent(int fd);
    void logContent(const char* path);
};

class Unit {
    public:
        const char*     filename;
        const char*     path;
        uint            count;
        char**          content;

    Unit(const char* filename, const char* path, uint count, char** content) {
        this->filename = filename;
        this->path = path;
        this->count = count;
        this->content = content;
    }

    void logContent(int fd);
    void logContent(const char* path);
};

FileArray*  get_files(const char* path);
Unit*       parse_file(const char* file, const char* path);

#define EOP (char*) null
#define EOL (char*) 1
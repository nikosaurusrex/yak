#include "io.h"

#include "common.h"

char *read_entire_file(const char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (!file) return 0;

    fseek(file, 0, SEEK_END);
    auto length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = new char[length + 1];

    fread(data, 1, length, file);
    data[length] = 0;

    fclose(file);

    return data;
}
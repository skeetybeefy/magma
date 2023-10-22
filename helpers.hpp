#include <string>

#ifndef helpers_h
#define helpers_h

int find_index(char* args[], int len, std::string item) {
    for (unsigned int i = 0; i < len; i++) {
        if (args[i] == item) {
            return i;
        }
    }
    return -1;
}

#endif

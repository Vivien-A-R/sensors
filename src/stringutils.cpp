#include "stringutils.h"
#include <Arduino.h>

bool startswith(const char *s, const char *p) {
    while (*p != '\0') {
        if (*s != *p) {
            return false;
        }

        s++;
        p++;
    }

    return true;
}

bool matches(const char *s1, const char *s2) {
    return strcmp(s1, s2) == 0;
}

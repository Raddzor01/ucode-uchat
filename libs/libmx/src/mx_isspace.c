#include "libmx.h"

bool mx_isspace(char c) {
    return (c == '\t'
            || c == '\n'
            || c == '\v' 
            || c == '\f' 
            || c == '\r' 
            || c == ' ' 
            ? 1 : 0);
}

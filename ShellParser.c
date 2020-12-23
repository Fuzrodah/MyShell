

#include "Headers/ShellParser.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

size_t split(char* input, char* output[], size_t output_size) {
    char* p, * start_of_word;
    start_of_word = 0;
    char c;
    enum states { DULL, IN_WORD, IN_STRING, IN_FUNC } state = DULL;
    size_t argc = 0;
    int inQuotes = 0;
    for (p = input; argc < output_size && *p != '\0'; p++) {
        c = *p;
        switch (state) {
        case DULL:
            if (isspace(c)) {
                continue;
            }
            if (c == '"') {
                state = IN_STRING;
                start_of_word = p+1;
                continue;
            }
            if (c == '$') {
                state = IN_FUNC;
                start_of_word = p;
                continue;
            }
            state = IN_WORD;
            start_of_word = p;
            continue;
        case IN_STRING:
            if (c == '"') {
                *p=0;
                char buffer[80];
                strcpy(buffer, start_of_word);
                strcpy(output[argc++], buffer);
                state = DULL;
            }
            continue;
        case IN_WORD:
            if (isspace(c)) {
                *p = 0;
                strcpy(output[argc++], start_of_word);
                state = DULL;
                continue;
            }
            if (c == '"') {
                state = IN_STRING;
                continue;
            }
            continue;
        case IN_FUNC:
            if(c == '(')
                inQuotes = 1;
            if(c == ')' && inQuotes == 1) {
                *p=0;
                char buffer[80];
                strcpy(buffer, start_of_word);
                strcat(buffer, ")");
                strcpy(output[argc++], buffer);

                state = DULL;
                continue;
            } else if (isspace(c) && inQuotes == 0) {
                *p = 0;
                strcpy(output[argc++], start_of_word);
                state = DULL;
                inQuotes = 0;
                continue;
            }

            continue;;
        }
    }
    if (state != DULL && argc < output_size)
        strcpy(output[argc++], start_of_word);
    return argc;
}

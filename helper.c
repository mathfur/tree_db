#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edge.h"

#define NODE_SIZE 1000

size_t get_pairs_from_query_string(char *query, char *key, char *output, size_t max_len){
    char* query_ = (char*) malloc(sizeof(char) * (strlen(query) + 3));
    sprintf(query_, "&%s&", query);

    char* key_ = (char*) malloc(sizeof(char) * (strlen(key) + 3));
    sprintf(key_, "&%s=", key);

    int len = 0;

    char *key_location;
    if(key_location = strstr(query_, key_)){
        key_location += strlen(key_);
        strcpy(output, key_location);

        for(len=0;len < strlen(output);len++){
            if(output[len] == '&'){
                output[len] = '\0';
                break;
            }
        }
    }

    free(query_);
    free(key_);

    return len;
}

size_t get_substring_by_start_and_end(char *s, char start, char end, char *result, size_t max_len){
    if(*s == '\0'){
        *result = '\0';
        return 0;
    }

    int inside_expect = 0;
    int result_len = 0;
    int string_len = strlen(s);

    int i;
    for(i=0;i < string_len;i++){
        if(!inside_expect && (*s == start)){
            inside_expect = 1;
            s++;
            continue;
        }

        if(inside_expect && (*s == end)){
            *result = '\0';
            break;
        }

        if(inside_expect){
            *result = *s;
            result++;
            result_len++;
        }

        s++;
    }

    return result_len;
}

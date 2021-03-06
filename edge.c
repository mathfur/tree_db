#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edge.h"

#define LARGE_NUMBER (1000*1000)

typedef struct _HashElem {
    Edge* edge;
    struct _HashElem* next;
} HashElem;

struct _HashTable {
    HashElem* start_table;
    HashElem* end_table;
    int size;
};

//=================================================

static int inspect_edge(Edge *edge, char *buf, int max_length){
    if(strlen(edge->start) + strlen(edge->end) >= max_length){
        return -1;
    }

    strcpy(buf, edge->start);
    strcat(buf, ",");
    strcat(buf, edge->end);
    return strlen(buf);
}

static int is_equal_edge(Edge *edge1, Edge *edge2){
    if((edge1->start == edge2->start) && (edge1->end == edge2->end)) {
        return 1;
    } else {
        return 0;
    }
}

static int get_key(int table_size, char *s){
    int sum=0;
    while(*s != '\0'){
        sum += (int)*s;
        s++;
    }
    return sum % table_size;
}

static void free_hash_elem(HashElem *hash_elem, int free_edge_also){
    if(hash_elem->next){
        free_hash_elem(hash_elem->next, free_edge_also);
    }

    if(hash_elem->edge){
        if(free_edge_also){
            free(hash_elem->edge->start);
            free(hash_elem->edge->end);
            free(hash_elem->edge);
        }
        free(hash_elem);
    }
}

void free_table(HashTable *tbl){
    int i;
    for(i = 0;i < tbl->size;i++){
        free_hash_elem(&(tbl->start_table[i]), 1);
        free_hash_elem(&(tbl->end_table[i]), 0);
    }
    free(tbl->start_table);
    free(tbl->end_table);
    free(tbl);
}

static void add_edge(HashElem *hash_elem, Edge *edge){
    while(hash_elem->next){
        hash_elem = hash_elem->next;
    }

    HashElem* next_hash_elem1 = (HashElem *) malloc(sizeof(HashElem));
    next_hash_elem1->edge = edge;
    next_hash_elem1->next = NULL;

    hash_elem->next = next_hash_elem1;
}

static int get_edges_by_start(HashTable *tbl, char *start, Edge *edges, int max_num_of_result){
    int key = get_key(tbl->size, start);
    HashElem *hash_elem = &tbl->start_table[key];

    int count = 0;
    while(hash_elem = hash_elem->next){
        if(!strcmp(hash_elem->edge->start, start)){
            *edges = *(hash_elem->edge);
            edges++;
            count++;
            if(max_num_of_result == count){
                printf("Node length is too long.");
                return -1;
            }
        }
    }

    return count;
}

static int search_edges_by_start_and_end(HashTable *tbl, char *start, char *end, Edge *edges, int max_num_of_result){
    int i, len, count;

    Edge* edges_got_by_start = (Edge*) malloc(sizeof(Edge) * max_num_of_result);

    count = 0;
    if(0 < (len = get_edges_by_start(tbl, start, edges_got_by_start, max_num_of_result))){
        for(i=0;i<len;i++){
            if(!strcmp((edges_got_by_start+i)->end, end)){
                *edges = *(edges_got_by_start+i);
                edges++;
                count++;
            }
        }
    }

    free(edges_got_by_start);

    return count;
}

//=================================================

HashTable* init_table(int size){
    HashTable* tbl = (HashTable*) malloc(sizeof(HashTable));

    tbl->start_table = (HashElem*) malloc(sizeof(HashElem)*size);
    tbl->end_table = (HashElem*) malloc(sizeof(HashElem)*size);
    tbl->size = size;

    int i;
    for(i = 0;i < size;i++){
        tbl->start_table[i].edge = NULL;
        tbl->start_table[i].next = NULL;

        tbl->end_table[i].edge = NULL;
        tbl->end_table[i].next = NULL;
    }

    return tbl;
}

int add_edge_to_table(HashTable *tbl, char *start, char *end){
    Edge *edges = (Edge*) malloc(sizeof(Edge) * LARGE_NUMBER);
    int len = search_edges_by_start_and_end(tbl, start, end, edges, LARGE_NUMBER);
    free(edges);

    if(0 < len){
        return 0;
    }

    char* start_entity = (char*)malloc(sizeof(char) * (strlen(start) + 1));
    char* end_entity = (char*)malloc(sizeof(char) * (strlen(end) + 1));

    strcpy(start_entity, start);
    strcpy(end_entity, end);

    Edge* edge = (Edge*)malloc(sizeof(Edge));

    edge->start = start_entity;
    edge->end = end_entity;

    int start_key = get_key(tbl->size, edge->start);
    add_edge(&(tbl->start_table[start_key]), edge);

    int end_key = get_key(tbl->size, edge->end);
    add_edge(&(tbl->end_table[end_key]), edge);

    return 0;
}

static int get_descendants0(HashTable *tbl, char *center, int depth, Edge *result, int max_num_of_result, char* already_used_nodes){
    int idx = 0;

    if(depth < 0){
        return -1;
    }else if(depth == 0){
        return 0;
    }else{
        int j;
        char *ptr;
        for(j=0;j<BUFSIZ;j++){
            ptr = already_used_nodes + BUFSIZ*j;
            if(*ptr == '\0'){
                strcpy(ptr, center);
                break;
            }else if(!strcmp(ptr, center)){
                break;
            }
        }

        Edge edges[max_num_of_result];
        int len = get_edges_by_start(tbl, center, edges, max_num_of_result);

        int i;
        int continue_flag;
        for(i=0;i<len;i++){
            continue_flag = 0;

            if(max_num_of_result < i){
                return (-1);
            }

            result[idx] = edges[i];
            idx++;

            for(j=0;j<BUFSIZ;j++){
                ptr = already_used_nodes + BUFSIZ*j;

                if(!strcmp(ptr, edges[i].end)){
                    continue_flag = 1;
                }
            }

            if(continue_flag){
                continue;
            }

            Edge child_result[max_num_of_result];
            int len_child = get_descendants0(tbl, edges[i].end, depth - 1, child_result, max_num_of_result, already_used_nodes);

            if(len_child < 0){
                return -1;
            }

            for(j=0;j<len_child;j++){
                if(max_num_of_result < j){
                    return (-1);
                }

                result[idx] = child_result[j];
                idx++;
            }
        }

        return idx;
    }
}

int get_descendants(HashTable *tbl, char *center, int depth, Edge *result, int max_num_of_result){
    char *buf = (char*) malloc(sizeof(char) * BUFSIZ * BUFSIZ);

    int i;
    for(i=0;i < BUFSIZ*BUFSIZ;i++){
        *(buf + i) = '\0';
    }

    int r = get_descendants0(tbl, center, depth, result, max_num_of_result, buf);
    free(buf);

    return r;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edge.h"

#define NODE_SIZE 1000

typedef struct _Edge {
        char *start;
        char *end;
} Edge;

typedef struct _HashElem {
    Edge* edge;
    struct _HashElem* next;
} HashElem;

struct _HashTable {
    HashElem* start_table;
    HashElem* end_table;
    int size;
};

int get_key(int table_size, char *s){
    int sum=0;
    while(*s != '\0'){
        sum += (int)*s;
        s++;
    }
    return sum % table_size;
}

HashTable* init_table(int size){
    HashTable* tbl = (HashTable*) malloc(sizeof(HashTable));

    tbl->start_table = (HashElem*) malloc(sizeof(HashElem)*size);
    tbl->end_table = (HashElem*) malloc(sizeof(HashElem)*size);

    int i;
    for(i = 0;i < size;i++){
        tbl->start_table[i].edge = NULL;
        tbl->start_table[i].next = NULL;

        tbl->end_table[i].edge = NULL;
        tbl->end_table[i].next = NULL;
    }

    return tbl;
}

void free_table(HashTable* tbl){
    free(tbl->start_table);
    free(tbl->end_table);
    free(tbl);
}

void add_edge(HashElem *hash_elem, Edge* edge){
    if(hash_elem->edge){
        HashElem* next_hash_elem1 = (HashElem *) malloc(sizeof(HashElem));

        next_hash_elem1->edge = edge;
        next_hash_elem1->next = NULL;

        while(hash_elem->next){
            hash_elem = hash_elem->next;
        }

        hash_elem->next = next_hash_elem1;
    }else{
        hash_elem->edge = edge;
    }
}

void add_edge_to_table(HashTable* tbl, char *start, char *end){
    char* start_entity = (char*)malloc(sizeof(char) * strlen(start));
    char* end_entity = (char*)malloc(sizeof(char) * strlen(end));

    strcpy(start_entity, start);
    strcpy(end_entity, end);

    Edge* edge = (Edge*)malloc(sizeof(Edge));

    edge->start = start_entity;
    edge->end = end_entity;


    int start_key = get_key(tbl->size, edge->start);
    add_edge(&tbl->start_table[start_key], edge);

    int end_key = get_key(tbl->size, edge->end);
    add_edge(&tbl->end_table[end_key], edge);
}

char** get_ends_by_start(HashTable* tbl, char *name){
    char** ends = (char**) malloc(sizeof(char) * NODE_SIZE);

    int key = get_key(tbl->size, name);

    HashElem *hash_elem = &tbl->start_table[key];

    if(hash_elem->edge == NULL){
        return 0;
    }

    int i = 0;
    do {
        ends[i] = hash_elem->edge->end;
        i++;
        if(NODE_SIZE == i){
            printf("Node length is too long.", NODE_SIZE);
            exit(-1);
        }
    }while((hash_elem = hash_elem->next) && (hash_elem->edge != NULL));

    ends[i] = NULL;

    return ends;
}

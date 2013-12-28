#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edge.h"

#define NODE_SIZE 1000

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

static void free_table(HashTable *tbl){
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
        *edges = *(hash_elem->edge);
        edges++;
        count++;
        if(max_num_of_result == count){
            printf("Node length is too long.");
            return -1;
        }
    }

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

void add_edge_to_table(HashTable *tbl, char *start, char *end){
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
}

int get_descendants(HashTable *tbl, char *center, int depth, Edge *result, int max_num_of_result){
    int idx = 0;

    if(depth < 0){
        return -1;
    }else if(depth == 0){
        return 0;
    }else{
        Edge edges[max_num_of_result];
        int len = get_edges_by_start(tbl, center, edges, max_num_of_result);

        int i, j;
        for(i=0;i<len;i++){
            result[idx] = edges[i];
            idx++;

            Edge child_result[max_num_of_result];
            int len_child = get_descendants(tbl, edges[i].end, depth - 1, child_result, max_num_of_result);
            for(j=0;j<len_child;j++){
                result[idx] = child_result[j];
                idx++;
            }
        }

        return idx;
    }
}

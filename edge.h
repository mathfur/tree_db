#ifndef __EDGE_H_INCLUDED_
#define __EDGE_H_INCLUDED_

typedef struct _HashTable HashTable;

typedef struct _Edge {
    char *start;
    char *end;
} Edge;

HashTable* init_table(int size);
int add_edge_to_table(HashTable* tbl, char *start, char *end);
int get_descendants(HashTable* tbl, char* center, int depth, Edge* result, int max_num_of_result);

#endif

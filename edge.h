#ifndef __EDGE_H_INCLUDED_
#define __EDGE_H_INCLUDED_

typedef struct _HashTable HashTable;

HashTable* init_table(int size);
void add_edge_to_table(HashTable* tbl, char *start, char *end);
char **get_ends_by_start(HashTable* tbl, char *name);

#endif

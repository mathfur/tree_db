#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "edge.h"
#include "tree.h"

#define HASH_TABLE_SIZE 10000

main(){
    Tree* t = create_simple_node(NULL);
    Tree* t1_1 = create_simple_node(NULL);
      Tree* t1_2 = create_simple_node("foo");
      Tree* t1_3 = create_simple_node("bar");
    Tree* t2_1 = create_simple_node(NULL);
      Tree* t2_2 = create_simple_node("baz");

    add_tree_to_node(t1_1, t1_2);
    add_tree_to_node(t1_1, t1_3);

    add_tree_to_node(t2_1, t2_2);

    add_tree_to_node(t, t1_1);
    add_tree_to_node(t, t2_1);

    display_tree(t);

    return 0;

    HashTable* tbl = init_table(HASH_TABLE_SIZE);

    char *fname = "tmp/data1.txt";

    int max_edges = 100;

    char s1[BUFSIZ], s2[BUFSIZ];
    FILE *f;
    if((f = fopen(fname, "r")) != NULL){
        while(fscanf(f, "%[^,\n],%[^,\n]\n",s1, s2 ) != EOF ){
            add_edge_to_table(tbl, s1, s2);
        }
    }else{
        printf( "File `%s` is not found.\n",  fname );
        return -1;
    }

    fclose(f);

    char** ends = get_ends_by_start(tbl, "e1");

    while(*ends != NULL){
        printf(">> %s\n", *ends);
        ends++;
    }

    printf("OK\n");
    return 0;
}

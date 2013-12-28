#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "edge.h"
#include "tree.h"
#include "server.h"

#define HASH_TABLE_SIZE 10000

main(){
    HashTable* tbl = init_table(HASH_TABLE_SIZE);

    char *fname = "tmp/data1.txt";
    int len, i;

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

    if(0){
        Edge result[100];
        len = get_descendants(tbl, "e2", 10, result, 100);

        for(i = 0;i < len;i++){
            printf("%s,%s\n", result[i].start, result[i].end);
        }

        return 0;
    }else{
        run_server(tbl);
        return 0;
    }
}

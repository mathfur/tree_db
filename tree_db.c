#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>

#include "edge.h"
#include "tree.h"
#include "server.h"

#define HASH_TABLE_SIZE 10000

void usage() {
    fprintf(stderr, "usage: \n");
    fprintf(stderr, " --port, -p      : http port number\n");
    fprintf(stderr, " --source, -s    : data input file path\n");
    fprintf(stderr, " --interface, -i : output destination, http or stdout\n");
    fprintf(stderr, " --help, -h      : show help\n");
}

int main(int argc, char *argv[]){
    int port = 12355;
    char source_fname[BUFSIZ];
    char interface[BUFSIZ] = "http";

    int opt;
    int option_index = 0;
    struct option long_options[] = {
        {"port",      required_argument, NULL, 'p'},
        {"source",    required_argument, NULL, 's'},
        {"interface", required_argument, NULL, 'i'},
        {"help",      no_argument,       NULL, 'h'}
    };

    while (( opt = getopt_long(argc, argv, "p:s:h", long_options, &option_index)) != -1)
    {
        switch(opt)
        {
            case 'p':
                port = atoi(optarg);

                if(port <= 0){
                    fprintf(stderr, "--interface has to be positive integer.\n");
                    exit(-1);
                }

                break;
            case 's':
                strcpy(source_fname, optarg);

                struct stat st;

                if(stat(source_fname, &st) < 0){
                    fprintf(stderr, "The file which specified by --source filename `%s` cannot be open.\n", source_fname);
                    exit(-1);
                }

                break;
            case 'i':
                if(strcmp(optarg, "stdout")){
                    strcpy(interface, "stdout");
                }else if(strcmp(optarg, "http")){
                    strcpy(interface, "http");
                }else{
                    fprintf(stderr, "--interface `%s` has to be stdout or http.\n", optarg);
                    exit(-1);
                }

                break;
            case 'h':
                usage();
                return 0;
            default:
                usage();
                return -1;
        }
    }

    printf("Port: %d\n", port);
    printf("Source: %s\n", source_fname);


    HashTable* tbl = init_table(HASH_TABLE_SIZE);

    int len, i;

    char s1[BUFSIZ], s2[BUFSIZ];
    FILE *f;
    if((f = fopen(source_fname, "r")) != NULL){
        while(fscanf(f, "%[^,\n],%[^,\n]\n",s1, s2 ) != EOF ){
            add_edge_to_table(tbl, s1, s2);
        }
    }else{
        printf( "File `%s` is not found.\n", source_fname );
        return -1;
    }

    fclose(f);

    if(interface == "stdout"){
        Edge result[100];
        len = get_descendants(tbl, "e2", 10, result, 100);

        for(i = 0;i < len;i++){
            printf("%s,%s\n", result[i].start, result[i].end);
        }

        return 0;
    }else{
        run_server(tbl, port);
        return 0;
    }
}

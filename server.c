#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "helper.h"
#include "server.h"
#include "edge.h"

#define MAX_NUM_OF_EDGES 10000

typedef struct _SearchOption {
    int depth;
    char center[BUFSIZ];
} SearchOption;

static SearchOption about_query(char *buf, int len){
    int flag = 0;

    char query[BUFSIZ];
    char buf2[BUFSIZ];
    int k = 0;

    int i;

    SearchOption search_option = {0, ""};

    get_substring_by_start_and_end(buf, '/', ' ', query, BUFSIZ);

    for(i=0;i<strlen(query);i++){
        if(query[i] == '?'){
            query[i] = '&';
            break;
        }
    }

    if(get_pairs_from_query_string(query, "depth", buf2, BUFSIZ) > 0){
        search_option.depth = atoi(buf2);
    }

    if(get_pairs_from_query_string(query, "center", buf2, BUFSIZ) > 0){
        strcpy(search_option.center, buf2);
    }

    return search_option;
}

int run_server(HashTable *tbl, int port) {
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    int yes = 1;
    
    /* ソケットの作成 */
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock0 < 0) {
            perror("socket");
            return 1;
    }
    
    /* ソケットの設定 */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    // addr.sin_len = sizeof(addr);

    setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes,  sizeof(yes));
    
    bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    
    /* TCPクライアントからの接続要求を待てる状態にする */
    listen(sock0, 5);
    
    while (1) {
        len = sizeof(client);
        sock = accept(sock0, (struct sockaddr *)&client,  &len);
        printf("accepted connection from %d, port=%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        char buf[BUFSIZ];
        char *ptr;
        
        len = recv(sock, buf, BUFSIZ, 0);

        SearchOption opt = about_query(buf, len);

        Edge result[MAX_NUM_OF_EDGES];
        len = get_descendants(tbl, opt.center, opt.depth, result, MAX_NUM_OF_EDGES);

        int i;
        for(i = 0;i < len;i++){
            // TODO: snprintfに修正する
            sprintf(buf, "%s,%s\n", result[i].start, result[i].end);
            write(sock, buf, strlen(buf));
        }
    
        write(sock, "\n\n", 1);
        close(sock);

#ifdef ONLY_ONE_RESPONSE
        printf("ONLY_ONE_RESPONSE\n");
        break;
#endif
    }
    
    /* TCPセッションの終了 */
    close(sock);
    
    /* listen するsocketの終了 */
    close(sock0);
    
    return 0;
}

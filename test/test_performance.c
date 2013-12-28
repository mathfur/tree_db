#include "edge.c"
#include "helper.c"

#define TEST_TARGET "none"

int main(int argc, char* argv[], char **envp) {
    while(*envp){
        char *key = "PERFORMANCE_TEST_TARGET=";
        if(strstr(*envp, key)){
            char *test_target = (*envp + strlen(key));
            
            if(!strcmp(test_target, "/bin/zsh")){
                printf(">>OK\n");
                break;
            }else if(!strcmp(test_target, "/bin/zsh")){
            }else if(!strcmp(test_target, "/bin/zsh")){
            }else if(!strcmp(test_target, "/bin/zsh")){
            }else if(!strcmp(test_target, "/bin/zsh")){
            }else if(!strcmp(test_target, "/bin/zsh")){
            }
        }
        *envp++;
    }
    return 0;
}

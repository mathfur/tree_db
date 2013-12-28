#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

Tree* create_simple_node(char* s){
    Tree* t = (Tree*) malloc(sizeof(Tree));

    t->name = s;
    t->tree_link = NULL;

    return t;
}

void add_tree_to_node(Tree* t, Tree* child){
    TreeLink* next_link = (TreeLink*) malloc(sizeof(TreeLink));
    TreeLink* curlink = t->tree_link;

    next_link->tree = child;
    next_link->next = NULL;

    if(curlink){
        while(curlink->next){
            curlink = curlink->next;
        }

        curlink->next = next_link;
    }else{
        t->tree_link = next_link;
    }
}

void show_indent(int indent_size){
    int i = 0;
    while(i < indent_size){
        printf(" ");
        i++;
    }
}

void display_tree0(Tree* t, int indent_size){
    show_indent(indent_size);
    printf("{\n");

    if(t->name){
        show_indent(indent_size + INDENT_STEP);
        printf("\"name\": \"%s\",\n", t->name);
    }

    show_indent(indent_size + INDENT_STEP);
    printf("\"children\": [\n");

    TreeLink* curlink = t->tree_link;
    while(curlink){
        display_tree0(curlink->tree, indent_size + INDENT_STEP*2);
        curlink = curlink->next;
    }

    show_indent(indent_size + INDENT_STEP);
    printf("]\n");

    show_indent(indent_size);
    printf("}\n");
}

void display_tree(Tree* t){
    printf("{\n");
    display_tree0(t, INDENT_STEP);
    printf("}\n");
}

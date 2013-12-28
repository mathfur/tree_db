#ifndef __TREE_H_INCLUDED_
#define __TREE_H_INCLUDED_

typedef struct _Tree {
    char* name;
    struct _TreeLink* tree_link;
} Tree;

typedef struct _TreeLink {
    struct _Tree* tree;
    struct _TreeLink* next;
} TreeLink;

Tree* create_simple_node(char *s);
void add_tree_to_node(Tree* t, Tree* child);
void display_tree(Tree* t);

#define INDENT_STEP 4

#endif

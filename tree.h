#ifndef TREE_H
#define TREE_H

#include "error.h"

typedef struct Node Node;

typedef struct Tree {
    Node *root;
} Tree;

Err tree_from_array(Node **root, int *array, int length);

void tree_insert(Node **root, Node *node);

void tree_free(Node **root);

Err tree_delete_id(Node **root, Node *parent, int id);

#endif
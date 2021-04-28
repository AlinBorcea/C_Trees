#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tree.h"

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

/// Private functions
Node *new_node(int key) {
    Node *node = (Node*) malloc(sizeof(Node));
    
    if (node != NULL) {    
        node->key = key;
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

Node *successor(Node *root) {
    Node *it;

    assert(root);

    it = root->right;
    if (!it) return NULL;

    while (it->left) {
        it = it->left;
    }

    return it;
}

void node_copy(Node *dst, Node *src) {
    assert(dst && src);
    dst->key = src->key;
}

int children_count(Node *node) {
    if (node == NULL) return -1;
    if (!node->left && !node->right) return 0;
    if (node->left && node->right) return 2;
    return 1;
}

/// Public functions.
Err tree_from_array(Node **root, int *array, int length) {
    Err e = { 0, "" };
    Node *node = NULL;
    int i;

    for (i = 0; i < length && e.code == 0; i++) {
        node = new_node(array[i]);

        if (node == NULL) {
            e.code = 0;
            strcpy(e.message, "error allocating node in tree_from_array");
            break;
        }

        tree_insert(root, node);
    }

    return e;
}

void tree_insert(Node **root, Node *node) {
    if (*root == NULL) {
        *root = node;
        return;
    }

    tree_insert(node->key < (*root)->key ?
    &(*root)->left : &(*root)->right, node);
}

void tree_free(Node **root) {
    if (*root == NULL) return;
    if ((*root)->left) tree_free(&(*root)->left);
    if ((*root)->right) tree_free(&(*root)->right);
    free(*root);
    *root = NULL;
}

Err tree_delete_id(Node **root, Node *parent, int id) {
    Err e = { 0, "" };
    Node *tmp = NULL;
    Node *tmp1 = NULL;
    int kids;

    if (*root == NULL) {
        e.code = 1;
        strcpy(e.message, "tree_delete_id: deleting a null node");
        return e;
    }

    if (id < (*root)->key) {
        if (!(*root)->left) {
            e.code = 2;
            strcpy(e.message, "given id is not in the tree");
            return e;
        }

        return tree_delete_id(&(*root)->left, *root, id);

    } else if (id > (*root)->key) {
        if (!(*root)->right) {
            e.code = 3;
            strcpy(e.message, "given id is not in the tree");
            return e;
        }

        return tree_delete_id(&(*root)->right, *root, id);

    } else {
        printf("attempting to delete id %d\n", (*root)->key);
        kids = children_count(*root);

        if (kids == -1) {
            e.code = 4;
            strcpy(e.message, "attempting to delete null node");
            return e;

        } else if (kids == 0) {
            printf("deleting leaf\n");
            if (parent->left == *root) {
                tmp = parent->left;
                parent->left = NULL;
            
            } else if (parent->right == *root) {
                tmp = parent->right;
                parent->right = NULL;
            }

        } else if (kids == 1) {
            printf("deleting only child\n");
            tmp = parent->left == *root ? parent->left : parent->right;
            tmp1 = tmp->left ? tmp->left : tmp->right;

            if (parent->left == *root) {
                parent->left = tmp1;
            
            } else if (parent->right == *root) {
                parent->right = tmp1;
            }

        } else if (kids == 2) {
            tmp1 = successor(*root);
            printf("Successor of target %d is %d\n", (*root)->key, tmp1->key);
            node_copy(*root, tmp1);
            return tree_delete_id(&(*root)->right, *root, tmp1->key);
        }

        if (tmp != NULL) {
            tmp->left = NULL;
            tmp->right = NULL;
            free(tmp);
        }
    }

    return e;
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "tree.h"
#include "error.h"

Err create_num_array(int **array, int argc, char **argv);

int main(int argc, char **argv) {
    Tree tree = { NULL };
    Err e = { 0, "" };
    int *num_array = NULL;

    if (argc <= 1) {
        fprintf(stderr, "type numbers as cli arguments\n");
        return 1;
    }

    if ((e = create_num_array(&num_array, argc, argv)).code) {
        fprintf(stderr, "%s\n", e.message);
        return e.code;
    }

    printf("*******Usage test*******\n\n");

    printf("tree_from_array: ");
    if ((e = tree_from_array(&tree.root, num_array, argc - 1)).code) {
        printf("fail\n");
        fprintf(stderr, "%s", e.message);
        return e.code;
    } printf("ok\n");

    printf("tree_delete_id 3: ");
    if ((e = tree_delete_id(&tree.root, tree.root, 3)).code) {
        printf("fail\n");
        fprintf(stderr, "%s", e.message);
        return e.code;
    } printf("ok\n");

    printf("tree_delete_id 3 non-existant ");
    if ((e = tree_delete_id(&tree.root, tree.root, 3)).code) {
        printf("passed\n");
    }

    free(num_array);
    tree_free(&tree.root);

    return 0;
}

Err create_num_array(int **array, int argc, char **argv) {
    Err e = { 0, "" };
    int i;

    assert(array != NULL && argc >= 2 && *argv != NULL);

    *array = (int*) malloc(sizeof(int) * (argc - 1));
    if (*array == NULL) {
        e.code = 1;
        strcpy(e.message, "int array allocation error");
        return e;
    }

    for (i = 0; i < argc - 1; i++) {
        (*array)[i] = atoi(argv[i + 1]);
    }

    return e;
}

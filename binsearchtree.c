//
// Created by fitli on 01.04.22.
//

#include <stdlib.h>
#include <stdio.h>
#include "binsearchtree.h"

struct BST create_empty_bst(int (*cmp_func)(void*, void*, void*)) {
    struct BST new;
    new.cmp_func = cmp_func;
    new.root = NULL;
    return new;
}

int add_node(struct BST* tree, void *data, void *temp_state) {
    struct BSTNode *new_node = malloc(sizeof(struct BSTNode));
    if(new_node == NULL) {
        fprintf(stderr, "Allocation error\n");
        return 0;
    }
    new_node->data = data;
    // we will insert new node as a leaf
    new_node->right = NULL;
    new_node->left = NULL;
    if (tree->root == NULL) {
        tree->root = new_node;
        new_node->parent = NULL;
        return 1;
    }
    struct BSTNode *cur_node = tree->root;
    while (1) {
        if(tree->cmp_func(cur_node->data, new_node->data, temp_state) > 0) {
            if(cur_node->left == NULL) {
                cur_node->left = new_node;
                new_node->parent = cur_node;
                new_node->side = LEFT;
                return 1;
            }
            else {
                cur_node = cur_node->left;
            }
        }
        else {
            if(cur_node->right == NULL) {
                cur_node->right = new_node;
                new_node->parent = cur_node;
                new_node->side = RIGHT;
                return 1;
            }
            else {
                cur_node = cur_node->right;
            }
        }
    }
}

struct BSTNode *find_node(struct BST *tree, void *data, void *temp_state) {
    struct BSTNode *cur_node = tree->root;
    while(cur_node != NULL) {
        int cmp = tree->cmp_func(cur_node->data, data, temp_state);
        if (cmp == 0) {
            return cur_node;
        }
        else if (cmp > 0) {
            cur_node = cur_node->left;
        }
        else {
            cur_node = cur_node->right;
        }
    }
    return NULL;
}

void remove_node(struct BST *tree, struct BSTNode* node) {
    struct BSTNode *replace_with;
    int replace_node = 0;
    // leaf
    if(node->left == NULL && node->right == NULL) {
        replace_with = NULL;
        replace_node = 1;
    }
    // left subtree empty
    else if(node->left == NULL) {
        replace_with = node->right;
        replace_node = 1;
    }
    // right subtree empty
    else if(node->right == NULL) {
        replace_with = node->left;
        replace_node = 1;
    }

    if(replace_node) {
        if(replace_with != NULL) {
            replace_with->parent = node->parent;
            replace_with->side = node->side;
        }
        if(node->parent == NULL) {
            tree->root = replace_with;
        }
        else if (node->side == LEFT) {
            node->parent->left = replace_with;
        }
        else if (node->side == RIGHT) {
            node->parent->right = replace_with;
        }
        free(node);
    }
    else {
        struct BSTNode *next = node->right;
        while (next->left != NULL) {
            next = next->left;
        }
        node->data = next->data;
        remove_node(tree, next);
    }
}

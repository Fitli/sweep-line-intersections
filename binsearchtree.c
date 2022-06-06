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
    new.num_moves = 0;
    new.num_swaps = 0;
    new.num_comparisons = 0;
    return new;
}

struct BSTNode *add_node(struct BST* tree, void *data, void *temp_state) {
    struct BSTNode *new_node = malloc(sizeof(struct BSTNode));
    if(new_node == NULL) {
        fprintf(stderr, "Allocation error\n");
        return NULL;
    }
    new_node->data = data;
    // we will insert new node as a leaf
    new_node->right = NULL;
    new_node->left = NULL;
    if (tree->root == NULL) {
        tree->root = new_node;
        new_node->parent = NULL;
        return new_node;
    }
    struct BSTNode *cur_node = tree->root;
    while (1) {
        tree->num_comparisons++;
        if(tree->cmp_func(cur_node->data, new_node->data, temp_state) > 0) {
            if(cur_node->left == NULL) {
                cur_node->left = new_node;
                new_node->parent = cur_node;
                new_node->side = LEFT;
                return new_node;
            }
            else {
                tree->num_moves++;
                cur_node = cur_node->left;
            }
        }
        else {
            if(cur_node->right == NULL) {
                cur_node->right = new_node;
                new_node->parent = cur_node;
                new_node->side = RIGHT;
                return new_node;
            }
            else {
                tree->num_moves++;
                cur_node = cur_node->right;
            }
        }
    }
}

struct BSTNode *find_node(struct BST *tree, void *data, void *temp_state) {
    struct BSTNode *cur_node = tree->root;
    while(cur_node != NULL) {
        tree->num_comparisons++;
        int cmp = tree->cmp_func(cur_node->data, data, temp_state);
        if (cmp == 0) {
            return cur_node;
        }
        else if (cmp > 0) {
            tree->num_moves++;
            cur_node = cur_node->left;
        }
        else {
            tree->num_moves++;
            cur_node = cur_node->right;
        }
    }
    return NULL;
}

struct BSTNode *find_prev(struct BSTNode *node, struct BST *tree) {
    if(node->left != NULL) {
        tree->num_moves++;
        node = node->left;
        while (node->right != NULL) {
            tree->num_moves++;
            node = node->right;
        }
        return node;
    } else {
        while (node->parent != NULL && node->side == LEFT) {
            tree->num_moves++;
            node = node->parent;
        }
        if(node->parent == NULL) {
            return NULL;
        }
        return node->parent;
    }
}

struct BSTNode *find_next(struct BSTNode *node, struct BST *tree) {
    if(node->right != NULL) {
        tree->num_moves++;
        node = node->right;
        while (node->left != NULL) {
            tree->num_moves++;
            node = node->left;
        }
        return node;
    } else {
        while (node->parent != NULL && node->side == RIGHT) {
            tree->num_moves++;
            node = node->parent;
        }
        if(node->parent == NULL) {
            return NULL;
        }
        return node->parent;
    }
}

void swap_parent_child(struct BST *tree, struct BSTNode* parent, struct BSTNode* child) {
    if(parent->parent == NULL) {
        tree->root = child;
    } else {
        parent->side == LEFT ? (parent->parent->left = child) : (parent->parent->right = child);
    }
    if(child -> left != NULL) {
        child->left->parent = parent;
    }
    if(child -> right != NULL) {
        child->right->parent = parent;
    }

    struct BSTNode old_parent = *parent;
    struct BSTNode old_child = *child;
    if (old_child.side == LEFT) {
        child->left = parent;
        child->right = old_parent.right;
        if(child->right != NULL) {
            child->right->parent = child;
        }
    }
    else {
        child->right = parent;
        child->left = old_parent.left;
        if(child->left != NULL) {
            child->left->parent = child;
        }
    }
    child->parent = old_parent.parent;
    child->side = old_parent.side;

    parent->parent = child;
    parent->side = old_child.side;
    parent->left = old_child.left;
    parent->right = old_child.right;
}

void swap_nodes(struct BST *tree, struct BSTNode* a, struct BSTNode* b) {
    tree->num_swaps++;
    if (a->parent == b){
        swap_parent_child(tree, b, a);
        return;
    }
    if (b->parent == a){
        swap_parent_child(tree, a, b);
        return;
    }
    if (a->parent != NULL)
        a->side == LEFT ? (a->parent->left = b) : (a->parent->right = b);
    else
        tree->root = b;
    if (a->left != NULL)
        a->left->parent = b;
    if (a->right != NULL)
        a->right->parent = b;

    if (b->parent != NULL)
        b->side == LEFT ? (b->parent->left = a) : (b->parent->right = a);
    else
        tree->root = a;
    if (b->left != NULL)
        b->left->parent = a;
    if (b->right != NULL)
        b->right->parent = a;

    struct BSTNode helper = *a;
    a->parent = b->parent;
    a->side = b->side;
    a->left = b->left;
    a->right = b->right;

    b->parent = helper.parent;
    b->side = helper.side;
    b->left = helper.left;
    b->right = helper.right;
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
        if(node->parent == NULL) {
            tree->root = replace_with;
        }
        else if (node->side == LEFT) {
            node->parent->left = replace_with;
        }
        else if (node->side == RIGHT) {
            node->parent->right = replace_with;
        }
        if(replace_with != NULL) {
            replace_with->parent = node->parent;
            replace_with->side = node->side;
        }
        free(node);
    }
    else {
        tree->num_moves++;
        struct BSTNode *next = node->right;
        while (next->left != NULL) {
            tree->num_moves++;
            next = next->left;
        }
        swap_nodes(tree, node, next);
        remove_node(tree, node);
    }
}


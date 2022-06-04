//
// Created by fitli on 01.04.22.
//

#include <stdlib.h>
#include <stdio.h>
#include "rbtree.h"

struct BST create_empty_bst(int (*cmp_func)(void*, void*, void*)) {
    struct BST new;
    new.cmp_func = cmp_func;
    new.root = NULL;
    return new;
}

void rotate_left(struct BST *tree, struct BSTNode *pivot) {
    struct BSTNode *p = pivot->parent;
    struct BSTNode *l = pivot->left;
    p->right = l;
    if (l != NULL) {
        l->parent = p;
        l->side = RIGHT;
    }
    pivot->parent = p->parent;
    if(pivot->parent == NULL) {
        tree->root = pivot;
    } else {
        if (p->side == LEFT) {
            pivot->parent->left = pivot;
        } else {
            pivot->parent->right = pivot;
        }
    }
    pivot->side = p->side;
    p->side = LEFT;
    p->parent = pivot;
    pivot->left = p;
}

void rotate_right(struct BST *tree, struct BSTNode *pivot) {
    struct BSTNode *p = pivot->parent;
    struct BSTNode *r = pivot->right;
    p->left = r;
    if (r != NULL) {
        r->parent = p;
        r->side = LEFT;
    }
    pivot->parent = p->parent;
    if(pivot->parent == NULL) {
        tree->root = pivot;
    } else {
        if (p->side == LEFT) {
            pivot->parent->left = pivot;
        } else {
            pivot->parent->right = pivot;
        }
    }
    pivot->side = p->side;
    p->side = RIGHT;
    p->parent = pivot;
    pivot->right = p;
}

void rebalance_insert(struct BST *tree, struct BSTNode *new_node) {
    struct BSTNode *parent = NULL, *grandpa = NULL, *uncle = NULL;
    parent = new_node->parent;
    if (parent != NULL){
        grandpa = parent->parent;
    }
    if (grandpa != NULL)
        uncle = (parent->side == LEFT) ? grandpa->right : grandpa->left;

    if(parent == NULL) { //I3
        return;
    }
    if (parent->color == BLACK) { //I1
        return;
    }
    if (grandpa == NULL) { //I4 - parent is red root
        parent->color = BLACK;
        return;
    }
    if (grandpa->color == BLACK && parent->color == RED && uncle && uncle->color == RED) { //I2
        new_node->color = RED;
        parent->color = BLACK;
        uncle->color = BLACK;
        rebalance_insert(tree, grandpa);
        return;
    }
    if (new_node->side != parent->side) {//I5 - parent red, uncle black, inner grandchild
        if(new_node->side == RIGHT) {
            rotate_left(tree, new_node);
        } else {
            rotate_right(tree, new_node);
        }
        rebalance_insert(tree, parent);
        return;
    }
    // I6 - outer grandchild
    if(parent->side == LEFT) {
        rotate_right(tree, parent);
    } else {
        rotate_left(tree, parent);
    }
    parent->color = BLACK;
    grandpa->color = RED;

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
        new_node->color = BLACK;
        return new_node;
    }
    struct BSTNode *cur_node = tree->root;
    while (1) {
        if(tree->cmp_func(cur_node->data, new_node->data, temp_state) > 0) {
            if(cur_node->left == NULL) {
                cur_node->left = new_node;
                new_node->parent = cur_node;
                new_node->side = LEFT;
                new_node->color = RED;
                rebalance_insert(tree, new_node);
                return new_node;
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
                new_node->color = RED;
                rebalance_insert(tree, new_node);
                return new_node;
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

struct BSTNode *find_prev(struct BSTNode *node) {
    if(node->left != NULL) {
        node = node->left;
        while (node->right != NULL) {
            node = node->right;
        }
        return node;
    } else {
        while (node->parent != NULL && node->side == LEFT) {
            node = node->parent;
        }
        if(node->parent == NULL) {
            return NULL;
        }
        return node->parent;
    }
}

struct BSTNode *find_next(struct BSTNode *node) {
    if(node->right != NULL) {
        node = node->right;
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    } else {
        while (node->parent != NULL && node->side == RIGHT) {
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
    child->color = old_parent.color;

    parent->parent = child;
    parent->side = old_child.side;
    parent->color = old_child.color;
    parent->left = old_child.left;
    parent->right = old_child.right;
}

void swap_nodes(struct BST *tree, struct BSTNode* a, struct BSTNode* b) {
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
    a->color = b->color;

    b->parent = helper.parent;
    b->side = helper.side;
    b->left = helper.left;
    b->right = helper.right;
    b->color = helper.color;
}


void rebalance_delete(struct BST *tree, struct BSTNode* node) {
    struct BSTNode* parent = node->parent;
    if(parent == NULL) { //R2 - root, done
        return;
    }
    struct BSTNode* sibling = (node->side == LEFT) ? parent->right : parent->left;
    if(parent->color == BLACK && sibling->color == BLACK &&
            (sibling->left == NULL || sibling->left->color == BLACK) &&
            (sibling->right == NULL || sibling->right->color == BLACK)) { //R1
        sibling->color = RED;
        rebalance_delete(tree, parent);
        return;
    }
    if(sibling->color == RED) { //R3
        rotate_left(tree, sibling);
        sibling->color = BLACK;
        parent->color = RED;
        rebalance_delete(tree, node);
        return;
    }
    if(parent->color == RED && sibling->color == BLACK &&
            (sibling->left == NULL || sibling->left->color == BLACK) &&
            (sibling->right == NULL || sibling->right->color == BLACK)) { //R4
        parent->color = BLACK;
        sibling->color = RED;
        return;
    }
    //R5
    if(node->side == LEFT && sibling &&
       sibling->left && sibling->left->color == RED &&
       (!sibling->right || sibling->right->color == BLACK)) {
        struct BSTNode* niece = sibling->left;
        rotate_right(tree, niece);
        niece->color = BLACK;
        sibling->color = RED;
        rebalance_delete(tree, node);
        return;
    }
    if(node->side == RIGHT && sibling &&
       sibling->right && sibling->right->color == RED &&
       (!sibling->left || sibling->left->color == BLACK)) {
        struct BSTNode* niece = sibling->right;
        rotate_left(tree, niece);
        niece->color = BLACK;
        sibling->color = RED;
        rebalance_delete(tree, node);
        return;
    }
    // R6
    if(node->side == LEFT) {
        rotate_left(tree, sibling);
        sibling->color = parent->color;
        parent->color = BLACK;
        sibling->right->color = BLACK;
        return;
    }
    else {
        rotate_right(tree, sibling);
        sibling->color = parent->color;
        parent->color = BLACK;
        sibling->left->color = BLACK;
        return;
    }
}

void remove_node(struct BST *tree, struct BSTNode* node) {
    struct BSTNode* parent = node->parent;

    struct BSTNode *replace_with;
    int replace_node = 0;

    // leaf
    if(node->left == NULL && node->right == NULL && (node->color == RED || parent == NULL)) {
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
            replace_with->color = BLACK;
        }
        else if (node->color == BLACK){
            rebalance_delete(tree, node);
        }
        free(node);
    }
    else {
        struct BSTNode *next = node->right;
        while (next->left != NULL) {
            next = next->left;
        }
        swap_nodes(tree, node, next);
        remove_node(tree, node);
    }
}


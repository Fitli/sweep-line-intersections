//
// Created by fitli on 01.04.22.
//

#ifndef SWEEP_LINE_INTERSECTIONS_BINSEARCHTREE_H
#define SWEEP_LINE_INTERSECTIONS_BINSEARCHTREE_H

enum Side {
    LEFT,
    RIGHT
};

struct BSTNode {
    void *data;
    struct BSTNode* parent;
    enum Side side;
    struct BSTNode* left;
    struct BSTNode* right;
};

struct BST {
    struct BSTNode* root;
    int (*cmp_func)(void*, void*, void*); // returns positive number iff first is bigger then second value
};

struct BST create_empty_bst(int (*cmp_func)(void*, void*, void*));
int add_node(struct BST* tree, void *data, void *temp_state);
struct BSTNode *find_node(struct BST *tree, void *data, void *temp_state);
void remove_node(struct BST *tree, struct BSTNode* node);


#endif //SWEEP_LINE_INTERSECTIONS_BINSEARCHTREE_H

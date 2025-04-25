#include "bst.h"
#include <stdio.h>

// Uses helper method builder() to build a binary search tree using an array of int elements.
void build_tree(TreeNode** root, const int elements[], const int count) {
    for (int i = 0; i < count; i++) {
        builder(root, elements[i]);
    }
}

// Helper method for build_tree() to recursively allocate space for a TreeNode and place the data inputted into the node, while initializing left and right nodes.
void builder(TreeNode** root, int data) {
    if (root == NULL) {
        *root = (TreeNode*) malloc(sizeof(TreeNode));
        (*root) -> data = data;
        (*root) -> left = NULL;
        (*root) -> right = NULL;
    }
    else if (data <= (*root) -> data) {
        builder(&(*root) -> left, data);
    }
    else {
        builder(&(*root) -> right, data);
    }
}

// Performs a traversal of the binary search tree based on the specified traversal type.
void traverse(const TreeNode* root, const TraversalType type) {
    if (root == NULL) {
        return;
    }
    switch (type) {
        case PREORDER:
            printf("%d\n", root -> data);
            traverse(root -> left, PREORDER);
            traverse(root -> right, PREORDER);
            break;
        case POSTORDER:
            traverse(root -> left, POSTORDER);
            traverse(root -> right, POSTORDER);
            printf("%d\n", root -> data);
            break; 
        case INORDER:
            traverse(root -> left, INORDER);
            printf("%d\n", root -> data);
            traverse(root -> right, INORDER);
            break; 
    }
}

// Cleans up the binary search tree by freeing memory for each node.
void cleanup_tree(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    cleanup_tree(root -> left);
    cleanup_tree(root -> right);

    free(root);
}

int main(int argc, char *argv[]) {
    TreeNode **root = malloc(sizeof(TreeNode));
    int count = (int) &argv[1];
    printf("Please enter %d values to place in tree:\n", count);
    int * elements = malloc(sizeof(int) * count);
    for (int i = 0; i <= count; i++) {
        scanf("%d", elements[i]);
    }
    build_tree(root, elements, count);
    printf("Input values:\n");
    for (int i = 0; i <= count; i++) {
        printf("%d\n", elements[i]);
    }
    printf("Preorder:\n");
    traverse(root, PREORDER);
    printf("Inorder:\n");
    traverse(root, INORDER);
    printf("Postorder:\n");
    traverse(root, POSTORDER);
    printf("\n"); 
}

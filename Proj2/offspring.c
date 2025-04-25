/// 
/// File: offspring.c
/// Compilation: gcc -Wall -ggdb -std=c99 offspring.c trimit.c -o offspring
/// Usage: ./offspring [tree_file] <- optional!
///
/// Program that optionally reads a file and creates a tree of the data.
/// The program then allows the user to interact with the tree by entering
/// commands. The commands can be found by typing 'help' into the program.
/// 
/// The program uses a queue and breadth first search to find nodes and print.
/// 
/// @author jht3250 : John Treon
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trimit.h"

#define MAX_USER_INPUT (1024) // max length of input

// struct for the tree
typedef struct NTree_S {
    char * name; 
    struct NTree_S ** children;
    size_t child_count;
} NTree;

// queue implementation
typedef struct Queue {
    NTree **data;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

// createQueue() that allocates memory for a queue and initializes it
// args: capacity - capacity of the queue
// returns: pointer to the queue
Queue* createQueue(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = 0;
    queue->size = 0;
    queue->rear = capacity - 1;  // This is important for the enqueue function
    queue->data = (NTree**)malloc(queue->capacity * sizeof(NTree*));
    return queue;
}

int isFull(Queue* queue) {
    return (queue->size == queue->capacity);
}

int isEmpty(Queue* queue) {
    return (queue->size == 0);
}

// Function to add an item to the queue.
void enqueue(Queue* queue, NTree* item) {
    if (isFull(queue)) return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->data[queue->rear] = item;
    queue->size++;
}

// Function to remove an item from queue.
NTree* dequeue(Queue* queue) {
    if (isEmpty(queue)) return NULL;
    NTree* item = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return item;
}

// create_node() that allocates memory for a node and initializes it
NTree * create_node(char * name) {
    NTree * node = malloc(sizeof(NTree));
    node -> name = malloc(strlen(name) + 1);
    strcpy(node -> name, name);
    node -> children = NULL;
    node -> child_count = 0;
    return node;
}

// destroy_tree() that uses recursion to free all allocated nodes
// args: tree - pointer to the tree
// returns: void
void destroy_tree(NTree ** tree) {
    if (tree == NULL || *tree == NULL) { // <-- Check *tree here as well
        return;
    }
    for (size_t i = 0; i < (*tree) -> child_count; i++) {
        destroy_tree(&((*tree) -> children[i]));
    }
    free((*tree) -> children);
    free((*tree) -> name);
    free(*tree);
    *tree = NULL;
}


// find_node() that uses a queue to find a node
// args: tree - pointer to the tree, name - name of the node to find
// returns: pointer to the node if found, NULL otherwise
NTree * find_node(NTree ** tree, char * name) {
    if (tree == NULL) {
        return NULL;
    }
    if (name == NULL) {
        name = "";
    }
    Queue *queue = createQueue(1000);  
    enqueue(queue, *tree);

    while (!isEmpty(queue)) {
        NTree *node = dequeue(queue);
        if (strcmp(node->name, name) == 0) {
            free(queue->data);
            free(queue);
            return node;
        }
        for (size_t i = 0; i < node->child_count; i++) {
            enqueue(queue, node->children[i]);
        }
    }
    // clear memory leaks
    free(queue->data);
    free(queue);
    return NULL;
}

// method find() that uses find_node() to find a node and print it's children if it exists
// args: tree - pointer to the tree, name - name of the node to find
// returns: void
void find(NTree ** tree, char * name) {
    if (name == NULL) {
        name = "";
    }
    NTree * node = find_node(tree, name);
    if (node == NULL || strcmp(name, "") == 0) {
        fprintf(stderr, "error: '%s' not found.\n", name);
        return;
    }
    if (node -> child_count == 0) {
        printf("%s had no offspring.\n", node -> name);
        return;
    }
    printf("%s had ", node -> name);
    if (node -> child_count == 1) {
        printf("%s.\n", node -> children[0] -> name);
        return;
    }
    else {
        for (size_t i = 0; i < node -> child_count; i++) {
            if (i < node -> child_count - 2) {
                printf("%s, ", node->children[i]->name);
            }
            else if (i == node->child_count - 1) {
                printf("and %s.\n", node->children[i]->name);
            } 
            else {
                printf("%s ", node->children[i]->name);
            }
        }
    }
}

// print_tree() that uses breadth first search to print the tree
// args: tree - pointer to the tree, name - name of the node to start printing from
// returns: void
void print_tree(NTree ** tree, char * name) {
    if (*tree == NULL || strcmp((*tree) -> name, "") == 0) {
        fprintf(stderr, "error: Tree is empty.\n");
        return;
    }
    NTree * startNode;
    if (name == NULL) {
        startNode = *tree;
    }
    else {
        startNode = find_node(tree, name);
        if (startNode == NULL) {
            fprintf(stderr, "error: '%s' not found.\n", name);
            return;
        }
    }

    Queue *queue = createQueue(1000); 
    enqueue(queue, startNode);

    while (!isEmpty(queue)) {
        NTree *node = dequeue(queue);

        if (node->child_count == 0) {
            printf("%s had no offspring.\n", node->name);
        } else {
            printf("%s had ", node->name);
            if (node->child_count == 1) {
                printf("%s.\n", node->children[0]->name);
                enqueue(queue, node->children[0]);
            }
            else {
                for (size_t i = 0; i < node->child_count; i++) {
                    if (i < node -> child_count - 2) {
                        printf("%s, ", node->children[i]->name);
                    }
                    else if (i == node->child_count - 1) {
                        printf("and %s.\n", node->children[i]->name);
                    } 
                    else {
                        printf("%s ", node->children[i]->name);
                    }
                    enqueue(queue, node->children[i]);
                }
            }
        }
    }
    free(queue->data);
    free(queue);
}




// Adds a node to the tree, whether that be the root as a parent or to a parent as a child.
// args: tree - pointer to the tree, parent_name - name of the parent node, child_name - name of the child node
// returns: void
void add_child(NTree** tree, char* parent_name, char* child_name) {
    if (*tree == NULL) { // if the tree is empty
        if (parent_name == NULL || child_name == NULL) {
            fprintf(stderr, "Error: Invalid names provided.\n");
            return;
        }
        
        *tree = create_node(parent_name); // add the parent as the root
        (*tree)->children = malloc(sizeof(NTree *)); // allocate memory for the children
        (*tree)->children[0] = create_node(child_name); // add the child to the root
        (*tree)->child_count++; 
        return;
    }
    else if (strcmp((*tree)->name, "") == 0) { // if the tree is empty
        if (parent_name == NULL || child_name == NULL) {
            fprintf(stderr, "error: Invalid names provided.\n");
            return;
        }
        *tree = create_node(parent_name); 
        (*tree)->children = malloc(sizeof(NTree *));
        (*tree)->children[0] = create_node(child_name);
        (*tree)->child_count++;
        return;
    }
    if (parent_name == NULL) { // if parent name is not given
        fprintf(stderr, "error: parent name not given.\n");
        return;
    }
    if (child_name == NULL) { // if child name is not given
        fprintf(stderr, "error: child name not given.\n");
        return;
    }
    
    NTree* parent = find_node(tree, parent_name); // find the parent node
    if (parent == NULL) { // if the parent is not found
        if (strcmp((*tree)->name, child_name) == 0) { // if the child is the root
            NTree* new_root = create_node(parent_name); // create the new root
            new_root->children = malloc(sizeof(NTree *));
            new_root->children[0] = *tree; // set the old root as a child of the new root
            new_root->child_count++;
            *tree = new_root; // set the new root as the root of the tree
            return;
        }
        else {
            fprintf(stderr, "error: '%s' is not in the tree, and '%s' is not the root.\n", parent_name, child_name);
            return;
        }
    }
    
    for (size_t i = 0; i < parent->child_count; i++) { // check if the child is already a child of the parent
        if (strcmp(parent->children[i]->name, child_name) == 0) {
            fprintf(stderr, "error: child already exists.\n");
            return;
        }
    }
    
    parent->children = realloc(parent->children, sizeof(NTree *) * (parent->child_count + 1)); // add the child to the parent
    parent->children[parent->child_count] = create_node(child_name);
    parent->child_count++;
}

// count the number of offspring of a node
// args: tree - pointer to the tree, name - name of the node
// returns: int - number of offspring
int count_offspring(NTree ** tree, char * name) {
    if (tree == NULL || *tree == NULL) {
        return 0;
    }
    NTree * node = (name == NULL) ? *tree : find_node(tree, name); // thought this was cool
    if (node == NULL) {
        return 0;
    }
    int count = 1;
    for (size_t i = 0; i < node->child_count; i++) {
        count += count_offspring(&(node->children[i]), NULL);  // Recursive call
    }
    return count;
}


// calculate the height of the tree
// args: tree - pointer to the tree, name - name of the node
// returns: int - height of the tree
int height(NTree ** tree, char * name) {
    if (tree == NULL || *tree == NULL) {
        return -1;
    }
    NTree * node = (name == NULL) ? *tree : find_node(tree, name); 
    if (node == NULL) {
        return -1;
    }
    int max_height = -1;
    for (size_t i = 0; i < node->child_count; i++) {
        int child_height = height(&(node->children[i]), NULL);  // Recursive call
        if (child_height > max_height) {
            max_height = child_height;
        }
    }
    return max_height + 1;
}


// method to initialize the tree
void init(NTree ** tree) {
    destroy_tree(tree);
    *tree = create_node("");
}

// method to print the help message
void help() {
    printf("add parent-name, child-name # find parent and add child.\nfind name     # search and print name and children if name is found.\nprint [name]  # breadth first traversal of offspring from name.\nsize [name]   # count members in the [sub]tree.\nheight [name] # return the height of [sub]tree.\ninit          # delete current tree and restart with an empty tree.\nhelp          # print this information.\nquit          # delete current tree and end program. \n");
}

// method to parse the command
// args: command - the command to be parsed, tree - pointer to the tree
// returns: void
void parse_command(char * command, NTree ** tree) {
    command = trim(command);
    char * command_name = strtok(command, " \n");
    if (command_name == NULL) {
        return;
    }
    if (strcmp(command_name, "add") == 0) {
        char * parent_name = strtok(NULL, ",\n"); // strtok using comma and newline
        char * child_name = strtok(NULL, ",\n");  // strtok using comma and newline
        if (parent_name && child_name) {         // Ensure both names are present
            add_child(tree, trim(parent_name), trim(child_name));  // Using trim to clean any extra spaces
        }
        else {
            printf("Usage: 'add parent name , child name'\n");
        }
    }

    else if (strcmp(command_name, "find") == 0) {
        char * name = strtok(NULL, "\n");
        find(tree, trim(name));
    } 
    else if (strcmp(command_name, "print") == 0) {
        char * name = strtok(NULL, "\n");
        print_tree(tree, trim(name));
    } 
    else if (strcmp(command_name, "size") == 0) {
        char * name = strtok(NULL, "\n");
        printf("size: %d\n", count_offspring(tree, trim(name)));
    } 
    else if (strcmp(command_name, "height") == 0) {
        char * name = strtok(NULL, "\n");
        printf("height: %d\n", height(tree, trim(name)));
    } 
    else if (strcmp(command_name, "init") == 0) {
        init(tree);
    } 
    else if (strcmp(command_name, "help") == 0) {
        help();
    } 
    else if (strcmp(command_name, "quit") == 0) {
        destroy_tree(tree);
        exit(0);
    } 
    else {
        printf("Invalid command\n");
    }
}

// method to read in a file
// args: tree - pointer to the tree, filename - name of the file to be read
// returns: void
NTree * addFromFile(NTree ** tree, char * line) {
    line = trim(line);
    char * parent_name = strtok(line, ",\n");
    if (!parent_name) {
        return *tree;
    }
    parent_name = trim(parent_name);
    char * child_token = strtok(NULL, ",\n");
    if (child_token == NULL) {
        if (!(*tree)) {
            *tree = create_node(parent_name);
        } 
        else {
            (*tree)->name = malloc(strlen(parent_name) + 1);  // allocate memory for new name
            strcpy((*tree)->name, parent_name);  // copy new name
        }
    }
    else {
        while (child_token != NULL) {
            add_child(tree, trim(parent_name), trim(child_token));
            child_token = strtok(NULL, ",\n");
        }
    }
    return *tree;
}


// read in file using addFromFile()
// args: fp - pointer to the file
// returns: void
NTree * read_file(FILE * fp) {
    NTree * tree = NULL;
    char line[MAX_USER_INPUT];
    while (fgets(line, MAX_USER_INPUT, fp) != NULL) {
        tree = addFromFile(&tree, line);
    }
    fflush(fp);
    return tree;
}

// main method
// args: argc - number of command line arguments, argv - the arguments themselves
int main(int argc, char ** argv) {
    NTree * tree = NULL;
    if (argc == 2) { // if there is a file
        FILE * fp = fopen(argv[1], "r");
        if (fp == NULL) {
            fprintf(stderr, "error: could not open file '%s'\n", argv[1]);
        }
        else {
            tree = read_file(fp);
            fclose(fp);
        }
    }
    else { // if there is no file
        init(&tree); // initialize the tree
    }
    while (1) { // loop until user quits
        char command[MAX_USER_INPUT];
        printf("offspring> ");
        fgets(command, MAX_USER_INPUT, stdin);
        if (command[0] == '\n') { // if user just presses enter
            // do nothing
        }
        else {
            parse_command(command, &tree);
        }
    }
    return 0;
}
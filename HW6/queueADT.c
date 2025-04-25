/// 
/// File: queueADT.c
/// a program to make a dynamic queue with abstract data types
/// 
/// @author jht3250 : John Treon
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "queueADT.h"
#ifndef _QUEUE_IMPL_
#define _QUEUE_IMPL_
#endif

/// Create a QueueADT which uses the supplied function as a comparison
/// routine.
///
/// The comparison function takes two void * parameters, and returns
/// an integer result which indicates the relationship between the
/// two things:
///
///	Result	Relationship
///	======	===========
///	 < 0	a < b
///	 = 0	a == b
///	 > 0	a > b
///
/// where ">" and "<" are dependent upon the data being compared
///
/// @param cmp the address of the comparison function to be used for
///    ordering this queue, or NULL if standard FIFO behavior is desired
/// @return a QueueADT instance, or NULL if the allocation fails
QueueADT que_create( int (*cmp)(const void * a, const void * b) ) {
    QueueADT queue = malloc(sizeof(QueueADT)); // initial allocation for queue
    queue -> comp = cmp; 
    return queue;
}

/// Indicate whether or not the supplied Queue is empty
///
/// @param the QueueADT to be tested
/// @return true if the queue is empty, otherwise false
bool que_empty(QueueADT queue) {
    return (!queue -> root);
}

/// Remove all contents from the supplied QueueADT.
///
/// @param queue - the QueueADT to be manipuated
void que_clear(QueueADT queue) {
    if (que_empty(queue)) {
        return;
    }
    else {
        clearHelper(queue -> root); // helper method to free nodes
        queue -> root = NULL; // clear root node
    }    
}

/// Helper method to recursively free all elements from queue
/// @param root - the root node to traverse 
void clearHelper(QueueStruct *root) {
    if (!root -> next) { // if there is no child node
        free(root); // free node
        return;
    }
    else {
        clearHelper(root -> next); // recurse
    }
}

/// Tear down and deallocate the supplied QueueADT.
///
/// @param queue - the QueueADT to be manipulated
void que_destroy( QueueADT queue ) {
    que_clear(queue);
    free(queue);
}

/// Insert the specified data into the Queue in the appropriate place
///
/// Uses the queue's comparison function to determine the appropriate
/// place for the insertion.
///
/// @param queue the QueueADT into which the value is to be inserted
/// @param data the data to be inserted
/// @exception If the value cannot be inserted into the queue for
///     whatever reason (usually implementation-dependent), the program
///     should terminate with an error message.  This can be done by
///     printing an appropriate message to the standard error output and
///     then exiting with EXIT_FAILURE, or by having an assert() fail.
void que_insert(QueueADT queue, void * ins) {
    QueueStruct* node = malloc(sizeof(QueueStruct));
    node -> data = ins; // insert data into root
    node -> next = NULL; // initialize child node

    if (que_empty(queue)) { // if queue is empty
        queue -> root = node;
        return;
    }
    if (!queue -> comp) { // if queue has no comparison (FIFO)
        insertHelper(queue -> root, node);
        return;
    }

    if (queue -> comp(ins, queue -> root -> data) <= 0) { // if inserted data is greater than or equal to previous data 
        QueueStruct* prev = queue -> root; // store root 
        queue -> root = node; // store inserted node in root
        node -> next = prev;
        return;
    }

    QueueStruct * prev = queue -> root; // set to root
    QueueStruct * current = prev -> next; // set to following after root

    bool running = true;
    while (true) {
        if (!prev -> next) { // if no child node
            prev -> next = node;
            running = false;
            break;
        } 
        // putting in between 
        if (queue -> comp(ins, current -> data) <= 0) { // if inserted data is greater than or equal to current data 
            node -> next = current; // set inserted node child to current
            prev -> next = node; // set inserting node to after previous
            running = false;
            break;
        }
        prev = current;
        current = current -> next;
    }
    return;
}

void insertHelper(QueueStruct* current, QueueStruct* new) {
    if (!current -> next) { // if child node is empty
        current -> next = new; // add node
        return;
    }
    else {
        insertHelper(current -> next, new); // recurse 
    }
}

/// Remove and return the first element from the QueueADT.
///
/// @param queue the QueueADT to be manipulated
/// @return the value that was removed from the queue
/// @exception If the queue is empty, the program should terminate
///     with an error message.  This can be done by printing an
///     appropriate message to the standard error output and then
///     exiting with EXIT_FAILURE, or by having an assert() fail.
void * que_remove(QueueADT queue) {
    QueueStruct * node; 
    void * data;
    if (que_empty(queue)) {
        printf("%s", "Error: Queue is empty!");
        exit(EXIT_FAILURE);
    }
    node = queue -> root;
    data = node -> data;
    queue -> root = node -> next;
    free(node);
    return data;
}
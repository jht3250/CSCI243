/// 
/// File: quick_sort.c
/// Compilation: gcc -pthread -Wall -std=c99 -o quick_sort quick_sort.c
/// Usage: ./quick_sort <size>
///
/// A program implementing threaded and non-threaded quick sort using recursion. 
/// The program takes in an argument of size, and creates an array of that size, populated 
/// with random integers from 0 - size*10. It then sorts the array using both non-threaded 
/// and threaded quick sort, and prints the time it took for both to complete, along with 
/// the difference between the two.
/// 
/// @author jht3250 : John Treon
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

// generate a random array of numbers of size 'size' and return a pointer to the first element
int *create_random_nums(int size) {
    int *array = malloc(size * sizeof(int));
    int i;
    for (i = 0; i < size; i++) {
        array[i] = rand() % (size * 10);
    }
    return array;
}

// helper method to partition the array and return the index of the pivot
// args: arr - pointer to the first element of the array, size - the count of integers in the array (not the size!)
// returns: index of the pivot
int partition(int *arr, int size) {
    int pivot = arr[size / 2];
    int i, j;
    for (i = 0, j = size - 1; ; i++, j--) {
        while (arr[i] < pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }
        if (i >= j) {
            break;
        }
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    return i;
}
// recursively quick sort the array and return a pointer to the first element of the sorted array
// args: arr - pointer to the first element of the array, size - the count of integers in the array (not the size!)
// returns: pointer to the first element of the sorted array
int *quick_sort(int *arr, int size) {
    if (size <= 1) {
        return arr;
    }
    int i = partition(arr, size); // index of the pivot
    quick_sort(arr, i); // recurse and sort the left side
    quick_sort(arr + i, size - i); // recurse and sort the right side
    return arr;
}


// struct to pass arguments to the threaded quick sort function
struct threaded_quick_sort_args {
    int *arr;
    int size;
};

// same as quick_sort, but each recursive call is done in a new thread
void *quick_sort_threaded(void *args) { 
    struct threaded_quick_sort_args *t_args = (struct threaded_quick_sort_args *) args; 
    int *arr = t_args -> arr; 
    int size = t_args -> size; 
    if (size <= 1) { 
        return arr;
    }
    int i = partition(arr, size); // partition the array and get the index of the pivot
    struct threaded_quick_sort_args *left_args = malloc(sizeof(struct threaded_quick_sort_args)); 
    left_args -> arr = arr; 
    left_args -> size = i;
    struct threaded_quick_sort_args *right_args = malloc(sizeof(struct threaded_quick_sort_args));
    right_args -> arr = arr + i;
    right_args -> size = size - i;
    pthread_t left_thread;
    pthread_t right_thread;
    pthread_create(&left_thread, NULL, quick_sort_threaded, left_args);
    pthread_create(&right_thread, NULL, quick_sort_threaded, right_args);
    pthread_join(left_thread, NULL);
    pthread_join(right_thread, NULL);
    free(left_args);
    free(right_args);
    return arr;
}

// main method
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./quick_sort <size>\n");
        return 1;
    }
    int size = atoi(argv[1]); // get the size from the command line argument
    int *arr = create_random_nums(size);
    int *arr_copy = malloc(size * sizeof(int)); // copy the array so we can sort it with the non-threaded quick sort
    int i;
    for (i = 0; i < size; i++) {
        arr_copy[i] = arr[i];
    }
    clock_t begin = clock(); // start non-threaded time
    quick_sort(arr, size);
    clock_t end = clock(); // end non-threaded time
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC; 
    double noThreadTime = time_spent; // store the time it took for non-threaded quick sort
    printf("Time for non-threaded quick sort: %f\n", time_spent);
    begin = clock(); // start threaded time
    struct threaded_quick_sort_args *args = malloc(sizeof(struct threaded_quick_sort_args));
    args -> arr = arr_copy; // pass the copy of the array to the threaded quick sort
    args -> size = size; // pass the size of the array to the threaded quick sort
    quick_sort_threaded(args);
    end = clock(); // end threaded time
    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    double threadTime = time_spent;
    printf("Time for threaded quick sort: %f\n", time_spent);
    printf("Difference between threaded and non-threaded: %f\n", fabs(threadTime - noThreadTime));
    free(arr); // memory cleanup
    free(arr_copy);
    free(args);
    return 0;
}
#ifndef ANALYSIS_H
#define ANALYSIS_H

typedef struct {
    long comparisons;
    long swaps;
} Stats;

void copy_array(int *src, int *dest, int n);
void swap_values(int *a, int *b);
int* make_random(int n);
int* make_sorted(int n);
int* make_reverse(int n);


Stats bubble_sort(int *arr, int n);
Stats selection_sort(int *arr, int n);
Stats insertion_sort(int *arr, int n);
Stats merge_sort(int *arr, int n);
Stats quicksort_first(int *arr, int n);
Stats quicksort_random(int *arr, int n);

#endif

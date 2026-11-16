#include "analysis.h"
#include <stdlib.h>

void copy_array(int *src, int *dest, int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

void swap_values(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int* make_random(int n) {
    int *arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = rand() % 1000000;
    return arr;
}

int* make_sorted(int n) {
    int *arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = i + 1;
    return arr;
}

int* make_reverse(int n) {
    int *arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = n - i;
    return arr;
}

Stats bubble_sort(int *arr, int n) {
    Stats s = {0, 0};
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            s.comparisons++;
            if (arr[j] > arr[j + 1]) {
                swap_values(&arr[j], &arr[j + 1]);
                s.swaps++;
            }
        }
    }
    return s;
}

Stats selection_sort(int *arr, int n) {
    Stats s = {0, 0};
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            s.comparisons++;
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        if (min_idx != i) {
            swap_values(&arr[i], &arr[min_idx]);
            s.swaps++;
        }
    }
    return s;
}

Stats insertion_sort(int *arr, int n) {
    Stats s = {0, 0};
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            s.comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                s.swaps++;
                j--;
            } else break;
        }
        arr[j + 1] = key;
    }
    return s;
}

// Merge Sort
void merge_sort_helper(int *arr, int *temp, int left, int right, Stats *s) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort_helper(arr, temp, left, mid, s);
        merge_sort_helper(arr, temp, mid + 1, right, s);
        int i = left, j = mid + 1, k = left;
        while (i <= mid && j <= right) {
            s->comparisons++;
            if (arr[i] <= arr[j]) temp[k++] = arr[i++];
            else temp[k++] = arr[j++];
            s->swaps++;
        }
        while (i <= mid) { temp[k++] = arr[i++]; s->swaps++; }
        while (j <= right) { temp[k++] = arr[j++]; s->swaps++; }
        for (k = left; k <= right; k++) arr[k] = temp[k];
    }
}

Stats merge_sort(int *arr, int n) {
    Stats s = {0, 0};
    int *temp = (int*)malloc(n * sizeof(int));
    merge_sort_helper(arr, temp, 0, n - 1, &s);
    free(temp);
    return s;
}

// Quicksort 
int partition_first(int *arr, int low, int high, Stats *s) {
    int pivot = arr[low];
    int yellow = low + 1;
    int green = low + 1;
    while (green <= high) {
        s->comparisons++;
        if (arr[green] < pivot) {
            swap_values(&arr[yellow], &arr[green]);
            s->swaps++;
            yellow++;
        }
        green++;
    }
    swap_values(&arr[low], &arr[yellow - 1]);
    s->swaps++;
    return yellow - 1;
}

void quick_helper(int *arr, int low, int high, Stats *s, int use_random) {
    if (low < high) {
        if (use_random) {
            int rand_idx = low + rand() % (high - low + 1);
            swap_values(&arr[low], &arr[rand_idx]);
            s->swaps++;
        }
        int p = partition_first(arr, low, high, s);
        quick_helper(arr, low, p - 1, s, use_random);
        quick_helper(arr, p + 1, high, s, use_random);
    }
}

Stats quicksort_first(int *arr, int n) {
    Stats s = {0, 0};
    quick_helper(arr, 0, n - 1, &s, 0);
    return s;
}

Stats quicksort_random(int *arr, int n) {
    Stats s = {0, 0};
    quick_helper(arr, 0, n - 1, &s, 1);
    return s;
}

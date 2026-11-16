#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "analysis.h"

typedef Stats (*SortFunc)(int*, int);

double time_one(SortFunc func, int *base, int n, int repeats) {
    double total = 0;
    for (int r = 0; r < repeats; r++) {
        int *arr = (int*)malloc(n * sizeof(int));
        copy_array(base, arr, n);
        clock_t start = clock();
        func(arr, n);
        clock_t end = clock();
        total += ((double)(end - start)) / CLOCKS_PER_SEC;
        free(arr);
    }
    return total / repeats;
}

int main() {
    srand((unsigned int)time(NULL));

    const char *names[6] = {"Bubble", "Selection", "Insertion", "Merge", "Quick-First", "Quick-Random"};
    SortFunc funcs[6] = {bubble_sort, selection_sort, insertion_sort, merge_sort, quicksort_first, quicksort_random};

    int sizes_small[] = {100, 500, 1000, 2000, 5000, 10000};
    int sizes_large[] = {50000, 100000};
    const char *input_names[3] = {"random", "sorted", "reverse"};

    printf("MODULE D - TIME COMPLEXITY ANALYSIS\n");
    printf("n,input,algorithm,avg_time_seconds\n");

    for (int t = 0; t < 3; t++) {
        for (int si = 0; si < 6; si++) {
            int n = sizes_small[si];
            int *base = NULL;
            if (t == 0) base = make_random(n);
            else if (t == 1) base = make_sorted(n);
            else base = make_reverse(n);

            for (int a = 0; a < 6; a++) {
                if (n >= 5000 && (a == 0 || a == 1)) continue; // skip O(n^2) too slow
                double tm = time_one(funcs[a], base, n, 3);
                printf("%d,%s,%s,%.6f\n", n, input_names[t], names[a], tm);
            }
            free(base);
        }
    }

    printf("\n--- LARGE n (Merge + Quicksort only) ---\n");
    for (int t = 0; t < 3; t++) {
        for (int si = 0; si < 2; si++) {
            int n = sizes_large[si];
            int *base = NULL;
            if (t == 0) base = make_random(n);
            else if (t == 1) base = make_sorted(n);
            else base = make_reverse(n);
            for (int a = 3; a < 6; a++) {
                double tm = time_one(funcs[a], base, n, 3);
                printf("%d,%s,%s,%.6f\n", n, input_names[t], names[a], tm);
            }
            free(base);
        }
    }
    return 0;
}

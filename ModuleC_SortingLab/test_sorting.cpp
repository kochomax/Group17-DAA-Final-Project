// test_sorting.cpp - tests the back end WITHOUT any interface
#include "sorting.h"
#include <iostream>
using namespace algo;
static int in[MAX_N], out[MAX_N];
int main() {
    int fails = 0;
    seedRandom(12345);
    int sizes[] = {1, 2, 3, 10, 100, 1000, 10000, 100000};
    for (int t = 0; t < 3; t++) for (int si = 0; si < 8; si++) {
        int n = sizes[si]; std::string err;
        if (t == 0) generateRandom(in, n, -50, 50, err);   // lots of duplicates
        else if (t == 1) fillSorted(in, n); else fillReverse(in, n);
        for (int k = 0; k < NUM_ALGOS; k++) {
            if (n > 10000 && k < 3) continue;               // skip slow O(n^2) at 100k
            Stats s; runSort((AlgoId)k, in, n, out, s);
            if (!isSorted(out, n)) { std::cout << "FAIL " << algoName((AlgoId)k) << " n=" << n << " type=" << t << "\n"; fails++; }
        }
    }
    // known counts: sorted input n=1000
    fillSorted(in, 1000); Stats s;
    runSort(INSERTION, in, 1000, out, s);  std::cout << "insertion sorted: cmp=" << s.comparisons << " (expect 999)\n";
    runSort(BUBBLE, in, 1000, out, s);     std::cout << "bubble sorted: cmp=" << s.comparisons << " (expect 999)\n";
    runSort(QUICK_FIRST, in, 1000, out, s);std::cout << "quick-first sorted: cmp=" << s.comparisons << " (expect 499500)\n";
    runSort(QUICK_RANDOM, in, 1000, out, s);std::cout << "quick-random sorted: cmp=" << s.comparisons << " (much smaller)\n";
    // validation
    int a[10], n; std::string err;
    std::cout << parseNumbers("", a, 10, n, err) << " " << err << "\n";
    std::cout << parseNumbers("5, x, 3", a, 10, n, err) << " " << err << "\n";
    std::cout << parseNumbers("5 -3,8  1", a, 10, n, err) << " n=" << n << "\n";
    std::cout << generateRandom(a, 0, 1, 2, err) << " " << err << "\n";
    // steps
    StepLog log; runSort(MERGE, a, n, out, s, &log); std::cout << "merge steps: " << log.size() << "\n";
    std::cout << (fails ? "TESTS FAILED\n" : "ALL SORT TESTS PASSED\n");
    return fails;
}

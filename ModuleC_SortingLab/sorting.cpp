// sorting.cpp  --  Module C (Sorting Lab) BACK END implementation
#include "sorting.h"
#include <chrono>

namespace algo {

// ===================== small helpers =====================

static unsigned rngState = 2463534242u;

void seedRandom(unsigned seed) { rngState = seed ? seed : 2463534242u; }

// xorshift32: our own pseudo-random generator.  O(1)
static unsigned nextRandom() {
    rngState ^= rngState << 13;
    rngState ^= rngState >> 17;
    rngState ^= rngState << 5;
    return rngState;
}

void copyArray(const int* src, int* dst, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

bool isSorted(const int* a, int n) {
    for (int i = 1; i < n; i++)
        if (a[i - 1] > a[i]) return false;
    return true;
}

static void swapInts(int& x, int& y) { int t = x; x = y; y = t; }

// ===================== StepLog =====================

StepLog::StepLog() : steps_(nullptr), count_(0), cap_(0) {}

StepLog::~StepLog() {
    for (int i = 0; i < count_; i++) delete[] steps_[i].data;
    delete[] steps_;
}

void StepLog::add(const int* a, int n, int hi1, int hi2, const std::string& label) {
    if (count_ == cap_) {                       // grow (doubling)
        int newCap = cap_ ? cap_ * 2 : 16;
        Step* bigger = new Step[newCap];
        for (int i = 0; i < count_; i++) bigger[i] = steps_[i];   // shallow copy of pointers
        delete[] steps_;
        steps_ = bigger;
        cap_ = newCap;
    }
    Step& st = steps_[count_++];
    st.data = new int[n];
    copyArray(a, st.data, n);
    st.n = n;
    st.hi1 = hi1;
    st.hi2 = hi2;
    st.label = label;
}

static std::string num(int v) {
    if (v == 0) return "0";
    std::string r;
    bool neg = v < 0;
    long long x = neg ? -(long long)v : v;
    while (x > 0) { r = char('0' + x % 10) + r; x /= 10; }
    return neg ? "-" + r : r;
}

// ===================== 1. Bubble sort =====================
// Time: best O(n) (already sorted, early exit), average/worst O(n^2). Space O(1). Stable.
void bubbleSort(int* a, int n, Stats& s, StepLog* log) {
    if (log) log->add(a, n, -1, -1, "Initial array");
    for (int pass = 1; pass < n; pass++) {
        bool swapped = false;
        for (int j = 0; j < n - pass; j++) {
            s.comparisons++;
            if (a[j] > a[j + 1]) {
                swapInts(a[j], a[j + 1]);
                s.swaps++;
                swapped = true;
            }
        }
        if (log) log->add(a, n, n - pass, -1, "Pass " + num(pass) + " done (largest of the rest bubbled to the right)");
        if (!swapped) break;                    // no swaps => already sorted
    }
}

// ===================== 2. Selection sort =====================
// Time: O(n^2) in ALL cases. Space O(1). Not stable (long-distance swap).
void selectionSort(int* a, int n, Stats& s, StepLog* log) {
    if (log) log->add(a, n, -1, -1, "Initial array");
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            s.comparisons++;
            if (a[j] < a[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            swapInts(a[i], a[minIdx]);
            s.swaps++;
        }
        if (log) log->add(a, n, i, minIdx, "Pass " + num(i + 1) + ": minimum placed at index " + num(i));
    }
}

// ===================== 3. Insertion sort =====================
// Time: best O(n) (sorted), average/worst O(n^2). Space O(1). Stable.
void insertionSort(int* a, int n, Stats& s, StepLog* log) {
    if (log) log->add(a, n, -1, -1, "Initial array");
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0) {
            s.comparisons++;
            if (a[j] > key) {
                a[j + 1] = a[j];                // shift right
                s.moves++;
                j--;
            } else {
                break;
            }
        }
        if (j + 1 != i) {
            a[j + 1] = key;
            s.moves++;
        }
        if (log) log->add(a, n, j + 1, i, "Pass " + num(i) + ": inserted " + num(key) + " into sorted part [0.." + num(i) + "]");
    }
}

// ===================== 4. Merge sort =====================
// Time: O(n log n) in all cases. Extra space O(n). Stable (take left on ties).
static void merge(int* a, int* tmp, int lo, int mid, int hi, Stats& s, StepLog* log, int n) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        s.comparisons++;
        if (a[i] <= a[j]) tmp[k++] = a[i++];    // <= keeps stability
        else              tmp[k++] = a[j++];
        s.moves++;
    }
    while (i <= mid) { tmp[k++] = a[i++]; s.moves++; }
    while (j <= hi)  { tmp[k++] = a[j++]; s.moves++; }
    for (int x = lo; x <= hi; x++) a[x] = tmp[x];   // copy back (not counted twice)
    if (log) log->add(a, n, lo, hi, "Merged [" + num(lo) + ".." + num(mid) + "] and [" + num(mid + 1) + ".." + num(hi) + "]");
}

static void mergeSortRec(int* a, int* tmp, int lo, int hi, Stats& s, StepLog* log, int n) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSortRec(a, tmp, lo, mid, s, log, n);
    mergeSortRec(a, tmp, mid + 1, hi, s, log, n);
    merge(a, tmp, lo, mid, hi, s, log, n);
}

void mergeSort(int* a, int n, Stats& s, StepLog* log) {
    if (log) log->add(a, n, -1, -1, "Initial array");
    if (n < 2) return;
    int* tmp = new int[n];
    mergeSortRec(a, tmp, 0, n - 1, s, log, n);
    delete[] tmp;
}

// ===================== 5. Quicksort =====================
// First-element pivot, "yellow/green" partition:
//   a[lo]            = pivot
//   a[lo+1 .. i]     = GREEN  (processed, < pivot)
//   a[i+1 .. j-1]    = YELLOW (processed, >= pivot)
//   a[j .. hi]       = not yet examined
// At the end the pivot is swapped with the last green element.
// Time: best/average O(n log n), worst O(n^2) (sorted input with first pivot).
// Extra space: O(log n) because we recurse on the SMALLER side and loop on the
// larger one (this also stops stack overflow on sorted 100,000 items).
// Not stable.
static int partitionFirst(int* a, int lo, int hi, Stats& s) {
    int pivot = a[lo];
    int i = lo;                                  // last green index
    for (int j = lo + 1; j <= hi; j++) {
        s.comparisons++;
        if (a[j] < pivot) {                      // a[j] is green
            i++;
            if (i != j) { swapInts(a[i], a[j]); s.swaps++; }
        }                                        // else it stays yellow
    }
    if (i != lo) { swapInts(a[lo], a[i]); s.swaps++; }
    return i;                                    // final index of pivot
}

static void quickRec(int* a, int lo, int hi, Stats& s, StepLog* log, int n, bool randomPivot) {
    while (lo < hi) {
        if (randomPivot) {                       // move a random element to the front
            int r = lo + (int)(nextRandom() % (unsigned)(hi - lo + 1));
            if (r != lo) { swapInts(a[lo], a[r]); s.swaps++; }
        }
        int pivotValue = a[lo];
        int p = partitionFirst(a, lo, hi, s);
        if (log) log->add(a, n, p, -1, "Partition [" + num(lo) + ".." + num(hi) + "] with pivot " + num(pivotValue) + " -> pivot now at index " + num(p));
        if (p - lo < hi - p) { quickRec(a, lo, p - 1, s, log, n, randomPivot); lo = p + 1; }
        else                 { quickRec(a, p + 1, hi, s, log, n, randomPivot); hi = p - 1; }
    }
}

void quickSortFirstPivot(int* a, int n, Stats& s, StepLog* log) {
    if (log) log->add(a, n, -1, -1, "Initial array");
    quickRec(a, 0, n - 1, s, log, n, false);
}

void quickSortRandomPivot(int* a, int n, Stats& s, StepLog* log) {
    if (log) log->add(a, n, -1, -1, "Initial array");
    quickRec(a, 0, n - 1, s, log, n, true);
}

// ===================== driver =====================

static void dispatch(AlgoId id, int* a, int n, Stats& s, StepLog* log) {
    switch (id) {
        case BUBBLE:       bubbleSort(a, n, s, log); break;
        case SELECTION:    selectionSort(a, n, s, log); break;
        case INSERTION:    insertionSort(a, n, s, log); break;
        case MERGE:        mergeSort(a, n, s, log); break;
        case QUICK_FIRST:  quickSortFirstPivot(a, n, s, log); break;
        case QUICK_RANDOM: quickSortRandomPivot(a, n, s, log); break;
    }
}

void runSort(AlgoId id, const int* input, int n, int* out, Stats& s, StepLog* log) {
    s = Stats();
    copyArray(input, out, n);

    unsigned savedSeed = rngState;               // so both runs use the same pivots
    typedef std::chrono::high_resolution_clock Clock;
    Clock::time_point t0 = Clock::now();
    dispatch(id, out, n, s, nullptr);            // timed run: no snapshots
    Clock::time_point t1 = Clock::now();
    s.timeMs = std::chrono::duration<double, std::milli>(t1 - t0).count();

    if (log) {                                   // second run just to record the steps
        int* tmp = new int[n];
        copyArray(input, tmp, n);
        Stats ignored;
        rngState = savedSeed;
        dispatch(id, tmp, n, ignored, log);
        delete[] tmp;
    }
}

void compareAll(const int* input, int n, CompareRow* rows) {
    int* out = new int[n];
    for (int k = 0; k < NUM_ALGOS; k++) {
        rows[k].name = algoName((AlgoId)k);
        runSort((AlgoId)k, input, n, out, rows[k].stats, nullptr);
    }
    delete[] out;
}

static const AlgoInfo INFO[NUM_ALGOS] = {
    {"Bubble sort",              "O(n)",       "O(n^2)",     "O(n^2)", "O(1)",     "Yes"},
    {"Selection sort",           "O(n^2)",     "O(n^2)",     "O(n^2)", "O(1)",     "No"},
    {"Insertion sort",           "O(n)",       "O(n^2)",     "O(n^2)", "O(1)",     "Yes"},
    {"Merge sort",               "O(n log n)", "O(n log n)", "O(n log n)", "O(n)", "Yes"},
    {"Quicksort (first pivot)",  "O(n log n)", "O(n log n)", "O(n^2)", "O(log n)", "No"},
    {"Quicksort (random pivot)", "O(n log n)", "O(n log n)", "O(n^2) (very unlikely)", "O(log n)", "No"},
};

const AlgoInfo& algoInfo(AlgoId id) { return INFO[id]; }
const char* algoName(AlgoId id) { return INFO[id].name; }

// ===================== input helpers =====================

bool parseNumbers(const std::string& text, int* out, int maxN, int& n, std::string& err) {
    n = 0;
    size_t i = 0, len = text.size();
    while (i < len) {
        char c = text[i];
        if (c == ' ' || c == ',' || c == '\t') { i++; continue; }
        // read one token
        size_t start = i;
        while (i < len && text[i] != ' ' && text[i] != ',' && text[i] != '\t') i++;
        std::string tok = text.substr(start, i - start);

        size_t p = 0;
        bool neg = false;
        if (tok[0] == '-' || tok[0] == '+') { neg = (tok[0] == '-'); p = 1; }
        if (p >= tok.size()) { err = "'" + tok + "' is not a number."; return false; }
        long long v = 0;
        for (; p < tok.size(); p++) {
            if (tok[p] < '0' || tok[p] > '9') { err = "'" + tok + "' is not a whole number."; return false; }
            v = v * 10 + (tok[p] - '0');
            if (v > 1000000000LL) { err = "'" + tok + "' is out of range (use -1,000,000,000 to 1,000,000,000)."; return false; }
        }
        if (n >= maxN) { err = "Too many values (maximum " + num(maxN) + ")."; return false; }
        out[n++] = (int)(neg ? -v : v);
    }
    if (n == 0) { err = "Input is empty. Type some numbers separated by commas or spaces."; return false; }
    return true;
}

bool generateRandom(int* a, int n, int lo, int hi, std::string& err) {
    if (n < 1 || n > MAX_N) { err = "n must be between 1 and " + num(MAX_N) + "."; return false; }
    if (lo > hi) { err = "Minimum value cannot be greater than the maximum."; return false; }
    unsigned long long range = (unsigned long long)((long long)hi - (long long)lo + 1);
    for (int i = 0; i < n; i++) a[i] = (int)((long long)lo + (long long)(nextRandom() % range));
    return true;
}

void fillSorted(int* a, int n)  { for (int i = 0; i < n; i++) a[i] = i + 1; }
void fillReverse(int* a, int n) { for (int i = 0; i < n; i++) a[i] = n - i; }

}  // namespace algo

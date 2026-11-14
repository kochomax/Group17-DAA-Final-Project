// sorting.h  --  Module C (Sorting Lab) BACK END
// CSC2107 Algorithm Explorer
//
// No std::sort, std::vector, std::swap, etc. are used: only plain arrays and
// our own code. The front end (console, Qt, web...) only needs this header.
//
// Counting conventions (state these in the report):
//   comparisons : every comparison between two ARRAY ELEMENTS
//   swaps       : every exchange of two array elements
//   moves       : every single assignment of an element into the array
//                 (insertion-sort shifts, merge-sort writes)
#ifndef SORTING_H
#define SORTING_H

#include <string>

namespace algo {

const int MAX_N = 100000;          // largest list the lab accepts
const int NUM_ALGOS = 6;           // 5 required + randomised quicksort

enum AlgoId { BUBBLE = 0, SELECTION, INSERTION, MERGE, QUICK_FIRST, QUICK_RANDOM };

struct Stats {
    long long comparisons = 0;
    long long swaps = 0;
    long long moves = 0;
    double timeMs = 0.0;           // high_resolution_clock, milliseconds
};

// One snapshot of the array for the Step / Auto-play display.
struct Step {
    int* data;                     // copy of the array at this moment
    int n;
    int hi1, hi2;                  // indices to highlight (-1 = none)
    std::string label;             // e.g. "Pass 2 done"
};

// Growable list of snapshots (own dynamic array).
class StepLog {
public:
    StepLog();
    ~StepLog();
    void add(const int* a, int n, int hi1, int hi2, const std::string& label);
    int size() const { return count_; }
    const Step& at(int i) const { return steps_[i]; }
private:
    StepLog(const StepLog&);               // not copyable
    StepLog& operator=(const StepLog&);
    Step* steps_;
    int count_, cap_;
};

struct AlgoInfo {
    const char* name;
    const char* best;
    const char* average;
    const char* worst;
    const char* space;
    const char* stable;
};

struct CompareRow {
    const char* name;
    Stats stats;
};

// ---- the sorting algorithms (each sorts a[0..n-1] in place) --------------
// log may be nullptr (no snapshots).
void bubbleSort   (int* a, int n, Stats& s, StepLog* log = nullptr);
void selectionSort(int* a, int n, Stats& s, StepLog* log = nullptr);
void insertionSort(int* a, int n, Stats& s, StepLog* log = nullptr);
void mergeSort    (int* a, int n, Stats& s, StepLog* log = nullptr);
void quickSortFirstPivot(int* a, int n, Stats& s, StepLog* log = nullptr);
void quickSortRandomPivot(int* a, int n, Stats& s, StepLog* log = nullptr);

// ---- driver functions ----------------------------------------------------
// Sorts a COPY of input: timed run with no logging, then (if log != nullptr)
// a second run on another copy that records steps. Returns sorted copy in out
// (out must hold n ints). Counts are identical in both runs.
void runSort(AlgoId id, const int* input, int n, int* out, Stats& s, StepLog* log = nullptr);

// Runs all 6 algorithms on the same input; rows[NUM_ALGOS] filled in.
void compareAll(const int* input, int n, CompareRow* rows);

const AlgoInfo& algoInfo(AlgoId id);
const char* algoName(AlgoId id);

// ---- input helpers with validation ---------------------------------------
// Parses "5, 3 8,1" style text. Returns false and sets err on bad input.
bool parseNumbers(const std::string& text, int* out, int maxN, int& n, std::string& err);
// Fills a[0..n-1] with random values in [lo, hi]. Returns false + err if invalid.
bool generateRandom(int* a, int n, int lo, int hi, std::string& err);
// Pattern generators for Module D (random / sorted / reverse).
void fillSorted(int* a, int n);
void fillReverse(int* a, int n);
void seedRandom(unsigned seed);

bool isSorted(const int* a, int n);
void copyArray(const int* src, int* dst, int n);

}  // namespace algo

#endif

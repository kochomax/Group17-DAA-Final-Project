// main.cpp  --  Module C (Sorting Lab) console FRONT END
// Only talks to the back end through sorting.h.
#include "sorting.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <thread>

using namespace algo;

static int data[MAX_N];       // the user's current list
static int dataN = 0;

static bool readInt(const std::string& line, long long& out) {
    size_t i = 0; bool neg = false;
    if (line.empty()) return false;
    if (line[0] == '-') { neg = true; i = 1; }
    if (i >= line.size()) return false;
    long long v = 0;
    for (; i < line.size(); i++) {
        if (line[i] < '0' || line[i] > '9') return false;
        v = v * 10 + (line[i] - '0');
        if (v > 1000000000LL) return false;
    }
    out = neg ? -v : v;
    return true;
}

static long long askInt(const std::string& prompt) {
    for (;;) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return 0;
        long long v;
        if (readInt(line, v)) return v;
        std::cout << "  ! Please type a whole number.\n";
    }
}

// Prints array; elements at hi1/hi2 are shown in [brackets] (highlight).
static void printArray(const int* a, int n, int hi1, int hi2) {
    const int LIMIT = 40;
    for (int i = 0; i < n && i < LIMIT; i++) {
        if (i == hi1 || i == hi2) std::cout << "[" << a[i] << "] ";
        else                      std::cout << " " << a[i] << "  ";
    }
    if (n > LIMIT) std::cout << "... (" << n - LIMIT << " more)";
    std::cout << "\n";
}

static void showData() {
    std::cout << "Current list (" << dataN << " items): ";
    printArray(data, dataN, -1, -1);
}

static void enterNumbers() {
    std::cout << "Type numbers separated by commas or spaces: ";
    std::string line;
    std::getline(std::cin, line);
    static int tmp[MAX_N]; int n; std::string err;
    if (!parseNumbers(line, tmp, MAX_N, n, err)) { std::cout << "  ! " << err << "\n"; return; }
    copyArray(tmp, data, n); dataN = n;
    showData();
}

static void randomNumbers() {
    long long n  = askInt("How many numbers (n)? ");
    long long lo = askInt("Smallest value? ");
    long long hi = askInt("Largest value? ");
    std::string err;
    if (n < 1 || n > MAX_N) { std::cout << "  ! n must be between 1 and " << MAX_N << ".\n"; return; }
    if (!generateRandom(data, (int)n, (int)lo, (int)hi, err)) { std::cout << "  ! " << err << "\n"; return; }
    dataN = (int)n;
    showData();
}

static void printStats(const Stats& s) {
    std::cout << "  Comparisons: " << s.comparisons
              << "   Swaps: " << s.swaps
              << "   Moves: " << s.moves
              << "   Time: " << std::fixed << std::setprecision(4) << s.timeMs << " ms\n";
}

static void printInfo(const AlgoInfo& i) {
    std::cout << "  " << i.name << ": best " << i.best << ", average " << i.average
              << ", worst " << i.worst << ", extra space " << i.space
              << ", stable: " << i.stable << "\n";
}

static void runOne() {
    if (dataN == 0) { std::cout << "  ! Enter or generate a list first.\n"; return; }
    std::cout << "1 Bubble  2 Selection  3 Insertion  4 Merge  5 Quicksort (first pivot)  6 Quicksort (random pivot)\n";
    long long c = askInt("Algorithm: ");
    if (c < 1 || c > NUM_ALGOS) { std::cout << "  ! Choose 1-6.\n"; return; }
    AlgoId id = (AlgoId)(c - 1);

    static int out[MAX_N];
    Stats s;
    StepLog log;
    runSort(id, data, dataN, out, s, &log);

    std::cout << "\n" << algoName(id) << " - " << log.size() << " steps recorded. "
              << "Press Enter = next step, a = auto-play, q = skip to result.\n";
    bool autoPlay = false; int delay = 700;
    for (int k = 0; k < log.size(); k++) {
        const Step& st = log.at(k);
        std::cout << "Step " << k << "/" << log.size() - 1 << "  " << st.label << "\n   ";
        printArray(st.data, st.n, st.hi1, st.hi2);
        if (k == log.size() - 1) break;
        if (autoPlay) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        } else {
            std::string cmd; std::getline(std::cin, cmd);
            if (cmd == "q") { 
                const Step& last = log.at(log.size() - 1);
                std::cout << "Final: "; printArray(last.data, last.n, -1, -1); break; }
            if (cmd == "a") {
                long long d = askInt("Delay between steps in ms (50-5000): ");
                delay = (int)(d < 50 ? 50 : (d > 5000 ? 5000 : d));
                autoPlay = true;
            }
        }
    }
    std::cout << "\nSorted result: "; printArray(out, dataN, -1, -1);
    printStats(s);
    printInfo(algoInfo(id));
}

static void compareAllUI() {
    if (dataN == 0) { std::cout << "  ! Enter or generate a list first.\n"; return; }
    CompareRow rows[NUM_ALGOS];
    compareAll(data, dataN, rows);
    std::cout << "\n" << std::left << std::setw(26) << "Algorithm"
              << std::right << std::setw(14) << "Comparisons" << std::setw(12) << "Swaps"
              << std::setw(12) << "Moves" << std::setw(14) << "Time (ms)" << "\n";
    std::cout << std::string(78, '-') << "\n";
    for (int k = 0; k < NUM_ALGOS; k++) {
        std::cout << std::left << std::setw(26) << rows[k].name << std::right
                  << std::setw(14) << rows[k].stats.comparisons
                  << std::setw(12) << rows[k].stats.swaps
                  << std::setw(12) << rows[k].stats.moves
                  << std::setw(14) << std::fixed << std::setprecision(4) << rows[k].stats.timeMs << "\n";
    }
}

static void helpPanel() {
    std::cout << "\nComplexity summary\n";
    for (int k = 0; k < NUM_ALGOS; k++) printInfo(algoInfo((AlgoId)k));
}

int main() {
    seedRandom((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count());
    for (;;) {
        std::cout << "\n===== SORTING LAB =====\n"
                  << "1 Enter numbers   2 Generate random   3 Show list\n"
                  << "4 Sort + step through   5 Compare all   6 Complexity help   0 Exit\n";
        long long c = askInt("Choice: ");
        switch (c) {
            case 1: enterNumbers(); break;
            case 2: randomNumbers(); break;
            case 3: if (dataN) showData(); else std::cout << "  (no list yet)\n"; break;
            case 4: runOne(); break;
            case 5: compareAllUI(); break;
            case 6: helpPanel(); break;
            case 0: return 0;
            default: std::cout << "  ! Choose 0-6.\n";
        }
    }
}

Algorithm Explorer: Module C (Sorting Lab)

CSC2107 Design and Analysis of Algorithms, Group Project, Advent Semester 2026
Uganda Christian University

Language:C++ (C++11 standard)
Group number:17
Members: _<names and registration numbers>

What this module does

The user enters a list of numbers (or generates `n` random ones). The program sorts it with:

1. Bubble sort
2. Selection sort
3. Insertion sort
4. Merge sort
5. Quicksort, first-element pivot (yellow/green partition)
6. Quicksort, randomised pivot (second option required by the brief)

For every run it shows the sorted result, the number of comparisons, swaps/moves and the execution time (high-resolution timer). It can also step through the array after every pass or merge/partition step, run all algorithms side by side on the same input, and show each algorithm's best/average/worst-case time, extra space and stability.

Files

| File | Role |
|------|------|
| `sorting.h` | Back end: declarations, counters, step log, input helpers |
| `sorting.cpp` | Back end: all six algorithms, timing, validation (no interface code) |
| `main.cpp` | Front end: console menu, stepping, Compare all, help panel |
| `test_sorting.cpp` | Tests the back end on its own, with no interface |

The algorithms live only in `sorting.h` / `sorting.cpp`, so they can be tested and reused (for example by Module D) without the interface.

Requirements

- A C++ compiler that supports C++11, for example:
  - g++(MinGW-w64 on Windows, or GCC on Linux/macOS)
  - Visual Studio(MSVC) on Windows
- No external libraries or other dependencies to install.

 How to build and run

 g++ (Windows with MinGW-w64, Linux, macOS)

Open a terminal in this folder and run:


g++ -std=c++11 -O2 -o sortlab main.cpp sorting.cpp


Then start the program:

```
./sortlab        (Linux / macOS)
sortlab          (Windows)
```

Visual Studio

1. Create a new empty C++ Console project.
2. Add `sorting.h`, `sorting.cpp` and `main.cpp` to it.
3. Build and run (Ctrl+F5).

Running the tests

```
g++ -std=c++11 -O2 -o test_sorting test_sorting.cpp sorting.cpp
./test_sorting
```

It should end with `ALL SORT TESTS PASSED`.

Using the program

```
1 Enter numbers   2 Generate random   3 Show list
4 Sort + step through   5 Compare all   6 Complexity help   0 Exit
```

- **Enter numbers:** type values separated by commas or spaces, for example `5, 3 8, 1`. Empty or non-numeric input shows an error message.
- **Generate random:** give `n` (1 to 100000), the smallest value and the largest value.
- **Sort + step through:** choose an algorithm. Press **Enter** for the next step, **a** for auto-play (you choose the delay in milliseconds), or **q** to skip to the result. Elements being compared, swapped or placed are shown in `[brackets]`.
- Compare all:runs all six algorithms on the current list and prints one table.
- **Complexity help:** best, average and worst case, extra space and stability.

 What the counters mean

- Comparisons: every comparison between two array elements.
- Swaps: every exchange of two array elements.
- Moves:every single assignment of an element into the array (insertion sort shifts, merge sort writes).

The time is measured on a run with no step recording, so recording the steps does not slow the measurement.

 Implementation notes

- Written from first principles: plain arrays only, with no `std::sort`, `std::vector`, `std::swap` or similar library routines doing the core work.
- Quicksort partition:the pivot is the first element. Elements smaller than the pivot form the green region, elements greater than or equal to it form the yellow region, and the pivot is swapped into place at the end.
- Quicksort recurses on the smaller side and loops on the larger side. This keeps stack use at O(log n), so first-pivot quicksort on already-sorted input of 100,000 numbers does not overflow the stack. The operation counts are unchanged.
- Merge sort is stable (ties take the left element first).

 



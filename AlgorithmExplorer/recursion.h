
#ifndef RECURSION_H
#define RECURSION_H

// Fibonacci algorithms
long long fibonacciNaive(int n, long long *calls);

long long fibonacciMemo(int n, long long memo[],
                        long long *calls);

long long fibonacciDP(int n, long long *iterations);

// Factorial algorithms
long long factorialRecursive(int n, long long *calls);

long long factorialIterative(int n, long long *iterations);

#endif
 
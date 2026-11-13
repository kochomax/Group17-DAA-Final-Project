
#include "recursion.h"

// Naive recursive Fibonacci
long long fibonacciNaive(int n, long long *calls)
{
    (*calls)++;

    if (n <= 1)
    {
        return n;
    }

    return fibonacciNaive(n - 1, calls)
         + fibonacciNaive(n - 2, calls);
}

// Memoized Fibonacci
long long fibonacciMemo(int n, long long memo[],
                        long long *calls)
{
    (*calls)++;

    if (n <= 1)
    {
        return n;
    }

    if (memo[n] != -1)
    {
        return memo[n];
    }

    memo[n] = fibonacciMemo(n - 1, memo, calls)
            + fibonacciMemo(n - 2, memo, calls);

    return memo[n];
}

// Bottom-up dynamic programming Fibonacci
long long fibonacciDP(int n, long long *iterations)
{
    if (n <= 1)
    {
        return n;
    }

    long long previous = 0;
    long long current = 1;

    for (int i = 2; i <= n; i++)
    {
        long long next = previous + current;

        previous = current;
        current = next;

        (*iterations)++;
    }

    return current;
}

// Recursive factorial
long long factorialRecursive(int n, long long *calls)
{
    (*calls)++;

    if (n == 0 || n == 1)
    {
        return 1;
    }

    return n * factorialRecursive(n - 1, calls);
}

// Iterative factorial
long long factorialIterative(int n, long long *iterations)
{
    long long result = 1;

    for (int i = 2; i <= n; i++)
    {
        result *= i;
        (*iterations)++;
    }

    return result;
}

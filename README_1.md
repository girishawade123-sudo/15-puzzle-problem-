# Smart Delivery Planning – Fractional Knapsack (C)

A menu-driven C program that solves the **Fractional Knapsack** problem with a **greedy algorithm**.

A delivery vehicle has a fixed carrying capacity and a set of packages, each with a weight and a profit. The vehicle can carry a complete package or a fraction of one. The program finds the load that gives the **maximum total value**.

## How it works

1. Compute the **value/weight ratio** (profit ÷ weight) of every package.
2. **Sort** the packages in decreasing order of ratio using **merge sort**.
3. Go through the sorted list:
   - if the whole package fits, take all of it;
   - otherwise take only the fraction that fills the remaining space. Every package after that gets 0.
4. Report the packages chosen, the total weight used and the maximum value.

Taking the best ratio first is optimal here because fractions are allowed, so no unit of capacity can be used better.

## Menu

```
1. Enter Package Details
2. Display Package Details
3. Calculate Value/Weight Ratio
4. Sort Packages by Ratio
5. Find Maximum Value
6. Display Selected Packages
7. Exit
```

Use the options in order: 1, 3, 4, 5, 6. Each step depends on the one before it, and the program does not check the order. Option 7 prints the time complexity and exits.

## Build and run

Requires a C compiler such as GCC.

```bash
gcc knapsack.c -o knapsack
./knapsack          # on Windows: knapsack.exe
```

## Example

| Package | Profit | Weight | Ratio |
|---------|--------|--------|-------|
| 1       | 40     | 5      | 8     |
| 2       | 30     | 10     | 3     |
| 3       | 50     | 5      | 10    |
| 4       | 20     | 4      | 5     |

Vehicle capacity = **15**

Packages sorted by ratio: 3, 1, 4, 2.

```
Package  Fraction  Weight used  Value gained
3        1.00      5.00         50.00
1        1.00      5.00         40.00
4        1.00      4.00         20.00
2        0.10      1.00         3.00

Total weight used : 15.00
Maximum value     : 113.00
```

## Time complexity

| Step                | Complexity     | Reason                            |
|---------------------|----------------|-----------------------------------|
| Ratio calculation   | O(n)           | one pass over the packages        |
| Merge sort          | O(n log n)     | log n levels, n work per level    |
| Greedy selection    | O(n)           | one pass over the sorted packages |
| **Overall**         | **O(n log n)** | sorting dominates                 |

Extra space: O(n) for the temporary arrays used in merging.

## Notes

- Supports up to 100 packages.
- Package weights must be greater than 0.
- The original package numbers are kept while sorting, so the output always refers to the right package.

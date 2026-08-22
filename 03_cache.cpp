#include <iostream>
#include <chrono>   // Provides a high-precision digital stopwatch to measure CPU execution time



// Set a permanent grid size of 8,000 rows and 8,000 columns (64 million total numbers).
// Compile-time constant definition.
const int SIZE = 8000;

// Create a massive 2D memory array (allocates ~256 MB of RAM).
// BSS/Data Segment Global Allocation. Stored contiguously in physical RAM.
int matrix[SIZE][SIZE];

int main() {
    // Put numbers into the grid so the compiler doesn't throw away our loops.
    // Prevents "Dead-Code Elimination" during compiler optimization (-O2).
    for (int i = 0; i < SIZE; i++) matrix[i][i] = i + 1;

    // =========================================================================
    // TEST 1: ROW-MAJOR ACCESS (CACHE-FRIENDLY / SEQUENTIAL)
    // =========================================================================

    // Start the stopwatch for Test 1.
    // Read start timestamp from hardware clock registers (RDTSC).
    auto start1 = std::chrono::high_resolution_clock::now();
    long long sum1 = 0;

    // Read numbers left-to-right, row-by-row (Row 0, Row 1, Row 2...).
    // Sequential Memory Access. Matches physical array layout in RAM.
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            sum1 += matrix[i][j];
            // HARDWARE EXPLANATION: Reading matrix[0][0] causes the CPU Hardware Prefetcher
            // to fetch an entire 64-byte "Cache Line" (16 numbers) into ultra-fast L1 Cache.
            // The next 15 reads result in sub-nanosecond "Cache Hits".
        }
    }

    // Stop the stopwatch for Test 1.
    auto end1 = std::chrono::high_resolution_clock::now();

    // =========================================================================
    // TEST 2: COLUMN-MAJOR ACCESS (CACHE-UNFRIENDLY / STRIDED)
    // =========================================================================

    // Start the stopwatch for Test 2.
    auto start2 = std::chrono::high_resolution_clock::now();
    long long sum2 = 0;

    // Read numbers top-to-bottom, column-by-column (Column 0 down, Column 1 down...).
    // Strided Memory Access (Non-sequential access pattern).
    for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE; i++) {
            sum2 += matrix[i][j]; 
            // HARDWARE EXPLANATION: Each iteration jumps 32,000 bytes forward in RAM!
            // This bypasses loaded cache lines, causing constant "L1/L2 Cache Misses".
            // The CPU stalls for ~50-100 nanoseconds waiting for slow main system RAM.
        }
    }

    // Stop the stopwatch for Test 2.
    auto end2 = std::chrono::high_resolution_clock::now();

    // =========================================================================
    // PRINT RESULTS
    // =========================================================================

    std::cout << "Sum Check: " << sum1 + sum2 << "\n";

    // Convert duration ticks to milliseconds and display output
    std::cout << "Row-Major (Sequential): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << " ms\n";

    std::cout << "Column-Major (Strided): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << " ms\n";

    return 0; // Exit program cleanly
}

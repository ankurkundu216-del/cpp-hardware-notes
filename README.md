# C++ Hardware Interaction & Low-Level Notes

Detailed notes, assembly analyses, and cache benchmarks demonstrating how C++ maps directly to CPU architecture, physical memory management, and compiler optimizations.

---

## 1. Memory Addresses & Raw Pointers (`main.cpp`)

* **Virtual RAM Addresses (`0x7ffe...`)**: Pointer addresses map directly to virtual memory locations managed by the CPU Memory Management Unit (MMU).
* **Data Allocation Sizes**: Standard integers (`int`) occupy 4 bytes (32-bit), whereas 64-bit architecture pointers require 8 bytes to address the system memory space.
* **Unoptimized Assembly Analysis (`-O0`)**:
  * `sub rsp, 32`: Expands the local call stack frame down by 32 bytes to reserve memory.
  * `mov DWORD PTR [rbp-20], 42`: Writes the value `42` directly to a specific 4-byte stack offset in RAM.
  * `lea rax, [rbp-20]`: Executes **Load Effective Address** to extract the RAM memory address into register `rax`.

---

## 2. Zero-Cost Abstractions & Optimizations (`02_zero_cost.cpp`)

* **Function Inlining**: High-level C++ templates generate zero runtime function call overhead compared to manual C/Assembly code.
* **Aggressive Compiler Optimization (`-O3`)**:
  * **Constant Folding**: Pre-evaluates expressions (`10 * 2`) during compilation instead of at runtime.
  * **Register Assignment (`mov esi, 20`)**: Deletes intermediate pointer variables and hardcodes calculated values directly into CPU registers.
  * Eliminates stack write/read operations entirely.

---

## 3. Hardware Cache Locality & Memory Traversal (`03_cache.cpp`)

* **Sequential vs. Strided Memory Access**:
  * **Row-Major Access (`matrix[i][j]`)**: **~75 ms** — Reads memory sequentially. Triggers the CPU Hardware Prefetcher to load 64-byte **Cache Lines** into ultra-fast L1 Data Cache (~1 ns access), maximizing **Cache Hits**.
  * **Column-Major Access (`matrix[j][i]`)**: **~104 ms** — Jumps 32,000 bytes per iteration. Bypasses loaded cache lines and causes severe **L1/L2 Cache Misses**, stalling the CPU while fetching data from slow system RAM (~50–100 ns access).

---

## 4. Branch Prediction & Pipeline Stalls (`04_branch.cpp`)

* **CPU Instruction Pipeline**: Modern CPUs speculatively execute instructions down conditional branches (`if/else`) before evaluating conditions to keep the hardware pipeline saturated.
* **Unsorted Data (Random Branches)**: Flips condition results unpredictably, causing severe **Branch Mispredictions**. The CPU must flush the pipeline and discard instructions, causing severe CPU stalls.
* **Sorted Data (Predictable Branches)**: Allows the hardware branch predictor to reach near 100% accuracy, maintaining full execution throughput.

---

## 5. Heap vs. Stack Allocation Mechanics (`05_heap_vs_stack.cpp`)

* **Stack Allocation**: **~145 µs** — Involves adjusting the stack pointer register (`sub rsp, N`). Operates in $O(1)$ constant time with zero kernel/allocator overhead and optimal L1 cache locality.
* **Heap Allocation (`malloc` / `free`)**: **~4227 µs** (**~29x slower**) — Incurs significant C runtime (`glibc`) allocator overhead, searching free-list bins, acquiring thread locks, updating allocation tracking metadata headers, and increasing memory fragmentation.

---

## 6. Useful CLI Commands

```bash
# View human-readable Intel Assembly with main function highlighted
g++ -O3 -S -masm=intel filename.cpp -o assembly.s && grep -A 25 "main:" assembly.s

# Run Linux hardware performance counters to inspect cache misses and branch mispredictions
perf stat ./04_branch

# Compile and run cache locality benchmark
g++ -O2 03_cache.cpp -o 03_cache && ./03_cache

# Compile and run branch prediction benchmark
g++ -O2 04_branch.cpp -o 04_branch && ./04_branch

# Compile and run heap vs stack allocation benchmark
g++ -O2 05_heap_vs_stack.cpp -o 05_heap_vs_stack && ./05_heap_vs_stack

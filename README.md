# C++ Hardware Interaction & Low-Level Notes

Detailed notes and x86-64 assembly analyses demonstrating how C++ maps directly to CPU execution, physical memory management, and compiler optimizations.

---

## 1. Memory Addresses & Raw Pointers (`main.cpp`)

* **Virtual RAM Addresses (`0x7ffe...`)**: Pointer addresses map directly to virtual RAM locations managed by the CPU Memory Management Unit (MMU).
* **Data Allocation Sizes**: Standard integers (`int`) occupy 4 bytes (32-bit), whereas 64-bit architecture pointers require 8 bytes to address the entire system memory space.
* **Unoptimized Assembly Analysis (`-O0`)**:
  * `sub rsp, 32`: Expands the local call stack frame down by 32 bytes to reserve memory.
  * `mov DWORD PTR [rbp-20], 42`: Writes the value `42` directly to a specific 4-byte stack offset in RAM.
  * `lea rax, [rbp-20]`: Executes **Load Effective Address** to extract the RAM memory address into register `rax`.

---

## 2. Zero-Cost Abstractions & Optimizations (`02_zero_cost.cpp`)

* **Function Inlining**: High-level C++ templates and abstractions generate zero runtime function overhead compared to raw C/Assembly.
* **Aggressive Compiler Optimization (`-O3`)**:
  * **Constant Folding**: The compiler pre-evaluates `10 * 2` during compilation instead of at runtime.
  * **Register Assignment (`mov esi, 20`)**: The compiler completely deletes unnecessary intermediate variables and hardcodes the calculated value directly into register `esi`.
  * Eliminates pointer dereferencing and stack write/read overhead entirely.

---

## 3. Useful Assembly & Performance Commands

```bash
# View human-readable Intel Assembly with main function highlighted
g++ -O3 -S -masm=intel filename.cpp -o assembly.s && grep -A 25 "main:" assembly.s

# Run Linux hardware performance counters to check CPU cache misses
perf stat ./binary_name

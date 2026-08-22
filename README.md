cat << 'EOF' > README.md
# C++ Hardware Interaction & Low-Level Notes

Notes and assembly analysis on how C++ maps directly to CPU architecture, memory addresses, and hardware optimizations.

## 1. Memory Addresses & Raw Pointers (`main.cpp`)
- **Addresses (`0x7ffe...`)**: Pointer addresses map directly to virtual RAM locations.
- **Data Sizes**: Standard `int` takes 4 bytes (32-bit), while 64-bit pointers occupy 8 bytes to address physical/virtual RAM.
- **Assembly Insight**: Unoptimized (`-O0`) code uses explicit stack pointer operations (`sub rsp, 32`) and direct `mov` instructions to assign values to RAM offsets.

## 2. Zero-Cost Abstractions (`02_zero_cost.cpp`)
- **Function Inlining**: High-level C++ features like templates compile down to the exact same or better assembly than manual C code.
- **Compiler Optimization (`-O3`)**: Evaluates constant expressions at compile-time and hardcodes results into CPU registers (e.g., `mov esi, 20`), avoiding runtime function calls entirely.
EOF

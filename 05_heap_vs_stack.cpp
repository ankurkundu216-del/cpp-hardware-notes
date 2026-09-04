#include <iostream>
#include <chrono>
#include <cstdlib>

const int ITERATIONS = 100000;
const int ELEMENT_COUNT = 1024; // 4 KB allocation block

struct StackBuffer {
    int data[ELEMENT_COUNT];
};

int main() {
    // =========================================================================
    // TEST 1: STACK ALLOCATION
    // =========================================================================
    auto start1 = std::chrono::high_resolution_clock::now();
    
    long long stack_sum = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        StackBuffer buf;
        buf.data[0] = i;
        
        // COMPILER BARRIER: Forces compiler to treat buf as used/escaped memory
        asm volatile("" : : "g"(&buf) : "memory");
        
        stack_sum += buf.data[0];
    }
    
    auto end1 = std::chrono::high_resolution_clock::now();

    // =========================================================================
    // TEST 2: HEAP ALLOCATION (malloc / free)
    // =========================================================================
    auto start2 = std::chrono::high_resolution_clock::now();
    
    long long heap_sum = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        // Force raw call to C runtime heap allocator (glibc malloc)
        int* heap_ptr = static_cast<int*>(std::malloc(ELEMENT_COUNT * sizeof(int)));
        heap_ptr[0] = i;
        
        // COMPILER BARRIER: Prevents compiler from deleting malloc/free
        asm volatile("" : : "g"(heap_ptr) : "memory");
        
        heap_sum += heap_ptr[0];
        std::free(heap_ptr);
    }
    
    auto end2 = std::chrono::high_resolution_clock::now();

    // =========================================================================
    // DISPLAY RESULTS
    // =========================================================================
    std::cout << "Sum Check: " << stack_sum + heap_sum << "\n";
    
    std::cout << "Stack Allocation Time: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count() << " us\n";
              
    std::cout << "Heap Allocation Time:  " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count() << " us\n";

    return 0;
}

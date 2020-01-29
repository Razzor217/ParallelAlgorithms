#include <omp.h>
#include <chrono>
#include <iostream>

#include "GlobalAND.h"

int main() {

    const size_t size = 200000;
    std::vector<bool> a(size, true);
    a[size - 1] = false;
    
    std::cout << "Vector size: " << a.size() << std::endl;

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();

    std::cout << "Parallel computation took " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
        << " Microseconds.\n";

    start = std::chrono::system_clock::now();
    end = std::chrono::system_clock::now();

    std::cout << "Sequential computation took " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
        << " Microseconds.\n";
    return 0;
}
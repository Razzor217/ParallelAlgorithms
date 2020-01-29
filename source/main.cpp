#include <omp.h>
#include <chrono>
#include <iostream>

#include "PRAM-CRCW/Maximum.h"

int maximum(std::vector<int>& a, int& maximum)
{
    maximum = 0;
    int index = 0;
    for (size_t i = 0; i < a.size(); ++i)
    {
        if (a[i] > maximum)
        {
            maximum = a[i];
            index = i;
        }
    }
    return index;
}

int main() {

    const size_t size = 200000;
    std::vector<int> a(size, 1);
    a[0] = 2;
    std::vector<bool> m;
    int max;
    
    std::cout << "Vector size: " << a.size() << std::endl;

    auto start = std::chrono::system_clock::now();
    pram::maximum_omp(a, m);
    auto end = std::chrono::system_clock::now();

    std::cout << "Parallel computation took " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
        << " Microseconds.\n";

    start = std::chrono::system_clock::now();
    int index = maximum(a, max);
    end = std::chrono::system_clock::now();

    std::cout << "Sequential computation took " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
        << " Microseconds.\n";

    std::cout << "Parallel maximum at index 0: " << m[0] << std::endl;
    std::cout << "Sequential maximum at index 0: " << index << std::endl;
    return 0;
}

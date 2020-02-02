#include <chrono>
#include <iostream>

#include "LinearAlgebra/MatrixMultiplication.h"

#include "PRAM-CRCW/GlobalAND.h"
#include "PRAM-CRCW/GlobalOR.h"
#include "PRAM-CRCW/Maximum.h"

#include "Dummy.h"

int main() {

    // TODO: Boost Unit Tests for algorithms

    Eigen::MatrixXf a(4, 4);
    Eigen::MatrixXf b(4, 4);

    a << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;
    b = 2 * Eigen::Matrix4f::Identity();

    std::cout << a << std::endl;
    std::cout << b << std::endl;

    auto start = std::chrono::system_clock::now();
    auto c = algebra::matrix_mult_omp(a, b);
    auto end = std::chrono::system_clock::now();

    std::cout << "Parallel computation took " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
        << " Microseconds.\n";

    std::cout << c << std::endl;

    return 0;
}

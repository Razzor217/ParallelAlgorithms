#pragma once

#include <eigen3/Eigen/Eigen>

namespace algebra
{
    // Todo: simplest matrix multiplication algorithm with
    // #processing units = (#elements)^2

 
    Eigen::MatrixXf matrix_mult_omp(Eigen::MatrixXf& a, Eigen::MatrixXf& b)
    {
        // quadratic matrices
        int n = a.rows();
        Eigen::MatrixXf c(n, n);

        #pragma omp parallel for collapse(2)
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                // one PE calculates c(i, j)
                for (int k = 0; k < n; ++k)
                {
                    c(i, j) += a(i, k) * b(k, j);
                }
            }
        }
        return c;
    }
}

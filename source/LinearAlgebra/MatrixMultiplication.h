/**
 * @file MatrixMultiplication.h
 *
 * @brief Implementations of various parallel matrix multiplication algorithms.
 */

#pragma once

#include <eigen3/Eigen/Eigen>

namespace algebra
{
    /**
     * @brief Theoretical algorithm for matrix multiplication in CRCW PRAM.
     *
     * Implementation for square matrices in R^(n x n).
     * Feasible for (number of processing units) = n^2

     *
     * @param a Left matrix in R^(n x n)
     * @param b right matrix in R^(n x n)
     * @return resulting matrix in R^(n x n)
     */
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

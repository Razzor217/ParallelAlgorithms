/**
 * @file MatrixMultiplication.h
 *
 * @brief Implementations of various parallel matrix multiplication algorithms.
 */

#pragma once

#include <eigen3/Eigen/Eigen>
#include <cmath>

#include <mpi.h>

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
    Eigen::MatrixXf matrix_mult_omp(Eigen::MatrixXf& a, Eigen::MatrixXf& b);

    /**
     * @brief MPI implementation of matrix multiplication adapted from Cannon.
     * 
     * Distributed memory algorithm which uses 
     * (number of processing units) = (number of rows)^2.
     * Can only be used with square matrices.
     * 
     * @see matrix_mult_mpi_cannon_batch()
     * 
     * @param[in] a Left matrix of size n x n
     * @param[in] b Right matrix of size n x n
     * @param[inout] c Resulting matrix of size n x n
     * @param[in] comm MPI Communicator
     */
    void matrix_mult_mpi_cannon(Eigen::MatrixXf& a, 
        Eigen::MatrixXf& b, 
        Eigen::MatrixXf& c, 
        MPI_Comm comm);

    /**
     * @brief MPI implementation of matrix multiplication adapted from Cannon.
     * 
     * Distributed memory algorithm which divides the input and output matrices 
     * into smaller sub-matrices. Let m := sqrt(p). Then n x n matrix 
     * multiplication is broken down into several multiplications of 
     * (n / m) x (n / m) matrices.
     * 
     * @see matrix_mult_mpi_cannon()
     *
     * @param[in] a Left matrix of size n x n
     * @param[in] b Right matrix of size n x n
     * @param[inout] c Resulting matrix of size n x n
     * @param[in] comm MPI Communicator
     */
    void matrix_mult_mpi_cannon_batch(Eigen::MatrixXf& a, 
        Eigen::MatrixXf& b, 
        Eigen::MatrixXf& c, 
        MPI_Comm comm);

    /**
     * @brief MPI implementation of matrix multiplication adapted from 
     * Dekel, Nassimi and Sahni.
     *
     * 
     *
     * @param[in] a Left matrix of size n x n
     * @param[in] b Right matrix of size n x n
     * @param[inout] c Resulting matrix of size n x n
     * @param[in] comm MPI Communicator
     */
    void matrix_mult_mpi_dns(Eigen::MatrixXf& a, 
        Eigen::MatrixXf& b, 
        Eigen::MatrixXf& c, 
        MPI_Comm comm);
}

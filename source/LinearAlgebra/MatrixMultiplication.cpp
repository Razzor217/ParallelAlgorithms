#include "MatrixMultiplication.h"

namespace algebra
{
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

    void matrix_mult_mpi_cannon(Eigen::MatrixXf& a, 
        Eigen::MatrixXf& b, 
        Eigen::MatrixXf& c, 
        MPI_Comm comm)
    {
        /*
        * This version works only for (number of PEs) = p = n * n.
        * For the sake of simplicity, size checks for matrices are omitted.
        */
        MPI_Status status;
        int p;
        int rank;
        MPI_Comm_size(comm, &p);
        MPI_Comm_rank(comm, &rank);

        /*
        * PEs are arranged in a n x n grid.
        * Determine row and column from the PE rank.
        * Note: division into subproblems of size 1 x 1.
        */
        int i = rank / p;
        int j = rank % p;

        int k = (i + j) % p;
        float a1 = a(i, k);
        float b1 = b(k, j);
        c(i, j) = 0.f;

        for (int l = 0; l < p; ++l)
        {
            c(i, j) += a1 * b1;

            /*
            * send a to PE(i, (j + n - 1) mod n)
            * send b to PE((i + n - 1) mod n, j)
            */
            MPI_Bsend(&a1, 1, MPI_FLOAT, p * i + ((j + p - 1) % p), 0, comm);
            MPI_Bsend(&b1, 1, MPI_FLOAT, p * ((i + p - 1) % p) + j, 0, comm);

            float a2;
            float b2;

            /*
            * receive a2 from PE(i, (j + 1) mod n)
            * receive b2 from PE((i + 1) mod n, j)
            */
            MPI_Recv(&a2, 1, MPI_FLOAT, p * i + ((j + 1) % p), 0, comm, &status);
            MPI_Recv(&b2, 1, MPI_FLOAT, p * ((i + 1) % p) + j, 0, comm, &status);

            a1 = a2;
            b1 = b2;
        }
    }

    void matrix_mult_mpi_cannon_batch(Eigen::MatrixXf& a, 
        Eigen::MatrixXf& b, 
        Eigen::MatrixXf& c, 
        MPI_Comm comm)
    {
        MPI_Status status;
        int p;
        int rank;
        int n = a.rows();

        MPI_Comm_size(comm, &p);
        MPI_Comm_rank(comm, &rank);

        /*
         * PEs are arranged in a m x m grid.
         * Determine row and column from the PE rank.
         * Note: 
         *   1. assume p is a quadratic number
         *   2. assume m divides n
         *   3. division into subproblems of size (n / m) x (n / m).
         */
        int m = std::sqrt(p);
        int i = rank / m;
        int j = rank % m;

        int k = (i + j) % m;
        Eigen::MatrixXf a1(n / m, n / m);
        Eigen::MatrixXf b1(n / m, n / m);

        /*
         * Compute start / end indices of sub-matrices
         */
        int i_n1 = (n / m) * i;
        int i_n2 = (n / m) * (i + 1) - 1;
        int j_n1 = (n / m) * j;
        int j_n2 = (n / m) * (j + 1) - 1;
        int k_n1 = (n / m) * k;
        int k_n2 = (n / m) * (k + 1) - 1;
        
        /*
         * a1 gets sub-matrix a(i, k) of size (n / m) x (n / m)
         * b1 gets sub-matrix b(k, j) of size (n / m) x (n / m)
         */
        a1 << a.block(i_n1, k_n1, i_n2, k_n2);
        b1 << b.block(k_n1, j_n1, k_n2, j_n2);

        for (int l = 0; l < k; ++l)
        {
            /*
             * Multiplication and addition of (n / m) x (n / m) matrices
             */
            c.block(i_n1, j_n1, i_n2, j_n2) += a1 * b1;

            /*
            * send a to PE(i, (j + m - 1) mod m)
            * send b to PE((i + m - 1) mod m, j)
            */
            MPI_Bsend(a1.data(), (n / m) * (n / m), MPI_FLOAT, m * i + ((j + m - 1) % m), 0, comm);
            MPI_Bsend(b1.data(), (n / m) * (n / m), MPI_FLOAT, m * ((i + m - 1) % m) + j, 0, comm);

            /*
            * receive a2 from PE(i, (j + 1) mod n)
            * receive b2 from PE((i + 1) mod n, j)
            */
            MPI_Recv(a1.data(), (n / m) * (n / m), MPI_FLOAT, m * i + ((j + 1) % m), 0, comm, &status);
            MPI_Recv(b1.data(), (n / m) * (n / m), MPI_FLOAT, m * ((i + 1) % m) + j, 0, comm, &status);
        }
    }

    void matrix_mult_mpi_cannon_batch_dense(Eigen::MatrixXf& a, Eigen::MatrixXf& b, Eigen::MatrixXf& c, MPI_Comm comm)
    {
        MPI_Status status;
        int p, rank, n = a.rows();
        MPI_Comm_size(comm, &p);
        MPI_Comm_rank(comm, &rank);

        int m = std::sqrt(p);
        int i = rank / m, j = rank % m, k = (i + j) % m;
        Eigen::MatrixXf a1(n / m, n / m);
        Eigen::MatrixXf b1(n / m, n / m);

        int i_n1 = (n / m) * i, i_n2 = (n / m) * (i + 1) - 1;
        int j_n1 = (n / m) * j, j_n2 = (n / m) * (j + 1) - 1;
        int k_n1 = (n / m) * k, k_n2 = (n / m) * (k + 1) - 1;
        a1 << a.block(i_n1, k_n1, i_n2, k_n2);
        b1 << b.block(k_n1, j_n1, k_n2, j_n2);

        for (int l = 0; l < k; ++l)
        {
            c.block(i_n1, j_n1, i_n2, j_n2) += a1 * b1;
            MPI_Bsend(a1.data(), (n / m) * (n / m), MPI_FLOAT, m * i + ((j + m - 1) % m), 0, comm);
            MPI_Bsend(b1.data(), (n / m) * (n / m), MPI_FLOAT, m * ((i + m - 1) % m) + j, 0, comm);
            MPI_Recv(a1.data(), (n / m) * (n / m), MPI_FLOAT, m * i + ((j + 1) % m), 0, comm, &status);
            MPI_Recv(b1.data(), (n / m) * (n / m), MPI_FLOAT, m * ((i + 1) % m) + j, 0, comm, &status);
        }
    }

    void matrix_mult_mpi_dns(Eigen::MatrixXf& a, 
        Eigen::MatrixXf& b, 
        Eigen::MatrixXf& c, 
        MPI_Comm comm)
    {
        /*
         * source code goes here.
         */
    }
}
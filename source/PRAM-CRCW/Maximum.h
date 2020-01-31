/**
 * @file Maximum.h
 *
 * @brief Implementation of theoretical algorithms.
 */

#pragma once

#include <vector>

#include "GlobalAND.h"

namespace pram
{
    /**
     * @brief Theoretical algorithm for maximum computation in CRCW PRAM.
     *
     * Feasible for (number of processing units) = ( number of elements)^2
     *
     * @param[in] a Vector of integers whose maximum is to be determined
     * @param[out] m Vector of unit literals, true represents maximum element
     */
    void maximum_omp(std::vector<int>& a, std::vector<bool>& m)
    {
        std::vector<bool> b(a.size() * a.size());
        m.resize(a.size());

        #pragma omp parallel for collapse(2)
        for (size_t i = 0; i < a.size(); ++i)
        {
            for (size_t j = 0; j < a.size(); ++j)
            {
                b[a.size() * i + j] = (a[i] >= a[j]);
            }
        }

        #pragma omp parallel for
        for (size_t i = 0; i < a.size(); ++i)
        {
            // get vector slice
            auto first = b.cbegin() + a.size() * i;
            auto last = b.cbegin() + a.size() * (i + 1) - 1;
            std::vector<bool> v(first, last);
            // parallel AND
            m[i] = global_and_omp(v);
        }
    }
}

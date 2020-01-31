/**
 * @file GlobalAND.h
 *
 * @brief Implementation of parallel global AND for CRCW PRAM.
 */

#pragma once

#include <vector>

namespace pram
{
    /**
     * @brief Theoretical algorithm for global AND in CRCW PRAM.
     *
     * Feasible for (number of processing units) = (number of elements)
     *
     * @param a Vector of unit literals
     * @return true iff all elements are true, false otherwise
     */
    bool global_and_omp(std::vector<bool>& a)
    {
        bool result = true;

        #pragma omp parallel for
        for (size_t i = 0; i < a.size(); ++i)
        {
            if (!a[i])
            {
                result = false;
            }
        }
        return result;
    }
}

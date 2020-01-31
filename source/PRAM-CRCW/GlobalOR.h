#pragma once

#include <omp.h>
#include <vector>

namespace pram
{
    /**
     * Theoretical algorithm for global OR in CRCW PRAM.
     * Feasible for #processing units = #elements
     *
     * @param a Vector of unit literals
     * @return true iff there exists an element which is true, false otherwise
     */
    bool global_or_omp(std::vector<bool>& a)
    {
        bool result = false;

        #pragma omp parallel for
        for (size_t i = 0; i < a.size(); ++i)
        {
            if (a[i])
            {
                result = true;
            }
        }
        return result;
    }
}

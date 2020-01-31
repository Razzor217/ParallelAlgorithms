#pragma once

#include <omp.h>
#include <vector>

namespace pram
{
    /**
     * Theoretical algorithm for global AND in CRCW PRAM.
     * Feasible for #processing units = #elements
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

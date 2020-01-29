#pragma once

#include <omp.h>
#include <vector>

namespace pram
{
    /*
     * Theoretical algorithm for global OR in CRCW PRAM
     * Feasible for #processing units = #elements
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

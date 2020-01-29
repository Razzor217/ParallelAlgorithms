#pragma once

#include <omp.h>
#include <vector>

namespace omp
{
    /*
     * Theoretical algorithm for global OR in CRCW PRAM
     * Feasible for #elements = #processing units
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

    /*
     * Theoretical algorithm for global AND in CRCW PRAM
     * Feasible for #elements = #processing units
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

    void maximum_omp(std::vector<int>& a, std::vector<int>& m)
    {
        std::vector<std::vector<int>> b(a.size())

        #pragma omp parallel for collapse(2)
        for (size_t i = 0)
    }
}
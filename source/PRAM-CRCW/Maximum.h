#pragma once

#include <omp.h>
#include <vector>

#include "GlobalAND.h"

namespace pram
{
    /*
     * Theoretical algorithm for maximum computation in CRCW PRAM
     * Feasible for #processing units = (#elements)^2
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

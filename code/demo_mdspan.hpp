#pragma once

#include "mdspan.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace demo_mdspan
{
    namespace stdex = std::experimental;

    float *foo()
    {
        static float f;
        return &f;
    }

    std::array d{
        0, 5, 1, 3, 8, 4, 2, 7, 6,
    };

    void Demo_mdspan()
    {
        stdex::mdspan m{d.data(), stdex::extents{3, 3}};
        static_assert(m.rank() == 2, "Rank is two");

        for (std::size_t i = 0; i < m.extent(0); ++i)
            for (std::size_t j = 0; j < m.extent(1); ++j)
    #if MDSPAN_USE_BRACKET_OPERATOR
                std::cout << "m[" << i << ", " << j << "] == " << m[i, j]
                        << " Using []\n";
    #else
                std::cout << "m(" << i << ", " << j << ") == " << m(i, j)
                        << " Using ()\n";
    #endif

        stdex::mdspan m0{d.data()};
        static_assert(m0.rank() == 0, "Rank is zero");
    #if MDSPAN_USE_BRACKET_OPERATOR
        std::cout << "m0[] == " << m0[] << " Using []\n";
    #else
        std::cout << "m0() == " << m0() << " Using ()\n";
    #endif

        double c[8];
        stdex::mdspan mc(c);
        static_assert(mc.rank() == 1, "Rank is one");
        static_assert(mc.extent(0) == 8, "Extent is 8");

        float *fp = foo();
        stdex::mdspan mf(fp);
        static_assert(mf.rank() == 0, "Rank is zero");
    }

} // namespace demo_mdspan

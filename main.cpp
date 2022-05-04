#pragma once

#include <iostream>
#include <boost/rational.hpp>

#include "monomial.h"

int main() {
    { // Test monomial
        Monomial<boost::rational<int>> mon({36, 48}, {
                {0, 1},
                {2, 2},
                {3, 3}});
        auto mon2 = mon;
        assert(mon == mon2);
        assert(mon * Monomial<boost::rational<int>>(2) == mon2 + mon2);
        assert(mon - mon == Monomial<boost::rational<int>>(0));
        assert(mon * mon == Monomial<boost::rational<int>>({9, 16}, {{0, 2},
                                                                     {2, 4},
                                                                     {3, 6}}));
        assert(mon2 / mon == Monomial<boost::rational<int>>(1));
        mon2 *= mon2;
        assert(mon * mon == mon2);
        assert(mon2 / mon == mon);
        assert(!mon2.IsDivisibleBy(Monomial<boost::rational<int>>(1, {{100, 1}})));
        std::cout << "---------------------Monomial output example----------------------------\n";
        std::cout << mon << "\n";
        std::cout << "------------------------------------------------------------------------\n\n";
    }
    return 0;
}
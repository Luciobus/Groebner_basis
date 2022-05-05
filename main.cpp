#pragma once

#include <iostream>
#include <boost/rational.hpp>

#include "monomial.h"
#include "monomial_order.h"

int main() {
    {   // Test monomial
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
    {   // Monomial order test
        Monomial<boost::rational<int>> m1(1, {{0, 1}, {2, 2}, {3, 4}});
        Monomial<boost::rational<int>> m2(2, {{0, 1}, {2, 2}, {3, 4}});
        Monomial<boost::rational<int>> m3({2, 3}, {{0, 1}, {2, 3}, {3, 3}});
        Monomial<boost::rational<int>> m4(0, {{0, 1}, {2, 3}, {3, 3}});
        {   // Lex order subtest
            assert(!Lex<boost::rational<int>>::less(m1, m2));
            assert(Lex<boost::rational<int>>::less_or_equal(m1, m2));
            assert(Lex<boost::rational<int>>::greater(m3, m1));
            assert(Lex<boost::rational<int>>::greater_or_equal(m2, m4));
        }
        {   // DegLex order subtest
            assert(!DegLex<boost::rational<int>>::less(m1, m2));
            assert(DegLex<boost::rational<int>>::less_or_equal(m1, m2));
            assert(DegLex<boost::rational<int>>::greater(m2, m4));
            assert(DegLex<boost::rational<int>>::greater_or_equal(m3, m2));
        }
        {   // DegRevLex order subtest
            assert(!DegRevLex<boost::rational<int>>::less(m1, m2));
            assert(DegRevLex<boost::rational<int>>::less_or_equal(m1, m2));
            assert(DegRevLex<boost::rational<int>>::greater(m1, m3));
            assert(DegRevLex<boost::rational<int>>::greater_or_equal(m1, m4));
        }
    }
    return 0;
}
#pragma once

#include <iostream>
#include <boost/rational.hpp>

#include "monomial.h"
#include "monomial_order.h"
#include "polynomial.h"
#include "polynomial_order.h"
#include "algorithms.h"

int main() {
    namespace gr = groebner;
    {   // Test monomial
        gr::Monomial mon({
                                 {0,  1},
                                 {2,  2},
                                 {3,  3},
                                 {10, 0}});
        auto mon2 = mon;
        assert(mon == mon2);
        assert(mon * mon == gr::Monomial({{0, 2},
                                          {2, 4},
                                          {3, 6}}));
        assert(mon2 / mon == gr::Monomial());
        mon2 *= mon2;
        assert(mon * mon == mon2);
        assert(mon2 / mon == mon);
        assert(!mon2.IsDivisibleBy(gr::Monomial({{100, 1}})));
        std::cout << "---------------------Monomial output example----------------------------\n";
        std::cout << mon << "\n";
        std::cout << "------------------------------------------------------------------------\n\n";
    }
    {   // Monomial order test
        gr::Monomial m1({{0, 1},
                         {2, 2},
                         {3, 4}});
        gr::Monomial m2({{0, 1},
                         {2, 2},
                         {3, 4}});
        gr::Monomial m3({{0, 1},
                         {2, 3},
                         {3, 3}});
        gr::Monomial m4({{0, 1},
                         {2, 3},
                         {3, 3}});
        {   // Lex order subtest
            assert(!gr::Lex::less()(m1, m2));
            assert(gr::Lex::less_or_equal()(m1, m2));
            assert(gr::Lex::greater()(m3, m1));
            assert(gr::Lex::greater_or_equal()(m4, m2));
        }
        {   // DegLex order subtest
            assert(!gr::DegLex::less()(m1, m2));
            assert(gr::DegLex::less_or_equal()(m1, m2));
            assert(gr::DegLex::greater()(m4, m2));
            assert(gr::DegLex::greater_or_equal()(m3, m2));
        }
        {   // DegRevLex order subtest
            assert(!gr::DegRevLex::less()(m1, m2));
            assert(gr::DegRevLex::less_or_equal()(m1, m2));
            assert(gr::DegRevLex::greater()(m1, m3));
            assert(gr::DegRevLex::greater_or_equal()(m1, m4));
        }
    }
    {   // Test polynomial
        gr::Polynomial<boost::rational<int>> pol({{gr::Monomial({{33, 2},
                                                                 {10, 100}}), 16},
                                                  {gr::Monomial({{1, 2},
                                                                 {2, 100}}),  10}});
        gr::Polynomial<boost::rational<int>> pol1({{gr::Monomial({{33, 2},
                                                                  {10, 100}}), 16},
                                                   {gr::Monomial({{1, 2},
                                                                  {2, 100}}),  10}});
        gr::Polynomial<boost::rational<int>> poly({{gr::Monomial({{100, 1},
                                                                  {10,  100}}), 8},
                                                   {gr::Monomial({{1, 2},
                                                                  {2, 100}}),   -10}});
        std::cout << "-------------------Polynomial output example----------------------------\n";
        std::cout << pol << "\n";
        std::cout << "------------------------------------------------------------------------\n\n";
        assert(pol == pol1);
        assert(pol != poly);
        assert(pol - pol == gr::Polynomial<boost::rational<int>>());
        assert(pol + pol == pol * gr::Polynomial<boost::rational<int>>({{gr::Monomial(), 2}}));
        assert(gr::Polynomial<boost::rational<int>>() * gr::Polynomial<boost::rational<int>>() ==
               gr::Polynomial<boost::rational<int>>());
        assert(pol * poly == gr::Polynomial<boost::rational<int>>({{gr::Monomial({{1,   2},
                                                                                  {2,   100},
                                                                                  {10,  100},
                                                                                  {100, 1}}), 80},
                                                                   {gr::Monomial({{10,  200},
                                                                                  {33,  2},
                                                                                  {100, 1}}), 128},
                                                                   {gr::Monomial({{1,  2},
                                                                                  {2,  100},
                                                                                  {10, 100},
                                                                                  {33, 2}}),  -160},
                                                                   {gr::Monomial({{1, 4},
                                                                                  {2, 200}}), -100}}));
        assert(pol + poly == gr::Polynomial<boost::rational<int>>({{gr::Monomial({{10,  100},
                                                                                  {100, 1}}), 8},
                                                                   {gr::Monomial({{10, 100},
                                                                                  {33, 2}}),  16}}));
    }
    {
        using Poly = gr::Polynomial<boost::rational<int>, groebner::DegLex::greater>;
        std::set<Poly, gr::PolynomialOrder> s;
        s.insert(Poly({{gr::Monomial({{1, 3}}), 1},
                       {gr::Monomial({{1, 1},
                                      {2, 1}}), -2}}));
        s.insert(Poly({{gr::Monomial({{1, 2}, {2, 1}}), 1},
                       {gr::Monomial({{2, 2}}), -2}, {gr::Monomial({{1, 1}}), 1}}));
        std::cout << "--------------------Set of polynoms at beginning-------------------\n";
        for (const auto& p : s) {
            std::cout << p << "\n";
        }
        Poly poly = *s.begin();
        std::cout << "--------------------Extended to Groebner basis-------------------\n";
        gr::Algorithm::ExtendToGroebner(s);
        for (const auto& p : s) {
            std::cout << p << "\n";
        }
        std::cout << "-------------------------Reduced Groebner basis-------------------\n";
        gr::Algorithm::MakeReduced(s);
        for (const auto& p : s) {
            std::cout << p << "\n";
        }
        assert(groebner::Algorithm::IsInGroebnerIdeal(poly, s));
    }
    return 0;
}

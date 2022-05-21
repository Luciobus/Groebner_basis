#include <iostream>
#include <boost/rational.hpp>

#include "modular.h"
#include "monomial.h"
#include "monomial_order.h"
#include "polynomial.h"
#include "polynomial_order.h"
#include "algorithms.h"
#include "tests.h"

namespace groebner {
void test_modular() {
    using Mod = Modular<17>;
    assert(Mod(15) + 15 == Mod(13));
    assert(12 - Mod(15) == Mod(14));

    assert(Mod(12) == Mod(-5));
    assert(Mod(0) == Mod(-0));
    assert(Mod(1) != Mod(-1));

    assert(Mod(1231231) * -16 == Mod(1231231));
    assert(Mod(3) * 2 == 6);
    assert(Mod(10) * Mod(2) == Mod(3));
    assert(Mod(9) / Mod(3) == Mod(3));
    assert(Mod(10) / Mod(3) == Mod(60));
    assert(Mod(100).GetValue() == 15);
    std::cout << "Modular tests passed\n";
}

void test_monomial() {
    Monomial mon({
                         {0,  1},
                         {2,  2},
                         {3,  3},
                         {10, 0}});
    auto mon2 = mon;
    assert(mon == mon2);
    assert(mon * mon == Monomial({{0, 2},
                                  {2, 4},
                                  {3, 6}}));
    assert(mon2 / mon == Monomial());
    mon2 *= mon2;
    assert(mon * mon == mon2);
    assert(mon2 / mon == mon);
    assert(!mon2.IsDivisibleBy(Monomial({{100, 1}})));
    std::cout << "Monomial tests passed\n";
}

void test_monomial_order() {
    Monomial m1({{0, 1},
                 {2, 2},
                 {3, 4}});
    Monomial m2({{0, 1},
                 {2, 2},
                 {3, 4}});
    Monomial m3({{0, 1},
                 {2, 3},
                 {3, 3}});
    Monomial m4({{0, 1},
                 {2, 3},
                 {3, 3}});
    // Lex order subtest
    assert(!Lex::less()(m1, m2));
    assert(Lex::less_or_equal()(m1, m2));
    assert(Lex::greater()(m3, m1));
    assert(Lex::greater_or_equal()(m4, m2));
    // DegLex order subtest
    assert(!DegLex::less()(m1, m2));
    assert(DegLex::less_or_equal()(m1, m2));
    assert(DegLex::greater()(m4, m2));
    assert(DegLex::greater_or_equal()(m3, m2));
    // DegRevLex order subtest
    assert(!DegRevLex::less()(m1, m2));
    assert(DegRevLex::less_or_equal()(m1, m2));
    assert(DegRevLex::greater()(m3, m1));
    assert(DegRevLex::greater_or_equal()(m4, m1));
    std::cout << "Monomial order tests passed\n";
}

void test_polynomial() {
    Polynomial<Modular<131>> pol({{Monomial({{33, 2},
                                             {10, 100}}), 16},
                                  {Monomial({{1, 2},
                                             {2, 100}}),  10}});
    Polynomial<Modular<131>> pol1({{Monomial({{33, 2},
                                              {10, 100}}), 16},
                                   {Monomial({{1, 2},
                                              {2, 100}}),  10}});
    Polynomial<Modular<131>> poly({{Monomial({{100, 1},
                                              {10,  100}}), 8},
                                   {Monomial({{1, 2},
                                              {2, 100}}),   -10}});
    assert(pol == pol1);
    assert(pol != poly);
    assert(pol - pol == Polynomial<Modular<131>>());
    assert(pol + pol == pol * Polynomial<Modular<131>>({{Monomial(), 2}}));
    assert(Polynomial<Modular<131>>() * Polynomial<Modular<131>>() ==
           Polynomial<Modular<131>>());
    assert(pol * poly == Polynomial<Modular<131>>({{Monomial({{1,   2},
                                                              {2,   100},
                                                              {10,  100},
                                                              {100, 1}}), 80},
                                                   {Monomial({{10,  200},
                                                              {33,  2},
                                                              {100, 1}}), 128},
                                                   {Monomial({{1,  2},
                                                              {2,  100},
                                                              {10, 100},
                                                              {33, 2}}),  -160},
                                                   {Monomial({{1, 4},
                                                              {2, 200}}), -100}}));
    assert(pol + poly == Polynomial<Modular<131>>({{Monomial({{10,  100},
                                                              {100, 1}}), 8},
                                                   {Monomial({{10, 100},
                                                              {33, 2}}),  16}}));
    std::cout << "Polynomial tests passed\n";
}

void test_algorithms() {
    using Poly = Polynomial<boost::rational<int>, groebner::DegLex::greater>;
    std::set<Poly, PolynomialOrder> s;
    s.insert(Poly({{Monomial({{1, 3}}), 1},
                   {Monomial({{1, 1},
                              {2, 1}}), -2}}));
    s.insert(Poly({{Monomial({{1, 2},
                              {2, 1}}), 1},
                   {Monomial({{2, 2}}), -2},
                   {Monomial({{1, 1}}), 1}}));
    Poly poly = *s.begin();
    Algorithm::ExtendToGroebner(s);
    Algorithm::MakeReduced(s);
    assert(groebner::Algorithm::IsInGroebnerIdeal(poly, s));
    std::cout << "Algorithms tests passed\n";
}

void test_all() {
    test_modular();
    test_monomial();
    test_monomial_order();
    test_polynomial();
    test_algorithms();
}
} // namespace groebner

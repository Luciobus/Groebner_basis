#pragma once

#include <boost/rational.hpp>

#include "polynomial.h"
#include "algorithms.h"

int main() {
    using Monomial = groebner::Monomial;
    using Polynomial = groebner::Polynomial<boost::rational<int>>;
    Polynomial f1({
                          {Monomial({{0, 3}}), 1}, // x^3
                          {Monomial({{0, 1}, {1, 1}}), -2} // -2xy
                  });
    Polynomial f2({
                          {Monomial({{0, 2}, {1, 1}}), 1}, // x^2y
                          {Monomial({{1, 2}}), -2}, // -y^2
                          {Monomial({{0, 1}}), 1} // x
                  });
    std::set<Polynomial, groebner::PolynomialOrder> ideal({f1, f2});
    groebner::Algorithm::ExtendToGroebner(&ideal);

    for (const auto& f: ideal) {
        std::cout << f << "\n";
    }
    return 0;
}

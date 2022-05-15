#ifndef GROEBNER_BASIS_POLYNOMIAL_ORDER_H
#define GROEBNER_BASIS_POLYNOMIAL_ORDER_H

#include "polynomial.h"

namespace groebner {
struct PolynomialOrder {
    template<class P, class Comparator>
    bool operator()(const Polynomial<P, Comparator>& lhs, const Polynomial<P, Comparator>& rhs) const {
        auto lhs_m = lhs.GetMonomials();
        auto rhs_m = rhs.GetMonomials();
        auto it = lhs_m.cbegin();
        auto jt = rhs_m.cbegin();
        while (it != lhs_m.cend() && jt != rhs_m.cend()) {
            if (*it != *jt) {
                return Comparator()(*it, *jt);
            }
            ++it;
            ++jt;
        }
        return it == lhs_m.cend() && jt != rhs_m.cend();
    }
};
} // namespace groebner

#endif //GROEBNER_BASIS_POLYNOMIAL_ORDER_H

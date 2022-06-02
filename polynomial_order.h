#ifndef GROEBNER_BASIS_POLYNOMIAL_ORDER_H
#define GROEBNER_BASIS_POLYNOMIAL_ORDER_H

#include "polynomial.h"

namespace groebner {
class PolynomialOrder {
public:
    template<class P, class Comparator>
    bool operator()(const Polynomial<P, Comparator>& lhs, const Polynomial<P, Comparator>& rhs) const {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), TermsComparator<P, Comparator>());
    }

private:
    template<class P, class Comparator>
    struct TermsComparator {
        bool operator()(const std::pair<Monomial, P>& lhs, const std::pair<Monomial, P>& rhs) const {
            return Comparator()(lhs.first, rhs.first);
        }
    };
};
} // namespace groebner

#endif //GROEBNER_BASIS_POLYNOMIAL_ORDER_H

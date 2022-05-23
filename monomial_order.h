#ifndef GROEBNER_BASIS_MONOMIAL_ORDER_H
#define GROEBNER_BASIS_MONOMIAL_ORDER_H

#include "monomial.h"

namespace groebner {

namespace Lex {
struct less {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct less_or_equal {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct greater {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct greater_or_equal {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};
} // namespace Lex

namespace Deg {

struct less {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct less_or_equal {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct greater {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct greater_or_equal {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};
} // namespace Deg

namespace RevLex {
struct less {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct less_or_equal {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct greater {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct greater_or_equal {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};
} // namespace RevLex

template<typename FirstOrder = Deg::greater, typename SecondOrder = Lex::greater>
struct OrderPair {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const {
        if (FirstOrder()(lhs, rhs)) {
            return true;
        }
        return !FirstOrder()(rhs, lhs) && SecondOrder()(lhs, rhs);
    }
};

} // namespace groebner

#endif //GROEBNER_BASIS_MONOMIAL_ORDER_H

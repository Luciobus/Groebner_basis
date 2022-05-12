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

namespace DegLex {

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
} // namespace DegLex

namespace DegRevLex {
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
} // namespace DegRevLex

} // namespace groebner

#endif //GROEBNER_BASIS_MONOMIAL_ORDER_H

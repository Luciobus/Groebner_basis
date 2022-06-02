#ifndef GROEBNER_BASIS_MONOMIAL_ORDER_H
#define GROEBNER_BASIS_MONOMIAL_ORDER_H

#include "monomial.h"

namespace groebner {

struct Lex {
    static int cmp(const Monomial& lhs, const Monomial& rhs);

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
};

struct Deg {
    static int cmp(const Monomial& lhs, const Monomial& rhs);

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
};

struct RevLex {
    static int cmp(const Monomial& lhs, const Monomial& rhs);

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
};

template<typename FirstOrder = Deg, typename SecondOrder = Lex>
struct OrderPair {
    static int cmp(const Monomial& lhs, const Monomial& rhs) {
        int first_cmp = FirstOrder::cmp(lhs, rhs);
        return first_cmp ? first_cmp : SecondOrder::cmp(lhs, rhs);
    }

    struct less {
        bool operator()(const Monomial& lhs, const Monomial& rhs) const {
            return cmp(lhs, rhs) < 0;
        }
    };

    struct less_or_equal {
        bool operator()(const Monomial& lhs, const Monomial& rhs) const {
            return cmp(lhs, rhs) <= 0;
        }
    };

    struct greater {
        bool operator()(const Monomial& lhs, const Monomial& rhs) const {
            return cmp(lhs, rhs) > 0;
        }
    };

    struct greater_or_equal {
        bool operator()(const Monomial& lhs, const Monomial& rhs) const {
            return cmp(lhs, rhs) >= 0;
        }
    };
};

using DegLex = OrderPair<Deg, Lex>;
using DegRevLex = OrderPair<Deg, RevLex>;

} // namespace groebner

#endif //GROEBNER_BASIS_MONOMIAL_ORDER_H

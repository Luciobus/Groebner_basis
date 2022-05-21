#include "monomial.h"
#include "monomial_order.h"

namespace groebner {
namespace Lex {
int cmp(const Monomial& lhs, const Monomial& rhs) {
    auto it = lhs.GetPowers().cbegin();
    auto jt = rhs.GetPowers().cbegin();
    while (it != lhs.GetPowers().cend() && jt != rhs.GetPowers().cend()) {
        if (it->first != jt->first) {
            return (it->first < jt->first) - (it->first > jt->first);
        }
        if (it->second != jt->second) {
            return (it->second > jt->second) - (it->second < jt->second);
        }
        ++it;
        ++jt;
    }
    return (jt == rhs.GetPowers().cend()) - (it == lhs.GetPowers().cend());
}

bool less::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) < 0;
}

bool less_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) <= 0;
}

bool greater::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) > 0;
}

bool greater_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) >= 0;
}
} // namespace Lex

namespace DegLex {
int cmp(const Monomial& lhs, const Monomial& rhs) {
    uint64_t lhs_sum = 0, rhs_sum = 0;
    for (const auto& [_, degree]: lhs.GetPowers()) {
        lhs_sum += degree;
    }
    for (const auto& [_, degree]: rhs.GetPowers()) {
        rhs_sum += degree;
    }
    return (lhs_sum > rhs_sum) - (lhs_sum < rhs_sum);
}

bool less::operator()(const Monomial& lhs, const Monomial& rhs) const {
    int cmp_res = cmp(lhs, rhs);
    return cmp_res < 0 || (cmp_res == 0 && Lex::less()(lhs, rhs));
}

bool less_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    int cmp_res = cmp(lhs, rhs);
    return cmp_res < 0 || (cmp_res == 0 && Lex::less_or_equal()(lhs, rhs));
}

bool greater::operator()(const Monomial& lhs, const Monomial& rhs) const {
    int cmp_res = cmp(lhs, rhs);
    return cmp_res > 0 || (cmp_res == 0 && Lex::greater()(lhs, rhs));
}

bool greater_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    int cmp_res = cmp(lhs, rhs);
    return cmp_res > 0 || (cmp_res == 0 && Lex::greater_or_equal()(lhs, rhs));
}
} // namespace DegLex

namespace DegRevLex {
int cmp(const Monomial& lhs, const Monomial& rhs) {
    uint64_t lhs_sum = 0, rhs_sum = 0;
    for (const auto& [_, degree]: lhs.GetPowers()) {
        lhs_sum += degree;
    }
    for (const auto& [_, degree]: rhs.GetPowers()) {
        rhs_sum += degree;
    }
    return (lhs_sum > rhs_sum) - (lhs_sum < rhs_sum);
}

int cmp2(const Monomial& lhs, const Monomial& rhs) {
    auto it = lhs.GetPowers().crbegin();
    auto jt = rhs.GetPowers().crbegin();
    while (it != lhs.GetPowers().crend() && jt != rhs.GetPowers().crend()) {
        if (it->first != jt->first) {
            return (it->first < jt->first) - (it->first > jt->first);
        }
        if (it->second != jt->second) {
            return (it->second > jt->second) - (it->second < jt->second);
        }
        ++it;
        ++jt;
    }
    return (jt == lhs.GetPowers().crend()) - (it == rhs.GetPowers().crend());
}

bool less::operator()(const Monomial& lhs, const Monomial& rhs) const {
    int cmp_res = cmp(lhs, rhs);
    return cmp_res < 0 || (cmp_res == 0 && cmp2(lhs, rhs) > 0);
}

bool less_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    int cmp_res = cmp(lhs, rhs);
    return cmp_res < 0 || (cmp_res == 0 && cmp2(lhs, rhs) >= 0);
}

bool greater::operator()(const Monomial& lhs, const Monomial& rhs) const {
    int cmp_res = cmp(lhs, rhs);
    return cmp_res > 0 || (cmp_res == 0 && cmp2(lhs, rhs) < 0);
}

bool greater_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    int cmp_res = cmp(lhs, rhs);
    return cmp_res > 0 || (cmp_res == 0 && cmp2(lhs, rhs) <= 0);
}
} // namespace DegRevLex
} // namespace groebner

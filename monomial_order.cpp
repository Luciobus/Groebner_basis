#include "monomial.h"
#include "monomial_order.h"

namespace groebner {
namespace Lex {
namespace {
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
} // anonymous namespace

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

namespace Deg {
namespace {
int cmp(const Monomial& lhs, const Monomial& rhs) {
    Monomial::Degree_t lhs_sum = lhs.GetTotalDegree(), rhs_sum = rhs.GetTotalDegree();
    return (lhs_sum > rhs_sum) - (lhs_sum < rhs_sum);
}
} // anonymous namespace

bool less::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) < 0;
}

bool less_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) < 0;
}

bool greater::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) > 0;
}

bool greater_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) >= 0;
}
} // namespace Deg

namespace RevLex {
namespace {
int cmp(const Monomial& lhs, const Monomial& rhs) {
    auto it = lhs.GetPowers().crbegin();
    auto jt = rhs.GetPowers().crbegin();
    while (it != lhs.GetPowers().crend() && jt != rhs.GetPowers().crend()) {
        if (it->first != jt->first) {
            return (it->first > jt->first) - (it->first < jt->first);
        }
        if (it->second != jt->second) {
            return (it->second < jt->second) - (it->second > jt->second);
        }
        ++it;
        ++jt;
    }
    return (it == lhs.GetPowers().crend()) - (jt == rhs.GetPowers().crend());
}
} // anonymous namespace

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
} // namespace RevLex

} // namespace groebner

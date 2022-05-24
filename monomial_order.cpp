#include "monomial_order.h"

namespace groebner {
int Lex::cmp(const Monomial& lhs, const Monomial& rhs) {
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

bool Lex::less::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) < 0;
}

bool Lex::less_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) <= 0;
}

bool Lex::greater::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) > 0;
}

bool Lex::greater_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) >= 0;
}

int Deg::cmp(const Monomial& lhs, const Monomial& rhs) {
    Monomial::Degree_t lhs_deg = lhs.GetTotalDegree(), rhs_deg = rhs.GetTotalDegree();
    return (lhs_deg > rhs_deg) - (lhs_deg < rhs_deg);
}

bool Deg::less::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) < 0;
}

bool Deg::less_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) < 0;
}

bool Deg::greater::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) > 0;
}

bool Deg::greater_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) >= 0;
}

int RevLex::cmp(const Monomial& lhs, const Monomial& rhs) {
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

bool RevLex::less::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) < 0;
}

bool RevLex::less_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) <= 0;
}

bool RevLex::greater::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) > 0;
}

bool RevLex::greater_or_equal::operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp(lhs, rhs) >= 0;
}

} // namespace groebner

#ifndef GROEBNER_BASIS_ALGORITHMS_H
#define GROEBNER_BASIS_ALGORITHMS_H

#include <set>
#include "polynomial_order.h"

namespace groebner {
namespace Algorithm {
static Monomial GetLCM(const Monomial& lhs, const Monomial& rhs) {
    Monomial res = lhs;
    for (const auto& [index, degree]: rhs.GetPowers()) {
        if (degree > res.GetDegree(index)) {
            res *= Monomial({{index, degree - res.GetDegree(index)}});
        }
    }
    return res;
}

template<typename T, typename Comparator>
Polynomial<T, Comparator> GetSPoly(const Polynomial<T, Comparator>& f, const Polynomial<T, Comparator>& g) {
    auto f_lt = f.GetLeadingTerm();
    auto g_lt = g.GetLeadingTerm();
    Monomial lcm = GetLCM(f_lt.first, g_lt.first);
    return f * Polynomial<T, Comparator>({{lcm / f_lt.first, 1 / f_lt.second}}) -
           g * Polynomial<T, Comparator>({{lcm / g_lt.first, 1 / g_lt.second}});
}

template<typename T, typename Comparator>
Polynomial<T, Comparator>
GetReminder(const Polynomial<T, Comparator>& f, const std::set<Polynomial<T, Comparator>, PolynomialOrder>& F) {
    Polynomial<T, Comparator> p = f;
    Polynomial<T, Comparator> r;
    while (p != Polynomial<T, Comparator>()) {
        auto it = F.cbegin();
        bool is_changed = false;
        auto p_lt = p.GetLeadingTerm();
        while (it != F.cend() && !is_changed) {
            auto f_lt = it->GetLeadingTerm();
            if (p_lt.first.IsDivisibleBy(f_lt.first)) {
                p -= (*it) * Polynomial<T, Comparator>({{p_lt.first / f_lt.first, p_lt.second / f_lt.second}});
                is_changed = true;
            } else {
                ++it;
            }
        }
        if (!is_changed) {
            r += Polynomial<T, Comparator>({p_lt});
            p -= Polynomial<T, Comparator>({p_lt});
        }
    }
    return r;
}

template<typename T, typename Comparator>
void ExtendToGroebner(std::set<Polynomial<T, Comparator>, PolynomialOrder>& ideal) {
    std::set<Polynomial<T, Comparator>, PolynomialOrder> old;
    while (old != ideal) {
        old = ideal;
        for (auto it = old.cbegin(); it != old.cend(); ++it) {
            for (auto jt = std::next(it); jt != old.cend(); ++jt) {
                Polynomial<T, Comparator> S = GetSPoly(*it, *jt);
                if (GetReminder(S, old) != Polynomial<T, Comparator>()) {
                    ideal.insert(S);
                }
            }
        }
    }
}

template<typename T, typename Comparator>
void MakeReduced(std::set<Polynomial<T, Comparator>, PolynomialOrder>& F) {
    std::set<Polynomial<T, Comparator>, PolynomialOrder> tmp;
    for (auto f: F) {
        f.Normalize();
        tmp.insert(f);
    }
    F = tmp;
    for (auto f : tmp) {
        F.erase(f);
        if (GetReminder(f, F) != Polynomial<T, Comparator>()) {
            F.insert(f);
        }
    }
}

template<typename T, typename Comparator>
bool IsInIdeal(const Polynomial<T, Comparator>& f, const std::set<Polynomial<T, Comparator>, PolynomialOrder>& F) {
    std::set<Polynomial<T, Comparator>, PolynomialOrder> G = F;
    ExtendToGroebner(G);
//    MakeReduced(G);
    return GetReminder(f, G) == Polynomial<T, Comparator>();
}
} // namespace Algorithm
} // namespace groebner

#endif //GROEBNER_BASIS_ALGORITHMS_H

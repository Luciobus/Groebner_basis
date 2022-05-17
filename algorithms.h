#ifndef GROEBNER_BASIS_ALGORITHMS_H
#define GROEBNER_BASIS_ALGORITHMS_H

#include <set>
#include <unordered_set>
#include <deque>

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
    using Iter = typename std::set<Polynomial<T, Comparator>, PolynomialOrder>::iterator;
    // initialize
    size_t t = ideal.size();
    std::vector<Iter> iterators;
    iterators.reserve(t);
    std::set<std::pair<size_t, size_t>> pairs;
    for (Iter it = ideal.begin(); it != ideal.end(); ++it) {
        iterators.push_back(it);
    }
    for (size_t i = 0; i != t; ++i) {
        for (size_t j = i + 1; j != t; ++j) {
            pairs.emplace(i, j);
        }
    }
    // iterations
    while (!pairs.empty()) {
        auto [i_ind, j_ind] = *pairs.begin();
        Iter it = iterators[i_ind], jt = iterators[j_ind];
        auto [lm_i, lc_i] = it->GetLeadingTerm();
        auto [lm_j, lc_j] = jt->GetLeadingTerm();
        Monomial lcm = GetLCM(lm_i, lm_j);
        bool found_l = false;
        for (size_t l_ind = 0; l_ind != t; ++l_ind) {
            std::pair<size_t, size_t> p1(std::min(l_ind, i_ind), std::max(l_ind, i_ind));
            std::pair<size_t, size_t> p2(std::min(l_ind, j_ind), std::max(l_ind, j_ind));
            if (pairs.contains(p1) && pairs.contains(p2)) {
                auto [lm_l, lc_l] = iterators[l_ind]->GetLeadingTerm();
                if (lm_l.IsDivisibleBy(lcm)) {
                    found_l = true;
                    break;
                }
            }
        }
        if (lcm != lm_i * lm_j && !found_l) {
            Polynomial<T, Comparator> S = GetReminder(GetSPoly(*it, *jt), ideal);
            if (S != Polynomial<T, Comparator>()) {
                auto [iter, _] = ideal.insert(S); // always will be inserted
                iterators.push_back(iter);
                for (size_t i = 0; i != t; ++i) {
                    pairs.emplace(i, t);
                }
                ++t;
            }
        }
        pairs.erase(pairs.begin());
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
    for (auto f: tmp) {
        F.erase(f);
        if (GetReminder(f, F) != Polynomial<T, Comparator>()) {
            F.insert(f);
        }
    }
}

template<typename T, typename Comparator>
bool
IsInGroebnerIdeal(const Polynomial<T, Comparator>& f, const std::set<Polynomial<T, Comparator>, PolynomialOrder>& F) {
    return GetReminder(f, F) == Polynomial<T, Comparator>();
}

template<typename T, typename Comparator>
bool IsInIdeal(const Polynomial<T, Comparator>& f, const std::set<Polynomial<T, Comparator>, PolynomialOrder>& F) {
    std::set<Polynomial<T, Comparator>, PolynomialOrder> G = F;
    ExtendToGroebner(G);
//    MakeReduced(G);
    return IsInGroebnerIdeal(f, G);
}
} // namespace Algorithm
} // namespace groebner

#endif //GROEBNER_BASIS_ALGORITHMS_H

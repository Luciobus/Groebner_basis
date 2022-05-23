#ifndef GROEBNER_BASIS_ALGORITHMS_H
#define GROEBNER_BASIS_ALGORITHMS_H

#include <set>

#include "polynomial_order.h"

namespace groebner {
namespace Algorithm {
static Monomial GetLCM(const Monomial& lhs, const Monomial& rhs) {
    Monomial res = lhs;
    for (auto [index, degree]: rhs.GetPowers()) {
        res.SetDegree(index, std::max(res.GetDegree(index), degree));
    }
    return res;
}

template<typename T, typename Comparator>
Polynomial<T, Comparator> GetSPoly(const Polynomial<T, Comparator>& f, const Polynomial<T, Comparator>& g) {
    using Polynomial = Polynomial<T, Comparator>;
    auto f_lt = f.GetLeadingTerm();
    auto g_lt = g.GetLeadingTerm();
    Monomial lcm = GetLCM(f_lt.first, g_lt.first);
    return f * Polynomial({{lcm / f_lt.first, 1 / f_lt.second}}) -
           g * Polynomial({{lcm / g_lt.first, 1 / g_lt.second}});
}

template<typename T, typename Comparator>
bool ReduceOnce(Polynomial<T, Comparator> *p, const std::set<Polynomial<T, Comparator>, PolynomialOrder>& F) {
    using Polynomial = Polynomial<T, Comparator>;
    auto it = F.cbegin();
    auto p_lt = p->GetLeadingTerm();
    while (it != F.cend()) {
        auto f_lt = it->GetLeadingTerm();
        if (p_lt.first.IsDivisibleBy(f_lt.first)) {
            *p -= (*it) * Polynomial({{p_lt.first / f_lt.first, p_lt.second / f_lt.second}});
            return true;
        } else {
            ++it;
        }
    }
    return false;
}

template<typename T, typename Comparator>
Polynomial<T, Comparator>
GetReminder(const Polynomial<T, Comparator>& f, const std::set<Polynomial<T, Comparator>, PolynomialOrder>& F) {
    using Polynomial = Polynomial<T, Comparator>;
    Polynomial p = f;
    Polynomial r;
    while (!p.IsEmpty()) {
        if (!ReduceOnce(&p, F)) {
            auto p_lt = p.GetLeadingTerm();
            r += Polynomial({p_lt});
            p -= Polynomial({p_lt});
        }
    }
    return r;
}

template<typename T, typename Comparator>
void MakeGroebnerReduced(std::set<Polynomial<T, Comparator>, PolynomialOrder> *F) {
    std::set<Polynomial<T, Comparator>, PolynomialOrder> tmp;
    for (auto f: *F) {
        f.Normalize();
        tmp.insert(f);
    }
    *F = tmp;
    for (auto f: tmp) {
        F->erase(f);
        if (!GetReminder(f, *F).IsEmpty()) {
            F->insert(f);
        }
    }
}

template<typename T, typename Comparator>
std::vector<typename std::set<Polynomial<T, Comparator>, PolynomialOrder>::iterator>
MakeIteratorsFrom(std::set<Polynomial<T, Comparator>, PolynomialOrder>& ideal) {
    using Iter = typename std::set<Polynomial<T, Comparator>, PolynomialOrder>::iterator;
    std::vector<Iter> iterators;
    iterators.reserve(ideal.size());
    for (Iter it = ideal.begin(); it != ideal.end(); ++it) {
        iterators.push_back(it);
    }
    return iterators;
}

static std::set<std::pair<size_t, size_t>> MakeDistinctPairs(size_t n) {
    std::set<std::pair<size_t, size_t>> pairs;
    for (size_t i = 0; i != n; ++i) {
        for (size_t j = i + 1; j != n; ++j) {
            pairs.emplace(i, j);
        }
    }
    return pairs;
}

template<typename T, typename Comparator>
bool IsReducibleToZero(const std::set<std::pair<size_t, size_t>>& pairs, const std::vector<typename std::set<Polynomial<T, Comparator>, PolynomialOrder>::iterator>& iterators) {
    using Iter = typename std::set<Polynomial<T, Comparator>, PolynomialOrder>::iterator;

    auto [i_ind, j_ind] = *pairs.cbegin();
    Iter it = iterators[i_ind], jt = iterators[j_ind];
    auto [lm_i, lc_i] = it->GetLeadingTerm();
    auto [lm_j, lc_j] = jt->GetLeadingTerm();

    Monomial lcm = GetLCM(lm_i, lm_j);
    if (lcm == lm_i * lm_j) {
        return true;
    }
    for (size_t l_ind = 0; l_ind != iterators.size(); ++l_ind) {
        std::pair<size_t, size_t> p1(std::min(l_ind, i_ind), std::max(l_ind, i_ind));
        std::pair<size_t, size_t> p2(std::min(l_ind, j_ind), std::max(l_ind, j_ind));
        if (pairs.contains(p1) && pairs.contains(p2)) {
            auto [lm_l, lc_l] = iterators[l_ind]->GetLeadingTerm();
            if (lm_l.IsDivisibleBy(lcm)) {
                return true;
            }
        }
    }
    return false;
}

template<typename T, typename Comparator>
void ExtendToGroebner(std::set<Polynomial<T, Comparator>, PolynomialOrder> *ideal) {
    using Iter = typename std::set<Polynomial<T, Comparator>, PolynomialOrder>::iterator;
    // initialize
    std::vector<Iter> iterators = MakeIteratorsFrom(*ideal);
    std::set<std::pair<size_t, size_t>> pairs = MakeDistinctPairs(ideal->size());
    size_t t = ideal->size();
    // iterations
    while (!pairs.empty()) {
        auto [i_ind, j_ind] = *pairs.begin();
        Iter it = iterators[i_ind], jt = iterators[j_ind];

        if (!IsReducibleToZero<T, Comparator>(pairs, iterators)) {
            Polynomial<T, Comparator> S = GetReminder(GetSPoly(*it, *jt), *ideal);
            if (!S.IsEmpty()) {
                auto [iter, _] = ideal->insert(S); // always will be inserted
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
bool
IsInGroebnerIdeal(const Polynomial<T, Comparator>& f, const std::set<Polynomial<T, Comparator>, PolynomialOrder>& F) {
    return GetReminder(f, F).IsEmpty();
}

template<typename T, typename Comparator>
bool IsInIdeal(const Polynomial<T, Comparator>& f, const std::set<Polynomial<T, Comparator>, PolynomialOrder>& F) {
    std::set<Polynomial<T, Comparator>, PolynomialOrder> G = F;
    ExtendToGroebner(&G);
//    MakeReduced(G);
    return IsInGroebnerIdeal(f, G);
}
} // namespace Algorithm
} // namespace groebner

#endif //GROEBNER_BASIS_ALGORITHMS_H

#ifndef GROEBNER_BASIS_MONOMIAL_ORDER_H
#define GROEBNER_BASIS_MONOMIAL_ORDER_H

#include "monomial.h"

template<typename T>
class Lex {
public:
    static bool less(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        return cmp(lhs, rhs) < 0;
    }

    static bool less_or_equal(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        return cmp(lhs, rhs) <= 0;
    }

    static bool greater(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        return cmp(lhs, rhs) > 0;
    }

    static bool greater_or_equal(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        return cmp(lhs, rhs) >= 0;
    }

private:
    static int cmp(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        const auto lhs_powers = lhs.GetPowers();
        auto it = lhs_powers.cbegin();
        const auto rhs_powers = rhs.GetPowers();
        auto jt = rhs_powers.cbegin();
        while (it != lhs_powers.cend() && jt != rhs_powers.cend()) {
            if (it->first != jt->first) {
                return (it->first > jt->first) - (it->first < jt->first);
            }
            if (it->second != jt->second) {
                return (it->second > jt->second) - (it->second < jt->second);
            }
            ++it;
            ++jt;
        }
        return (jt == lhs_powers.cend()) - (it == rhs_powers.cend());
    }
};

template<typename T>
class DegLex {
public:
    static bool less(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        int cmp_res = cmp(lhs, rhs);
        return cmp_res < 0 || (cmp_res == 0 && Lex<T>::less(lhs, rhs));
    }

    static bool less_or_equal(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        int cmp_res = cmp(lhs, rhs);
        return cmp_res <= 0 || (cmp_res == 0 && Lex<T>::less_or_equal(lhs, rhs));
    }

    static bool greater(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        int cmp_res = cmp(lhs, rhs);
        return cmp_res > 0 || (cmp_res == 0 && Lex<T>::greater(lhs, rhs));
    }

    static bool greater_or_equal(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        int cmp_res = cmp(lhs, rhs);
        return cmp_res >= 0 || (cmp_res == 0 && Lex<T>::greater_or_equal(lhs, rhs));
    }

private:
    static int cmp(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        uint64_t lhs_sum = 0, rhs_sum = 0;
        for (const auto& [_, degree] : lhs.GetPowers()) {
            lhs_sum += degree;
        }
        for (const auto& [_, degree] : rhs.GetPowers()) {
            rhs_sum += degree;
        }
        return (lhs_sum > rhs_sum) - (lhs_sum < rhs_sum);
    }
};

template<typename T>
class DegRevLex {
public:
    static bool less(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        int cmp_res = cmp(lhs, rhs);
        return cmp_res < 0 || (cmp_res == 0 && Lex<T>::greater(lhs, rhs));
    }

    static bool less_or_equal(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        int cmp_res = cmp(lhs, rhs);
        return cmp_res <= 0 || (cmp_res == 0 && Lex<T>::greater_or_equal(lhs, rhs));
    }

    static bool greater(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        int cmp_res = cmp(lhs, rhs);
        return cmp_res > 0 || (cmp_res == 0 && Lex<T>::less(lhs, rhs));
    }

    static bool greater_or_equal(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        int cmp_res = cmp(lhs, rhs);
        return cmp_res >= 0 || (cmp_res == 0 && Lex<T>::less_or_equal(lhs, rhs));
    }

private:
    static int cmp(const Monomial<T>& lhs, const Monomial<T>& rhs) {
        uint64_t lhs_sum = 0, rhs_sum = 0;
        for (const auto& [_, degree] : lhs.GetPowers()) {
            lhs_sum += degree;
        }
        for (const auto& [_, degree] : rhs.GetPowers()) {
            rhs_sum += degree;
        }
        return (lhs_sum > rhs_sum) - (lhs_sum < rhs_sum);
    }
};

#endif //GROEBNER_BASIS_MONOMIAL_ORDER_H

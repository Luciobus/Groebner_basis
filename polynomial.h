#ifndef GROEBNER_BASIS_POLYNOMIAL_H
#define GROEBNER_BASIS_POLYNOMIAL_H

#include <map>
#include <vector>

#include "monomial.h"
#include "monomial_order.h"

namespace groebner {

template<typename T, typename Comparator = OrderPair<Deg::greater, Lex::greater>>
class Polynomial {
public:
    using Monomials_t = std::map<Monomial, T, Comparator>;
    using iterator = typename Monomials_t::iterator;
    using const_iterator = typename Monomials_t::const_iterator;

    Polynomial() = default;

    Polynomial(std::initializer_list<std::pair<const Monomial, T>> monomials) : monomials_(monomials) {
        RemoveZeros();
    }

    Polynomial& operator+=(const Polynomial& other) {
        for (const auto& [monomial, coefficient]: other.monomials_) {
            monomials_[monomial] += coefficient;
        }
        RemoveZeros();
        return *this;
    }

    friend Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial tmp = lhs;
        tmp += rhs;
        return tmp;
    }

    Polynomial& operator-=(const Polynomial& other) {
        for (const auto& [monomial, coefficient]: other.monomials_) {
            monomials_[monomial] -= coefficient;
        }
        RemoveZeros();
        return *this;
    }

    friend Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial tmp = lhs;
        tmp -= rhs;
        return tmp;
    }

    Polynomial& operator*=(const Polynomial& other) {
        *this = *this * other;
        return *this;
    }

    friend Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial res;
        for (const auto& [lm, lc]: lhs.monomials_) {
            for (const auto& [rm, rc]: rhs.monomials_) {
                res.monomials_[lm * rm] += lc * rc;
            }
        }
        res.RemoveZeros();
        return res;
    }

    friend bool operator==(const Polynomial& lhs, const Polynomial& rhs) {
        return lhs.monomials_ == rhs.monomials_;
    }

    friend bool operator!=(const Polynomial& lhs, const Polynomial& rhs) {
        return !(lhs.monomials_ == rhs.monomials_);
    }

    iterator begin() {
        return monomials_.begin();
    }
    iterator end() {
        return monomials_.end();
    }
    const_iterator begin() const {
        return monomials_.begin();
    }
    const_iterator end() const {
        return monomials_.end();
    }

    [[nodiscard]] bool IsEmpty() const {
        return monomials_.empty();
    }

    void Normalize() {
        if (IsEmpty()) {
            return;
        }
        T lc = monomials_.begin()->second;
        for (auto& [monomial, coefficient] : monomials_) {
            coefficient /= lc;
        }
    }

    const std::pair<const Monomial, T>& GetLeadingTerm() const {
        assert(!IsEmpty());
        return *(monomials_.cbegin());
    }

    [[nodiscard]] std::vector<Monomial> GetMonomials() const {
        std::vector<Monomial> monomials;
        monomials.reserve(monomials_.size());
        for (auto it = monomials_.cbegin(); it != monomials_.cend(); ++it) {
            monomials.push_back(it->first);
        }
        return monomials;
    }

    void Swap(Polynomial& other) {
        std::swap(monomials_, other.monomials_);
    }

    void RemoveZeros() {
        for (auto it = monomials_.begin(); it != monomials_.end();) {
            if (it->second == 0) {
                it = monomials_.erase(it);
            } else {
                ++it;
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const Polynomial& polynomial) {
        if (polynomial.monomials_.empty()) {
            out << T();
            return out;
        }
        for (auto it = polynomial.monomials_.begin(); it != polynomial.monomials_.end(); ++it) {
            if (it != polynomial.monomials_.begin() && it->second > T(0)) {
                out << "+";
            }
            out << it->second << it->first;
        }
        return out;
    }

private:
    Monomials_t monomials_;
};

} // namespace groebner

#endif //GROEBNER_BASIS_POLYNOMIAL_H

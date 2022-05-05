#ifndef GROEBNER_BASIS_MONOMIAL_H
#define GROEBNER_BASIS_MONOMIAL_H

#include <iostream>
#include <map>

template<typename T>
class Monomial {
public:
    using Degree_t = size_t;
    using Index_t = size_t;
    using Powers_t = std::map<Index_t, Degree_t>;

    Monomial() = default;

    Monomial(T coefficient, std::initializer_list<std::pair<Index_t, Degree_t>> powers = {}) : coefficient_(coefficient) {
        for (const auto& [index, degree] : powers) {
            assert(degree >= 0);
            powers_[index] = degree;
        }
        RemoveZeros();
    }

    Monomial(const Monomial& other) : coefficient_(other.coefficient_), powers_(other.powers_) {
    }

    Monomial(Monomial&& other)  noexcept {
        Swap(other);
    }

    ~Monomial() = default;

    void RemoveZeros() {
        if (coefficient_ == T()) {
            powers_.clear();
            return;
        }
        for (auto it = powers_.cbegin(); it != powers_.cend();) {
            if (it->second == 0) {
                it = powers_.erase(it);
            } else {
                ++it;
            }
        }
    }

    Monomial& operator=(const Monomial& other) {
        if (this == &other) {
            return *this;
        }
        Monomial tmp(other);
        Swap(tmp);
        return *this;
    }

    Monomial& operator=(Monomial&& other)  noexcept {
        if (this == &other) {
            return *this;
        }
        Swap(other);
        return *this;
    }

    void Swap(Monomial& other) {
        std::swap(coefficient_, other.coefficient_);
        std::swap(powers_, other.powers_);
    }

    Monomial& operator+=(const Monomial& other) {
        assert(powers_ == other.powers_);
        coefficient_ += other.coefficient_;
        RemoveZeros();
        return *this;
    }

    friend Monomial operator+(const Monomial& lhs, const Monomial& rhs) {
        Monomial res = lhs;
        res += rhs;
        return res;
    }

    Monomial& operator-=(const Monomial& other) {
        assert(powers_ == other.powers_);
        coefficient_ -= other.coefficient_;
        RemoveZeros();
        return *this;
    }

    friend Monomial operator-(const Monomial& lhs, const Monomial& rhs) {
        Monomial res = lhs;
        res -= rhs;
        return res;
    }

    Monomial& operator*=(const Monomial& other) {
        coefficient_ *= other.coefficient_;
        for (const auto& [index, degree] : other.powers_) {
            powers_[index] += degree;
        }
        RemoveZeros();
        return *this;
    }

    friend Monomial operator*(const Monomial& lhs, const Monomial& rhs) {
        Monomial res = lhs;
        res *= rhs;
        return res;
    }

    bool IsDivisibleBy(const Monomial& other) {
        for (const auto& [index, degree] : other.powers_) {
            if (!powers_.contains(index) || powers_[index] < degree) {
                return false;
            }
        }
        return true;
    }

    Monomial& operator/=(const Monomial& other) {
        if (!IsDivisibleBy(other)) {
            std::cerr << "Cannot be divided\n";
            return *this;
        }
        coefficient_ /= other.coefficient_;
        for (const auto& [index, degree] : other.powers_) {
            powers_[index] -= degree;
        }
        RemoveZeros();
        return *this;
    }

    friend Monomial operator/(const Monomial& lhs, const Monomial& rhs) {
        Monomial res = lhs;
        res /= rhs;
        return res;
    }

    friend bool operator==(const Monomial& lhs, const Monomial& rhs) {
        return (lhs.coefficient_ == rhs.coefficient_ && lhs.powers_ == rhs.powers_);
    }

    friend bool operator!=(const Monomial& lhs, const Monomial& rhs) {
        return !(lhs == rhs);
    }

    T GetCoefficient() const {
        return coefficient_;
    }

    Powers_t GetPowers() const {
        return powers_;
    }

    // In future for printing like a^2z^3aaz^32
    // void PrintABC();

    friend std::ostream& operator<<(std::ostream& out, const Monomial& monomial) {
        out << monomial.coefficient_;
        for (const auto& [index, degree] : monomial.powers_) {
            out << "x_" << index << "^" << degree;
        }
        return out;
    }

private:
    T coefficient_ = T();
    Powers_t powers_;
};

#endif //GROEBNER_BASIS_MONOMIAL_H

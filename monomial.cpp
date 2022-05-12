#include "monomial.h"

#include <iostream>
#include <map>

namespace groebner {

Monomial::Monomial() = default;

Monomial::Monomial(std::initializer_list<std::pair<const Index_t, Degree_t>> powers) {
    for (const auto& [index, degree]: powers) {
        assert(degree >= 0);
        powers_[index] = degree;
    }
    RemoveZeros();
}

Monomial::Monomial(const Monomial& other) : powers_(other.powers_) {
}

Monomial::Monomial(Monomial&& other) noexcept: powers_(std::move(other.powers_)) {
}

Monomial::~Monomial() = default;

Monomial& Monomial::operator=(const Monomial& other) {
    if (this == &other) {
        return *this;
    }
    Monomial tmp(other);
    Swap(tmp);
    return *this;
}

Monomial& Monomial::operator=(Monomial&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    Swap(other);
    return *this;
}

Monomial& Monomial::operator*=(const Monomial& other) {
    for (const auto& [index, degree]: other.powers_) {
        powers_[index] += degree;
    }
    RemoveZeros();
    return *this;
}

Monomial operator*(const Monomial& lhs, const Monomial& rhs) {
    Monomial res = lhs;
    res *= rhs;
    return res;
}

Monomial& Monomial::operator/=(const Monomial& other) {
    if (!IsDivisibleBy(other)) {
        std::cerr << "Cannot be divided\n";
        return *this;
    }
    for (const auto& [index, degree]: other.powers_) {
        powers_[index] -= degree;
    }
    RemoveZeros();
    return *this;
}

Monomial operator/(const Monomial& lhs, const Monomial& rhs) {
    Monomial res = lhs;
    res /= rhs;
    return res;
}

bool operator==(const Monomial& lhs, const Monomial& rhs) {
    return lhs.powers_ == rhs.powers_;
}

bool operator!=(const Monomial& lhs, const Monomial& rhs) {
    return lhs.powers_ != rhs.powers_;
}

void Monomial::RemoveZeros() {
    for (auto it = powers_.cbegin(); it != powers_.cend();) {
        if (it->second == 0) {
            it = powers_.erase(it);
        } else {
            ++it;
        }
    }
}

void Monomial::Swap(Monomial& other) {
    std::swap(powers_, other.powers_);
}

bool Monomial::IsDivisibleBy(const Monomial& other) {
    for (const auto& [index, degree]: other.powers_) {
        if (!powers_.contains(index) || powers_[index] < degree) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] const Monomial::Powers_t& Monomial::GetPowers() const {
    return powers_;
}

[[nodiscard]] Monomial::Degree_t Monomial::GetDegree(Index_t index) const {
    return powers_.contains(index) ? powers_.at(index) : Degree_t();
}

std::ostream& operator<<(std::ostream& out, const Monomial& monomial) {
    for (const auto& [index, degree]: monomial.powers_) {
        out << "x_" << index << "^" << degree;
    }
    if (monomial.powers_.empty()) {
        out << "1";
    }
    return out;
}

}   // namespace groebner

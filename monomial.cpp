#include "monomial.h"

namespace groebner {

Monomial::Monomial() = default;

Monomial::Monomial(std::initializer_list<std::pair<const Index_t, Degree_t>> powers) : powers_(powers) {
    RemoveZeros();
}

Monomial& Monomial::operator*=(const Monomial& other) {
    for (auto [index, degree]: other.powers_) {
        powers_[index] += degree;
    }
    return *this;
}

Monomial operator*(const Monomial& lhs, const Monomial& rhs) {
    Monomial res = lhs;
    res *= rhs;
    return res;
}

Monomial& Monomial::operator/=(const Monomial& other) {
    assert(IsDivisibleBy(other) && "Cannot be divided");
    for (auto [index, degree]: other.powers_) {
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
    return !(lhs.powers_ == rhs.powers_);
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
    for (auto [index, degree]: other.powers_) {
        if (GetDegree(index) < degree) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] const Monomial::Powers_t& Monomial::GetPowers() const {
    return powers_;
}

void Monomial::SetDegree(Index_t index, Degree_t degree) {
    powers_[index] = degree;
}

[[nodiscard]] Monomial::Degree_t Monomial::GetDegree(Index_t index) const {
    return powers_.contains(index) ? powers_.at(index) : Degree_t(0);
}

Monomial::Degree_t Monomial::GetTotalDegree() const {
    Degree_t res = 0;
    for (auto [index, degree]: powers_) {
        res += degree;
    }
    return res;
}

std::ostream& operator<<(std::ostream& out, const Monomial& monomial) {
    for (auto [index, degree]: monomial.powers_) {
        out << "x_" << index;
        if (degree != Monomial::Degree_t(1)) {
            out << "^" << degree;
        }
    }
    return out;
}

}   // namespace groebner

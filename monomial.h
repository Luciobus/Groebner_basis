#ifndef GROEBNER_BASIS_MONOMIAL_H
#define GROEBNER_BASIS_MONOMIAL_H

#include <iostream>
#include <map>

namespace groebner {

class Monomial {
public:
    using Degree_t = size_t;
    using Index_t = size_t;
    using Powers_t = std::map<Index_t, Degree_t>;

    Monomial();

    Monomial(std::initializer_list<std::pair<const Index_t, Degree_t>> powers);

//    Monomial(const Powers_t& powers);

    Monomial(const Monomial& other);

    Monomial(Monomial&& other) noexcept;

    ~Monomial();

    Monomial& operator=(const Monomial& other);

    Monomial& operator=(Monomial&& other) noexcept;

    Monomial& operator*=(const Monomial& other);

    friend Monomial operator*(const Monomial& lhs, const Monomial& rhs);

    Monomial& operator/=(const Monomial& other);

    friend Monomial operator/(const Monomial& lhs, const Monomial& rhs);

    friend bool operator==(const Monomial& lhs, const Monomial& rhs);

    friend bool operator!=(const Monomial& lhs, const Monomial& rhs);

    void RemoveZeros();

    void Swap(Monomial& other);

    bool IsDivisibleBy(const Monomial& other);

    [[nodiscard]] const Powers_t& GetPowers() const;

    [[nodiscard]] Degree_t GetDegree(Index_t index) const;

    // In future for printing like a^2z^3aaz^32
    // void PrintABC();

    friend std::ostream& operator<<(std::ostream& out, const Monomial& monomial);

private:
    Powers_t powers_;
};

}   // namespace groebner

#endif //GROEBNER_BASIS_MONOMIAL_H

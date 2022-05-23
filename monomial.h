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

    Monomial& operator*=(const Monomial& other);

    friend Monomial operator*(const Monomial& lhs, const Monomial& rhs);

    Monomial& operator/=(const Monomial& other);

    friend Monomial operator/(const Monomial& lhs, const Monomial& rhs);

    friend bool operator==(const Monomial& lhs, const Monomial& rhs);

    friend bool operator!=(const Monomial& lhs, const Monomial& rhs);

    void Swap(Monomial& other);

    bool IsDivisibleBy(const Monomial& other);

    [[nodiscard]] const Powers_t& GetPowers() const;

    void SetDegree(Index_t index, Degree_t degree);

    [[nodiscard]] Degree_t GetDegree(Index_t index) const;

    [[nodiscard]] Degree_t GetTotalDegree() const;

    friend std::ostream& operator<<(std::ostream& out, const Monomial& monomial);

private:
    Powers_t powers_;

    void RemoveZeros();
};

}   // namespace groebner

#endif //GROEBNER_BASIS_MONOMIAL_H

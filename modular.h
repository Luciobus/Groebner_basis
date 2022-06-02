#ifndef GROEBNER_BASIS_MODULAR_H
#define GROEBNER_BASIS_MODULAR_H

namespace groebner {
// mod must be prime
template<auto mod>
class Modular {
    using Value_t = decltype(mod);
    static_assert(std::is_integral_v<Value_t> && std::is_signed_v<Value_t>);
public:
    Modular() = default;

    Modular(const Value_t& value) : value_(value) {
        Normalize();
    }

    Modular& operator+=(const Modular& other) {
        value_ += other.value_;
        Normalize();
        return *this;
    }

    friend Modular operator+(const Modular& lhs, const Modular& rhs) {
        Modular res = lhs;
        res += rhs;
        return res;
    }

    Modular& operator-=(const Modular& other) {
        value_ -= other.value_;
        Normalize();
        return *this;
    }

    friend Modular operator-(const Modular& lhs, const Modular& rhs) {
        Modular res = lhs;
        res -= rhs;
        return res;
    }

    Modular& operator*=(const Modular& other) {
        value_ *= other.value_;
        Normalize();
        return *this;
    }

    friend Modular operator*(const Modular& lhs, const Modular& rhs) {
        Modular res = lhs;
        res *= rhs;
        return res;
    }

    Modular& operator/=(const Modular& other) {
        *this *= GetInverse(other);
        return *this;
    }

    friend Modular operator/(const Modular& lhs, const Modular& rhs) {
        Modular res = lhs;
        res /= rhs;
        return res;
    }

    friend bool operator==(const Modular& lhs, const Modular& rhs) {
        return lhs.value_ == rhs.value_;
    }

    friend bool operator!=(const Modular& lhs, const Modular& rhs) {
        return !(lhs == rhs);
    }

    static Modular GetInverse(const Modular& other) {
        assert(other != 0);
        return GetPow(other, mod - 2);
    }

    Value_t GetValue() {
        return value_;
    }

    friend std::istream& operator>>(std::istream& in, Modular& other) {
        in >> other.value_;
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Modular& other) {
        out << other.value_;
        return out;
    }

private:
    void Normalize() {
        value_ %= mod;
        if (value_ < 0) {
            value_ += mod;
        }
    }

    static Modular GetPow(const Modular& value, Value_t degree) {
        if (degree == 0) {
            return Modular(1);
        }
        Modular p = GetPow(value, degree / 2);
        p *= p;
        return (degree % 2 == 0) ? p : p * value;
    }

    Value_t value_ = 0;
};
}

#endif //GROEBNER_BASIS_MODULAR_H

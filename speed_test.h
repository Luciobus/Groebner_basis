#ifndef GROEBNER_BASIS_SPEED_TEST_H
#define GROEBNER_BASIS_SPEED_TEST_H

#include <ctime>
#include <string>
#include <iostream>
#include <boost/rational.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include "algorithms.h"

namespace groebner {
template<typename TFunction>
class TimerWrapper {
public:
    TimerWrapper(TFunction function, std::string message) :
            call(function),
            message_(std::move(message)),
            start_time_(clock()) {
    }

    template<class... TArgs>
    auto operator()(TArgs&& ... args) const {
        return call(std::forward<TArgs>(args)...);
    }

    ~TimerWrapper() {
        auto end_time = clock();
        auto diff = (end_time - start_time_);
        long double elapsed_time = (long double) (diff) / CLOCKS_PER_SEC;
        std::cout << message_ << elapsed_time << std::endl;
    }

    TFunction call;

private:
    const clock_t start_time_;
    const std::string message_;
};

template<typename TFunction>
TimerWrapper<TFunction> test_time(TFunction function, const std::string& message) {
    return TimerWrapper<TFunction>(function, message);
}

template<typename T, typename Comparator>
Polynomial<T, Comparator> get_sigma(size_t n, size_t ind) {
    using Polynomial = Polynomial<T, Comparator>;
    if (ind == 0) {
        return Polynomial({{Monomial(), T(1)}});
    }
    if (ind == n) {
        Monomial m;
        for (size_t i = 0; i < n; ++i) {
            m *= Monomial({{i, 1}});
        }
        return Polynomial({{m, T(1)}});
    }
    Polynomial last({{Monomial({{n - 1, 1}}), T(1)}});
    return std::move(get_sigma<T, Comparator>(n - 1, ind) + get_sigma<T, Comparator>(n - 1, ind - 1) * last);
}

template<typename T, typename Comparator>
std::set<Polynomial<T, Comparator>, PolynomialOrder> get_cyclic(size_t n) {
    using Polynomial = Polynomial<T, Comparator>;
    std::set<Polynomial, PolynomialOrder> res;
    for (size_t sigma = 1; sigma < n; ++sigma) {
        res.insert(get_sigma<T, Comparator>(n, sigma));
    }
    res.insert(get_sigma<T, Comparator>(n, n) - Polynomial({{Monomial(), T(1)}}));
    return res;
}

template<typename T>
void test_cyclic(size_t n) {
    using LexSet = std::set<Polynomial<T, Lex::greater>, PolynomialOrder>;
    using DegLexSet = std::set<Polynomial<T, OrderPair<Deg::greater, Lex::greater>>, PolynomialOrder>;
    std::cout << "Testing cyclic " << n << "...\n";
    LexSet cyclic = get_cyclic<T, Lex::greater>(n);
    for (const auto& f: cyclic) {
        std::cout << f << "\n";
    }
    {
        LexSet cyclic0 = get_cyclic<T, Lex::greater>(n);
        for (size_t attempt = 1; attempt <= 10; ++attempt) {
            std::string message = "Lex order: attempt #" + std::to_string(attempt) + ": ";
            test_time(Algorithm::ExtendToGroebner<T, Lex::greater>, message)(&cyclic0);
        }
        for (const auto& f: cyclic0) {
            std::cout << f << "\n";
        }
    }
    {
        DegLexSet cyclic1 = get_cyclic<T, OrderPair<Deg::greater, Lex::greater>>(n);
        for (size_t attempt = 1; attempt <= 10; ++attempt) {
            std::string message = "DegLex order: attempt #" + std::to_string(attempt) + ": ";
            test_time(Algorithm::ExtendToGroebner<T, OrderPair<Deg::greater, Lex::greater>>, message)(&cyclic1);
        }
        for (const auto& f: cyclic1) {
            std::cout << f << "\n";
        }
    }
    std::cout << "\n";
}

void test_speed() {
    using Value_t = boost::rational<boost::multiprecision::cpp_int>;
    std::cout << "\n";
    for (size_t i = 2; i < 6; ++i) {
        test_cyclic<Value_t>(i);
    }
}
} // namespace groebner
#endif //GROEBNER_BASIS_SPEED_TEST_H

#pragma once

#include <iostream>
#include <boost/rational.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include "modular.h"
#include "monomial.h"
#include "monomial_order.h"
#include "polynomial.h"
#include "polynomial_order.h"
#include "algorithms.h"
#include "tests.h"
#include "speed_test.h"

using namespace groebner;

int main() {
    groebner::test_all();
    groebner::test_speed();
    return 0;
}

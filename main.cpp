#pragma once

#include <iostream>
#include <boost/rational.hpp>

#include "monomial.h"
#include "monomial_order.h"
#include "polynomial.h"
#include "polynomial_order.h"
#include "algorithms.h"
#include "tests.h"

int main() {
    groebner::test_all();
    return 0;
}

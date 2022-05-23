#pragma once

#include "tests.h"
#include "speed_test.h"

using namespace groebner;

int main() {
    groebner::test_all();
    groebner::test_speed();
    return 0;
}

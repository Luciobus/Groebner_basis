cmake_minimum_required(VERSION 3.22)
project(Groebner_basis)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

find_package(Boost 1.76 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})


add_executable(Groebner_basis
        main.cpp monomial.cpp monomial_order.cpp tests.cpp)
add_executable(tmp
        tmp.cpp monomial.cpp monomial_order.cpp tests.cpp)

target_link_libraries(Groebner_basis LINK_PUBLIC ${Boost_LIBRARIES})

cmake_minimum_required(VERSION 3.29)

project(capture-this-examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Treat warnings as errors to make standard differences visible
add_compile_options(-Wall -Wextra -Wpedantic)

# ─── Third-party: Boost.UT via FetchContent ──────────────────────────
include(FetchContent)
FetchContent_Declare(ut
    GIT_REPOSITORY https://github.com/boost-ext/ut.git
    GIT_TAG        v2.3.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(ut)

# ─── Old examples (kept for reference) ───────────────────────────────
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

add_executable(01_this_pointer random_stuff/01_this_pointer.cpp)
add_executable(02_star_this random_stuff/02_star_this.cpp)
add_executable(03_init_capture random_stuff/03_init_capture.cpp)
add_executable(04_implicit_this random_stuff/04_implicit_this.cpp)
add_executable(05_deprecated_eq random_stuff/05_deprecated_eq.cpp)
add_executable(06_dangling random_stuff/06_dangling.cpp)

# ─── UT-based tests ──────────────────────────────────────────────────
enable_testing()

# Helper: add a UT test compiled under a specific C++ standard
function(add_ut_test NAME SOURCE STD)
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} PRIVATE Boost::ut)
    set_target_properties(${NAME} PROPERTIES
        CXX_STANDARD ${STD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
    add_test(NAME ${NAME} COMMAND ${NAME})
endfunction()

# Helper: compile-failure test (ICM-style: code must NOT compile)
function(add_compile_fail_test NAME SOURCE STD)
    add_executable(${NAME} EXCLUDE_FROM_ALL ${SOURCE})
    set_target_properties(${NAME} PROPERTIES
        CXX_STANDARD ${STD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
    target_compile_options(${NAME} PRIVATE -pedantic-errors)
    add_test(NAME ${NAME}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
                --target ${NAME} --config $<CONFIG>)
    set_tests_properties(${NAME} PROPERTIES WILL_FAIL TRUE)
endfunction()

# ── [this] pointer capture (C++11) ───────────────────────────────────
add_ut_test(test.this_pointer.cxx11     capture/test_this_pointer_cxx11.cpp     11)

# ── [=] and [&] implicit this (C++11) ────────────────────────────────
add_ut_test(test.implicit_capture.cxx11 capture/test_implicit_capture_cxx11.cpp 11)

# ── [self=*this] init capture (C++14+) ───────────────────────────────
add_ut_test(test.init_capture.cxx14     capture/test_init_capture_cxx14.cpp     14)
add_compile_fail_test(fail.init_capture.cxx11 capture/fail_init_capture_cxx11.cpp 11)

# ── [*this] copy capture (C++17+) ────────────────────────────────────
add_ut_test(test.star_this.cxx17        capture/test_star_this_cxx17.cpp        17)
add_compile_fail_test(fail.star_this.cxx11 capture/fail_star_this_cxx11.cpp     11)

# ── [*this] mutable (C++17+) ─────────────────────────────────────────
add_ut_test(test.star_this_mutable.cxx17 capture/test_star_this_mutable_cxx17.cpp 17)

# ── [=, this] / [=, *this] explicit (C++17+) ────────────────────────
add_ut_test(test.explicit_this.cxx17    capture/test_explicit_this_cxx17.cpp    17)
add_compile_fail_test(fail.explicit_this.cxx14 capture/fail_explicit_this_cxx14.cpp 14)

# ── [] empty capture — member not accessible ─────────────────────────
add_compile_fail_test(fail.empty_capture.cxx20 capture/fail_empty_capture_cxx20.cpp 20)

# ── [i] — data member is not a variable ──────────────────────────────
add_compile_fail_test(fail.member_capture.cxx20 capture/fail_member_capture_cxx20.cpp 20)

# ── [&i = i] init-capture ref to member (C++14+) ────────────────────
add_ut_test(test.member_ref.cxx14       capture/test_member_ref_cxx14.cpp       14)

# ── Deducing this / recursive lambda (C++23) ─────────────────────────
add_ut_test(test.deducing_this.cxx23    capture/test_deducing_this_cxx23.cpp    23)

# ── Dangling this (C++11) ────────────────────────────────────────────
add_ut_test(test.dangling.cxx11         capture/test_dangling_cxx11.cpp         11)

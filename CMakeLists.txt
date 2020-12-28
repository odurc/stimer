cmake_minimum_required (VERSION 3.14)

# project definitions
project(stimer C)
set(LIBRARY_VERSION "1.3.0")
set(CMAKE_C_STANDARD 99)

# add local cmake modules to be loaded by include()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# add options
option(ENABLE_TESTS     "Build and run tests" OFF)
option(ENABLE_LINTER    "Enable linter - static code check" OFF)
option(ENABLE_COVERAGE  "Enable coverage" OFF)

# define default build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

# add default compile options
add_compile_options(
    -Wall                          # enable all warnings
    -ffunction-sections            # create separate section for functions
    -fdata-sections                # create separate section for data
)

# ffunction-sections and fdata-sections allow later the linker to remove all unreferenced
# functions and data, see: https://gcc.gnu.org/onlinedocs/gnat_ugn/Compilation-options.html

# add compile options for debug
add_compile_options(
    $<$<CONFIG:DEBUG>:-O0>          # no optimization
    $<$<CONFIG:DEBUG>:-DDEBUG>      # define DEBUG macro
)

# add compile options for release
add_compile_options(
    $<$<CONFIG:RELEASE>:-O3>        # enable high-level of optimization
)

# add default link options
add_link_options(
    -Wl,--gc-sections               # remove unreferenced sections
)

# add link options for release
add_link_options(
    $<$<CONFIG:RELEASE>:-Wl,-s>     # strip all symbols
)

# add coverage flag for compiling and linking
# see: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
if(ENABLE_COVERAGE)
    message(STATUS "Coverage enabled")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# source code
file(GLOB SRC "src/*.c")

# build static library
set(STIMER_LIBRARY_NAME ${CMAKE_PROJECT_NAME})
add_library(${STIMER_LIBRARY_NAME} STATIC ${SRC})

include(CTest)

# configure valgrind options (overwrite default)
set(VALGRIND_COMMAND_OPTIONS "--leak-check=full --show-leak-kinds=all --error-exitcode=1")

# configure linter
if(ENABLE_LINTER)
    find_program(CLANG_TIDY clang-tidy)
    if (CLANG_TIDY)
        message(STATUS "Clang-Tidy enabled")
        set(CMAKE_C_CLANG_TIDY clang-tidy;)
    endif()

    find_program(CPPCHECK cppcheck)
    if (CPPCHECK)
        message(STATUS "Cppcheck enabled")
        set(CMAKE_C_CPPCHECK cppcheck;)
    endif()
endif()

# unit testing
if (BUILD_TESTING AND ENABLE_TESTS)
    message(STATUS "Tests enabled")
    find_package(CMocka REQUIRED)
    include(AddCMockaTest)
    include(AddMockedTest)
    add_subdirectory(test)
endif()

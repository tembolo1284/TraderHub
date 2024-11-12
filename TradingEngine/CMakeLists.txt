cmake_minimum_required(VERSION 3.15)
project(TradingEngine 
        VERSION 1.0.0 
        DESCRIPTION "A C++ Trading Engine with Price-Time Priority Matching"
        LANGUAGES CXX)

# Specify C++ standard and compile commands export
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable testing
include(CTest)
enable_testing()

# Include FetchContent for downloading dependencies
include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Function to set compiler flags based on the compiler being used
function(set_strict_compiler_flags target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4                     # Warning level 4
            /WX                     # Treat warnings as errors
            /permissive-           # Strict standards compliance
            /w14242                # Conversion possible loss of data
            /w14254                # Larger bit field type conversion
            /w14263                # Member function does not override
            /w14265                # Class has virtual functions, but destructor is not virtual
            /w14287                # Unsigned/negative constant mismatch
            /we4289                # Loop control variable declared in the for-loop is used outside
            /w14296                # Expression is always false/true
            /w14311                # Pointer truncation
            /w14545                # Expression before comma evaluates to a function
            /w14546                # Function call before comma missing argument list
            /w14547                # Operator before comma has no effect
            /w14549                # Operator before comma has no effect
            /w14555                # Expression has no effect
            /w14619                # Pragma warning
            /w14640                # Thread unsafe static member initialization
            /w14826                # Conversion from 'type1' to 'type2' is sign-extended
            /w14905                # Wide string literal cast to 'LPSTR'
            /w14906                # String literal cast to 'LPWSTR'
            /w14928                # Illegal copy-initialization
            /analyze               # Code analysis
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall                    # Enable all warnings
            -Wextra                  # Enable extra warnings
            -Werror                  # Treat warnings as errors
            -pedantic               # Strict ISO C/C++
            -pedantic-errors        # Treat pedantic warnings as errors
            -Wold-style-cast        # Warn about old-style C casts
            -Wcast-align            # Warn about pointer casts which increase alignment
            -Wcast-qual             # Warn about casts which remove qualifiers
            -Wconversion            # Warn about type conversions that may lose data
            -Wformat=2              # Warn about printf/scanf format string issues
            -Wduplicated-branches   # Warn about identical if-else branches
            -Wduplicated-cond       # Warn about duplicated conditions in if-else chains
            -Wlogical-op            # Warn about suspicious uses of logical operators
            -Wmissing-declarations  # Warn about missing declarations
            -Wmissing-include-dirs  # Warn about missing include directories
            -Wnon-virtual-dtor      # Warn about non-virtual destructors
            -Woverloaded-virtual    # Warn about overloaded virtual functions
            -Wpointer-arith         # Warn about pointer arithmetic
            -Wredundant-decls      # Warn about redundant declarations
            -Wshadow               # Warn about shadow variables
            -Wsign-conversion      # Warn about sign conversions
            -Wstrict-null-sentinel # Warn about incorrect uses of null sentinel
            -Wstrict-overflow=5    # Warn about all strict overflow cases
            -Wswitch-default       # Warn when a switch doesn't have a default case
            -Wswitch-enum          # Warn when a switch on an enum doesn't handle all cases
            -Wundef                # Warn if an undefined identifier is evaluated
            -Wunreachable-code     # Warn about unreachable code
            -Wuseless-cast         # Warn about useless casts
            -Wwrite-strings        # Warn when string literals are written to 'char*'
            -Wdisabled-optimization # Warn when an optimization pass is disabled
            -Weffc++               # Warn about violations of Effective C++ guidelines
            -Wmissing-noreturn     # Warn about functions that might be candidates for [[noreturn]]
            -Wstack-protector      # Warn about functions that might benefit from stack protector
            -Wstrict-aliasing=3    # Warn about strict aliasing rules
            -Wvla                  # Warn about variable length arrays
            -fstack-protector-strong # Enable stack protector
            $<$<CONFIG:Debug>:-fsanitize=address> # Enable AddressSanitizer in Debug builds
            $<$<CONFIG:Debug>:-fsanitize=undefined> # Enable UndefinedBehaviorSanitizer in Debug builds
        )

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_link_options(${target} PRIVATE
                -fsanitize=address
                -fsanitize=undefined
            )
        endif()
    endif()
endfunction()

# Define source files
set(SOURCES
    src/order.cpp
    src/order_book.cpp
    src/trade.cpp
    src/trader.cpp
)

# Define header files
set(HEADERS
    include/order.hpp
    include/order_book.hpp
    include/prioritizable_value_st.hpp
    include/trade.hpp
    include/trader.hpp
)

# Create main library
add_library(trading_engine ${SOURCES} ${HEADERS})
target_include_directories(trading_engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set_strict_compiler_flags(trading_engine)

# Set up testing
if(BUILD_TESTING)
    # Create test executables
    add_executable(unit_tests
        tests/unit/order_tests.cpp
        tests/unit/order_book_tests.cpp
        tests/unit/trade_tests.cpp
        tests/unit/trader_tests.cpp
    )

    add_executable(integration_tests
        tests/integration/integration_tests.cpp
    )

    add_executable(performance_tests
        tests/performance/performance_tests.cpp
    )

    # Apply strict compiler flags to test targets
    set_strict_compiler_flags(unit_tests)
    set_strict_compiler_flags(integration_tests)
    set_strict_compiler_flags(performance_tests)

    # Link libraries
    target_link_libraries(unit_tests PRIVATE
        trading_engine
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
    )

    target_link_libraries(integration_tests PRIVATE
        trading_engine
        GTest::gtest
        GTest::gtest_main
    )

    target_link_libraries(performance_tests PRIVATE
        trading_engine
        GTest::gtest
        GTest::gtest_main
    )

    # Enable test discovery
    include(GoogleTest)
    gtest_discover_tests(unit_tests)
    gtest_discover_tests(integration_tests)
    gtest_discover_tests(performance_tests)

    # Add custom targets for test categories
    add_custom_target(run_unit_tests
        COMMAND unit_tests
        DEPENDS unit_tests
    )

    add_custom_target(run_integration_tests
        COMMAND integration_tests
        DEPENDS integration_tests
    )

    add_custom_target(run_performance_tests
        COMMAND performance_tests
        DEPENDS performance_tests
    )

    # Add custom target for all tests
    add_custom_target(run_all_tests
        COMMAND ${CMAKE_COMMAND} -E echo "Running all tests..."
        COMMAND unit_tests
        COMMAND integration_tests
        COMMAND performance_tests
        DEPENDS unit_tests integration_tests performance_tests
    )

    # Create test results directory
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test-results)

    # Set test output format
    set_tests_properties(${ALL_TESTS} PROPERTIES
        ENVIRONMENT "GTEST_OUTPUT=xml:${CMAKE_BINARY_DIR}/test-results/"
    )
endif()

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
            # ... (other MSVC compiler flags)
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall                    # Enable all warnings
            -Wextra                  # Enable extra warnings
            -Werror                  # Treat warnings as errors
            -pedantic               # Strict ISO C/C++
            # ... (other GCC/Clang compiler flags)
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
add_library(TradingEngineLib ${SOURCES} ${HEADERS})
target_include_directories(TradingEngineLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set_strict_compiler_flags(TradingEngineLib)

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
        TradingEngineLib
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
    )

    target_link_libraries(integration_tests PRIVATE
        TradingEngineLib
        GTest::gtest
        GTest::gtest_main
    )

    target_link_libraries(performance_tests PRIVATE
        TradingEngineLib
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

# Install the library
install(TARGETS TradingEngineLib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

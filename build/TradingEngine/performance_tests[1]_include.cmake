if(EXISTS "/home/tembolo381/Desktop/cpp-workspace/TraderHub/build/TradingEngine/performance_tests[1]_tests.cmake")
  include("/home/tembolo381/Desktop/cpp-workspace/TraderHub/build/TradingEngine/performance_tests[1]_tests.cmake")
else()
  add_test(performance_tests_NOT_BUILT performance_tests_NOT_BUILT)
endif()
if(EXISTS "/home/tembolo381/Desktop/cpp-workspace/TraderHub/build/TradingEngine/unit_tests[1]_tests.cmake")
  include("/home/tembolo381/Desktop/cpp-workspace/TraderHub/build/TradingEngine/unit_tests[1]_tests.cmake")
else()
  add_test(unit_tests_NOT_BUILT unit_tests_NOT_BUILT)
endif()

if(EXISTS "/home/tembolo381/Desktop/cpp-workspace/TraderHub/TradingEngine/build/integration_tests[1]_tests.cmake")
  include("/home/tembolo381/Desktop/cpp-workspace/TraderHub/TradingEngine/build/integration_tests[1]_tests.cmake")
else()
  add_test(integration_tests_NOT_BUILT integration_tests_NOT_BUILT)
endif()

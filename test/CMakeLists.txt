cmake_minimum_required(VERSION 3.8)
project(monvif_tests)

find_package(GTest MODULE)
if(NOT GTest_FOUND)
  message(WARNING "\nGoogle Test library not found, try to get and build it\n")
  include(External_GTest)
endif()

include(GoogleTest)

if(NOT TARGET TortureLabs::monvif)
  find_package(monvif CONFIG REQUIRED)
endif()

add_executable(${PROJECT_NAME} internals.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE TortureLabs::monvif GTest::GMock
                                              gmock_main)

gtest_discover_tests(${PROJECT_NAME} TEST_PREFIX gtest:)

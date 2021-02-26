# this is simple project wrapper useful for project development

# * no need to install the library in order to run unit tests
# * entry point for IDEs like CLion or Visual Studio
cmake_minimum_required(VERSION 3.8)

set(VERSION 0.1)

project(monvif VERSION ${VERSION})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_TESTING "Builds tests included in project" ON)

# add project code
add_subdirectory(src)

if(BUILD_TESTING)
  # add unit tests
  enable_testing()
  add_subdirectory(test)
endif()

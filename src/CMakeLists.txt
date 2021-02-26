cmake_minimum_required(VERSION 3.8)

project(
  monvif
  VERSION ${VERSION}
  LANGUAGES C)

set(HEADERS example.h)

set(SOURCES example.c)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PUBLIC c_std_99)
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include>)
add_library(TortureLabs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

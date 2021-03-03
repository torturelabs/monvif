cmake_minimum_required(VERSION 3.8)

project(
  monvif
  VERSION ${VERSION}
  LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")

find_package(mxml QUIET)
if(NOT mxml_FOUND)
  message(WARNING "\nMiniXML library not found, try to get and build it\n")
  include(External_MiniXML)
endif()

set(HEADERS sha1.h soap.h tools.h xml.h)

set(SOURCES sha1.c soap.c tools.c xml.c)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PUBLIC c_std_99)

if(NOT CMAKE_BUILD_TYPE)
  message(
    STATUS "No build type specified; defaulting to CMAKE_BUILD_TYPE=Release.")
  set(CMAKE_BUILD_TYPE
      Release
      CACHE
        STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
else(NOT CMAKE_BUILD_TYPE)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(
      "=========================================================================================="
    )
    message(STATUS "Build type: Debug. Performance will be terrible!")
    message(
      STATUS
        "Add -DCMAKE_BUILD_TYPE=Release to the CMake command line to get an optimized build."
    )
    message(
      "=========================================================================================="
    )
  endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif(NOT CMAKE_BUILD_TYPE)

option(USE_ASAN "Set to ON to build with ASan" OFF)

if(USE_ASAN)
  # enable ASan
  set(ENV{ASAN_OPTIONS}
      "strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:symbolize=1"
  )
  include(CheckCCompilerFlag)
  CHECK_C_COMPILER_FLAG(-fsanitize-address-use-after-scope HasUseAfterScope)
  set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
  if(HasUseAfterScope)
    set(ASAN_FLAGS "${ASAN_FLAGS} -fsanitize-address-use-after-scope")
  endif()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_FLAGS}")
endif()

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include>)
add_library(TortureLabs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC MSweet::mxml)

# install library
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES
  DESTINATION include)

install(
  EXPORT ${PROJECT_NAME}Targets
  DESTINATION lib/cmake/${PROJECT_NAME}
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE TortureLabs::)

install(FILES include/monvif.h DESTINATION include)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake
                                 COMPATIBILITY SameMajorVersion)
install(FILES ../cmake/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION lib/cmake/${PROJECT_NAME})

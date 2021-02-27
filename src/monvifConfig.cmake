include(CMakeFindDependencyMacro)
find_dependency(Foo 1.0)
include("${CMAKE_CURRENT_LIST_DIR}/monvifTargets.cmake")

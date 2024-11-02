cmake_minimum_required(VERSION 3.16)
project(kindc
        DESCRIPTION "A kind web framework for C."
        LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
  set(default_build_type "Debug")
  message(STATUS "Set the build type to `${default_build_type}` as none was specified.")
  set(CMAKE_BUILD_TYPE ${default_build_type} CACHE STRING "Chooce the build type." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
  "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
message(STATUS "kindc Build Type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the version for kindc
set(kindc_Version_Major 0)
set(kindc_Version_Minor 1)
set(kindc_Version_Patch 0)
set(kindc_Version_Status "-dev")
set(PROJECT_VERSION
  "${kindc_Version_Major}.${kindc_Version_Minor}.${kindc_Version_Patch}${kindc_Version_Status}"
)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

# Setup library
set(SOURCES src/kind.c)

add_library(kindc STATIC ${SOURCES})

# Setup library version
set_target_properties(kindc PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(kindc PROPERTIES SOVERSION ${kindc_Version_Major})
set_target_properties(kindc PROPERTIES PUBLIC_HEADER include/kind.h)

# Setup private projct dirs
target_include_directories(kindc PRIVATE include)
target_include_directories(kindc PRIVATE src)

# Setup install rule
include(GNUInstallDirs)
install(TARGETS kindc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Setup testing
add_executable(kindc_test test/test.c)
target_link_libraries(kindc_test kindc)
target_include_directories(kindc_test PRIVATE include)
add_test(NAME kindc_test
         COMMAND kindc_test)

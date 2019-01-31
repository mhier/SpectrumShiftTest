cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

PROJECT(SpectrumShiftTest)

set(${PROJECT_NAME}_MAJOR_VERSION 01)
set(${PROJECT_NAME}_MINOR_VERSION 00)
set(${PROJECT_NAME}_PATCH_VERSION 00)

## Set additional version-specific variables
include(${CMAKE_SOURCE_DIR}/cmake/set_version_numbers.cmake)
## Load custom modules, i.e. FindDOOCS.cmake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

## Set the build type to Release if none is specified
## Force it into Release if "None" is specified (needed to overrule dkpg_buildpackage)
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "None")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "None")

include(cmake/add_dependency.cmake)
add_dependency(ChimeraTK-DeviceAccess 01.00 REQUIRED)
add_dependency(DOOCS REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 --coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

include_directories(${CMAKE_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_SOURCE_DIR}/src app_sources)
add_executable(${PROJECT_NAME} ${app_sources})
target_link_libraries(${PROJECT_NAME} ${ChimeraTK-DeviceAccess_LIBRARIES} ${DOOCS_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${ChimeraTK-DeviceAccess_LINK_FLAGS} ${DOOCS_LINK_FLAGS}")

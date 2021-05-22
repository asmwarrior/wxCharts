set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/../../vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})

cmake_minimum_required(VERSION 3.12)

project(wxcharts)

# options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE) 
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x64-windows-cpp17" CACHE STRING "Set vcpkg triplet" FORCE)

# wxwidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

if(WIN32 AND BUILD_SHARED_LIBS)    
    add_compile_definitions(WXCHARTS_BUILD_DLL)
endif()

# include
include_directories(./include/wx/charts)

# extract the common parts in a (internal) static library
file(GLOB LIBRARY_SRCS src/*.cpp)
add_library(wxcharts ${LIBRARY_SRCS})
target_include_directories(wxcharts PUBLIC include )
target_link_libraries(wxcharts ${wxWidgets_LIBRARIES})

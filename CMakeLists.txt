cmake_minimum_required(VERSION 3.12)

project(wxcharts)

# options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

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

# install
install(TARGETS ${CMAKE_PROJECT_NAME})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/wx" # source directory
        DESTINATION "include") # target directory

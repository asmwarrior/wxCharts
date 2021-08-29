cmake_minimum_required(VERSION 3.20)

project(wxcharts LANGUAGES CXX)

################
# config cmake #
################

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

#############
#dependency #
#############

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

##########
# target #
##########

file(GLOB SOURCE_FILES src/*.cpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

# export macro
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           PUBLIC # use PUBLIC instead of PRIVATE because tests use it
                               $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                               $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/wx/charts>
                               $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
                           INTERFACE
                               $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>
                               $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}/include/wx/charts>)

target_link_libraries(${PROJECT_NAME} PUBLIC ${wxWidgets_LIBRARIES})

# ##############################################################################
# install #
# ##############################################################################

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        EXPORT export-${PROJECT_NAME})

install(EXPORT export-${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        NAMESPACE wx::
        FILE ${PROJECT_NAME}Config.cmake)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" # source directory
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}) # target directory

install(FILES <PROJECT_BINARY_DIR>/wxcharts_export.h
        DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/include/wx/charts)

cmake_minimum_required(VERSION 3.1)
project(springs_assembler_sfml)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(box2d)
#add_subdirectory(earcut.hpp)
find_package(Eigen3 REQUIRED)

# Add the 'include' directory to the list of directories searched for header files
include_directories(include)

# Gather source files
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

# Create the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

set(SFML_DIR "C:/src/vcpkg/installed/x64-windows/share/sfml")

find_library(SFML_GRAPHICS_LIBRARY NAMES sfml-graphics PATHS "C:/src/vcpkg/installed/x64-windows/lib")
find_library(SFML_WINDOW_LIBRARY NAMES sfml-window PATHS "C:/src/vcpkg/installed/x64-windows/lib")
find_library(SFML_NETWORK_LIBRARY NAMES sfml-network PATHS "C:/src/vcpkg/installed/x64-windows/lib")
find_library(SFML_SYSTEM_LIBRARY NAMES sfml-system PATHS "C:/src/vcpkg/installed/x64-windows/lib")
find_library(SFML_AUDIO_LIBRARY NAMES sfml-audio PATHS "C:/src/vcpkg/installed/x64-windows/lib")

# Set the path to the Boost directory
set(BOOST_ROOT "C:/Program Files/boost/boost_1_82_0")

# Find the Boost libraries
find_package(Boost 1.82.0 REQUIRED)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost library not found")
endif()

#set(HTTPLIB_ROOT "${CMAKE_SOURCE_DIR}/cpp-httplib")
#include_directories(${HTTPLIB_ROOT})
#add_library(httplib INTERFACE)
#target_include_directories(httplib INTERFACE ${HTTPLIB_ROOT})


target_link_libraries(${PROJECT_NAME} PRIVATE
    box2d 
    Eigen3::Eigen
    ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_NETWORK_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${SFML_AUDIO_LIBRARY}
    ${Boost_LIBRARIES} 
) 

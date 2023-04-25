cmake_minimum_required(VERSION 3.1)
project(springs_assembler)
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

# Link libraries to executable
target_include_directories(${PROJECT_NAME} PUBLIC "C:\\opencv-4.7.0\\opencv\\build\\include") 
target_link_directories(${PROJECT_NAME} PUBLIC "C:\\opencv-4.7.0\\opencv\\build\\x64\\vc16\\lib") #this is the file name in the lib directory that was mentioned in the tutorial

target_link_libraries(${PROJECT_NAME} PRIVATE
    opencv_world470d 
    box2d 
    Eigen3::Eigen) 

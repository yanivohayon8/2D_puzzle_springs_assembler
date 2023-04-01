cmake_minimum_required(VERSION 3.1)
project(springs_assembler)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(box2d)
find_package(Eigen3 REQUIRED)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp" )
add_executable(${PROJECT_NAME} ${SOURCES} )



# worked importing opencv
target_include_directories(${PROJECT_NAME} PUBLIC "C:\\opencv-4.7.0\\opencv\\build\\include") # path to include directory
target_link_directories(${PROJECT_NAME} PUBLIC "C:\\opencv-4.7.0\\opencv\\build\\x64\\vc16\\lib") # path to lib directory




target_link_libraries(${PROJECT_NAME} PRIVATE
									opencv_world470d 
									box2d 
									Eigen3::Eigen) #this is the file name in the lib directory that was mentioned in the tutorial


#link_directories("box2d/")
#target_link_libraries(${PROJECT_NAME} box2d)

#find_library(box2d_LIB box2d)
#target_link_libraries(${PROJECT_NAME} "${box2d_LIB}")


#add_library(box2d "box2d/src")
#include(box2d)
#add_subdirectory(box2d)
#target_link_libraries(box2d PUBLIC box2d)


#add_subdirectory(box2d)
#add_subdirectory(eigen-3.4.0)
#include(box2d)


# Add your project files
#file(GLOB SRCFILES *.cpp)
#add_executable(${PROJECT_NAME} ${SRCFILES})
#target_link_libraries(${PROJECT_NAME} PUBLIC box2d)
#target_link_libraries(${PROJECT_NAME} PUBLIC eigen)
##target_include_directories(box2d PUBLIC)
##target_link_libraries(box2d)



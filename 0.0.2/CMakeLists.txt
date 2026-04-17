cmake_minimum_required(VERSION 3.10)
project(Wolfenstein3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/msys64/ucrt64/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Wolfenstein3D ${SOURCES})
target_include_directories(Wolfenstein3D PRIVATE src)
target_link_libraries(Wolfenstein3D sfml-graphics sfml-window sfml-system sfml-audio)

if(WIN32)
    set_target_properties(Wolfenstein3D PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
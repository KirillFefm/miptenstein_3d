cmake_minimum_required(VERSION 3.10)
project(Miptenstein3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Используем установленный SFML 2.6 вместо сабмодуля
set(SFML_DIR "C:/msys64/ucrt64/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

file(GLOB_RECURSE GAME_SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${GAME_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
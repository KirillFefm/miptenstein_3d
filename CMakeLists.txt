cmake_minimum_required(VERSION 3.10)
project(Miptenstein3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32 AND EXISTS "C:/msys64/ucrt64")
    list(APPEND CMAKE_PREFIX_PATH "C:/msys64/ucrt64")
endif()

find_package(SFML COMPONENTS Graphics Window System Audio QUIET)

if(NOT SFML_FOUND)
    find_package(SFML COMPONENTS graphics window system audio REQUIRED)
endif()

file(GLOB_RECURSE GAME_SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${GAME_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

if(TARGET SFML::Graphics)
    target_link_libraries(${PROJECT_NAME} SFML::Graphics SFML::Window SFML::System SFML::Audio)
else()
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio)
endif()

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        WIN32_EXECUTABLE "$<CONFIG:Release>"
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/arial.TTF"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/arial.TTF"
    COMMENT "Copying arial.TTF to build directory..."
)

# 2. Настраиваем рабочую директорию для Visual Studio (чтобы F5 работал без проблем)
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
cmake_minimum_required(VERSION 3.23)
project(mini_engine)

set(CMAKE_CXX_STANDARD 17)
set(REQUIRED_LIBS SDL2 SDL2_image SDL2_mixer SDL2_ttf lua)

# for Mac
if (APPLE)
    link_directories("/opt/homebrew/lib")
    include_directories("/opt/homebrew/include")
endif ()

find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
find_library(SDL2_LIBRARY NAME SDL2)
find_package(sol2 CONFIG REQUIRED)

include_directories(lib)
include_directories(include)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE ${REQUIRED_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE sol2)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})




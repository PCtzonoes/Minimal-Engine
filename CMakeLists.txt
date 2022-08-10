cmake_minimum_required(VERSION 3.23)
project(mini_engine)

set(CMAKE_CXX_STANDARD 17)
set(REQUIRED_LIBS SDL2 SDL2_image SDL2_mixer SDL2_ttf lua)


include_directories(lib)
include_directories(include)

add_subdirectory(src)



target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS})

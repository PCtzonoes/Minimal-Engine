cmake_minimum_required(VERSION 3.23)
project(zeppelin_mo)

set(CMAKE_CXX_STANDARD 17)
set(REQUIRED_LIBS SDL2 SDL2_image SDL2_mixer SDL2_ttf lua5.3)


include_directories(lib)
include_directories(include)

add_subdirectory(src)



target_link_libraries(${PROJECT_NAME}  ${REQUIRED_LIBS})

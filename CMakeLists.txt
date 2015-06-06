cmake_minimum_required(VERSION 2.6)
project(k)

if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()

add_library(kar graphics.c sdl.c background.c sprite.c font.c util.c)
add_executable(k main.c intro.c)
target_link_libraries(k kar SDL)
add_executable(show_bcg show_bcg.c)
target_link_libraries(show_bcg kar SDL)

cmake_minimum_required(VERSION 2.6)
project(k)

if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -O0 -DDEBUG")
endif()

add_library(kar graphics.c sdl.c background.c sprite.c draw.c font.c script.c sound.c game.c util.c)
add_executable(k main.c intro.c)
target_link_libraries(k kar SDL)
add_executable(show_bcg show_bcg.c)
target_link_libraries(show_bcg kar SDL)
add_executable(show_sprite show_sprite.c)
target_link_libraries(show_sprite kar SDL)

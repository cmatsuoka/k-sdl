cmake_minimum_required(VERSION 2.6)
project(k)

if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()

add_executable(k main.c graphics.c sdl.c background.c font.c util.c)
target_link_libraries(k SDL)
add_executable(show_bcg show_bcg.c graphics.c sdl.c background.c util.c)
target_link_libraries(show_bcg SDL)

cmake_minimum_required(VERSION 2.6)
project(k)
add_executable(k main.c graphics.c sdl.c)
target_link_libraries(k SDL)

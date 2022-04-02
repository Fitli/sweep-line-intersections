cmake_minimum_required(VERSION 3.17)
project(sweep_line_intersections C)

set(CMAKE_C_STANDARD 99)

add_executable(sweep_line_intersections main.c heap.c heap.h segment.c segment.h tests.c tests.h rbtree.c rbtree.h)
target_link_libraries(sweep_line_intersections m)
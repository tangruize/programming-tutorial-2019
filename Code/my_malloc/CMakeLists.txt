cmake_minimum_required(VERSION 3.14)
project(my_malloc C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -g)
add_link_options(-pg)

# 添加一个静态链接库
add_library(my_malloc my_malloc.c my_malloc.h)

add_executable(my_malloc_test my_malloc_test.c)
target_link_libraries(my_malloc_test my_malloc)  # 链接 my_malloc

# gprof 工具用于查看函数执行时间和占比等, 需要添加编译选项 -g 和链接选项 -pg
add_custom_target(gprof
        COMMAND ./test && gprof test
        DEPENDS test
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

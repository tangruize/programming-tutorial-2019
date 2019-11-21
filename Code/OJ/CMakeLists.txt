project(OJ)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -fno-asm)
include_directories(../redirect)
link_libraries(m redirect)

add_executable(oj-example_c example.c)
add_executable(oj-example_cpp example.cpp)

# 添加更多 add_executable
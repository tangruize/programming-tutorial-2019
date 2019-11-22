project(Code)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_subdirectory(redirect)
add_subdirectory(oj)
add_subdirectory(pointer)
add_subdirectory(struct)
add_subdirectory(my_malloc)
add_subdirectory(io)
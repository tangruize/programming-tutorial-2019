project(OJ)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(my_malloc)

add_subdirectory(lib)
include_directories(lib)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -fno-asm)
link_libraries(m redirect)

if (EXISTS ${PROJECT_SOURCE_DIR}/src/CMakeLists.txt)
    add_subdirectory(src)
endif()

add_executable(oj-example_c example.c)
add_executable(oj-example_cpp example.cpp)

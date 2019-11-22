project(Slides)

add_custom_target(slides
        COMMAND $(MAKE)
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(slides-clean
        COMMAND $(MAKE) clean
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(slides-introduction.pdf
        COMMAND $(MAKE) DIR=2019-10-25_Introduction
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(slides-function_array.pdf
        COMMAND $(MAKE) DIR=2019-11-01_Function_Array
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(slides-pointer_memory.pdf
        COMMAND $(MAKE) DIR=2019-11-08_Pointer_Memory
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(slides-struct_malloc.pdf
        COMMAND $(MAKE) DIR=2019-11-16_Struct_Malloc
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(slides-io.pdf
        COMMAND $(MAKE) DIR=2019-11-22_IO
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
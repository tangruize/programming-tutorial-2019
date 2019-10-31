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

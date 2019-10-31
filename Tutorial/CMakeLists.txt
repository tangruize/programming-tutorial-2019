project(Tutorial)

add_custom_command(OUTPUT gitbook-update
        COMMAND rsync -rut --exclude=CMakeLists.txt --exclude=_book --delete Tutorial ${PROJECT_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_target(gitbook-clean
        COMMAND rm -rf Tutorial
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

add_custom_target(gitbook-build
        COMMAND gitbook build
        DEPENDS gitbook-update
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/Tutorial)

add_custom_target(gitbook
        COMMAND gitbook serve --no-live --no-watch
        DEPENDS gitbook-update
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/Tutorial)
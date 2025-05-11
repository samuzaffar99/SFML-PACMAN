cmake_minimum_required(VERSION 3.16)
project(SFML-PACMAN VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Optionally set compile flags for MSVC / GCC / Clang
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Add SFML from external folder
add_subdirectory(external/SFML)

# Source files
add_executable(SFML-PACMAN
    src/main.cpp src/map.cpp
)

# Link SFML libraries
target_link_libraries(SFML-PACMAN
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

# Include SFML headers
target_include_directories(SFML-PACMAN
    PRIVATE
        external/SFML/include
)

# Platform-specific settings (Windows: copy DLLs if needed)
if(WIN32)
    add_custom_command(TARGET SFML-PACMAN POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE:sfml-audio>
            $<TARGET_FILE_DIR:SFML-PACMAN>
    )
endif()
add_custom_command(TARGET SFML-PACMAN POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SFML-PACMAN>/assets
)

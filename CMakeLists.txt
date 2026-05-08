cmake_minimum_required(VERSION 3.20)
project(GgBot CXX)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# GLFW
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD (OpenGL 3.3 Core)
FetchContent_Declare(glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v0.1.36
)
FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    set(GLAD_PROFILE "core" CACHE STRING "" FORCE)
    set(GLAD_API "gl=3.3" CACHE STRING "" FORCE)
    set(GLAD_GENERATOR "c" CACHE STRING "" FORCE)
    add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif()

# GLM (header-only)
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

set(SOURCES
    src/main.cpp
    src/state.cpp
    src/shader.cpp
    src/texture.cpp
    src/input.cpp
    src/animation.cpp
    src/mesh.cpp
    src/robot.cpp
    src/scene.cpp
)

add_executable(GgBot ${SOURCES})

target_include_directories(GgBot PRIVATE
    ${glad_SOURCE_DIR}/include
    vendor/stb
    src
)

target_link_libraries(GgBot PRIVATE glfw glad glm)

if(APPLE)
    target_link_libraries(GgBot PRIVATE "-framework OpenGL")
    target_compile_definitions(GgBot PRIVATE GL_SILENCE_DEPRECATION)
endif()

# Copy shaders and textures next to the executable after build
add_custom_command(TARGET GgBot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:GgBot>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:GgBot>/textures
)

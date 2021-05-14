cmake_minimum_required(VERSION 3.19)
project(rayTracing CUDA)

set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(INCLUDE_ROOT ${PROJECT_SOURCE_DIR}/include)
set(TARGET_NAME rayTracing)

add_compile_options(-O3 -Wall)

include_directories("/home/boris/libs/stb")

add_executable(${TARGET_NAME} src/main.cu)

set_target_properties(
    ${TARGET_NAME}
        PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)

target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDE_ROOT})

add_subdirectory(src)

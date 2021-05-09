cmake_minimum_required(VERSION 3.19)
project(rayTracing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(INCLUDE_ROOT ${PROJECT_SOURCE_DIR}/include)
set(TARGET_NAME rayTracing)

add_compile_options(-Wall -Werror -O3)

include_directories("/home/boris/libs/stb")

add_executable(${TARGET_NAME} src/main.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDE_ROOT})

add_subdirectory(src)

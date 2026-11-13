cmake_minimum_required(VERSION 3.16)

project(FractalJulia)

find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS Graphics Window System CONFIG REQUIRED)

add_executable(fractal-julia
    fractal-serial.cpp
    main.cpp
    palette.cpp
    fractal-simd.cpp
)

# Habilitar instrucciones AVX2 para la implementación SIMD
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(fractal-julia PRIVATE -mavx2)
endif()

target_link_libraries(fractal-julia PRIVATE
    fmt::fmt
    SFML::Graphics
    SFML::Window
    SFML::System
)
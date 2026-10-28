cmake_minimum_required(VERSION 3.16)
 
project(Ejemplo01)
 
find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS Graphics Window System CONFIG REQUIRED)
 
add_executable(ejemplo01 main.cpp)
target_link_libraries(ejemplo01 PRIVATE
    fmt::fmt
    SFML::Graphics SFML::Window SFML::System
 
)
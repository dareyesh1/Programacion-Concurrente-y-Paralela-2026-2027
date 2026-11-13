#pragma once

#include <cstdint>

extern int max_iteraciones;

//Funcion para calcular el fractal Julia
//Esta es como una interfaz en java

void julia_simd( double x_min, double y_min, double x_max, double y_max, 
    uint32_t width, uint32_t height, uint32_t* pixel_buffer);
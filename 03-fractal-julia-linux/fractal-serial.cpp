#include "fractal-serial.h"
#include "palette.h"

#include <complex> #include <cstdint> #include <cmath>

// Constante del fractal de Julia 
std::complex<double> c(-0.7, 0.27015);

int max_iteraciones = 50;

// Determinar si el punto pertenece al conjunto 
uint32_t acotado( std::complex<double> z0) { uint32_t iter = 0;

std::complex<double> z = z0;

while (iter < max_iteraciones && std::abs(z) < 2.0)
{
    z = z * z + c;
    iter++;
}

if (iter < max_iteraciones)
{
    // El punto escapa
    //return 0xFFFFFFFF; // Blanco

    int index = iter % PALETTE_SIZE;
    return color_ramp[index];
}

// El punto pertenece al conjunto
//return 0xFF000000; // Negro opaco

int index = iter % PALETTE_SIZE;
    return color_ramp[index];
}

// Calcular fractal de Julia 
void julia_serial( double x_min, double y_min, double x_max, double y_max, uint32_t width, uint32_t height, uint32_t* pixel_buffer) { 
    
    double dx = (x_max - x_min) / width; 
    double dy = (y_max - y_min) / height;

for (uint32_t i = 0; i < width; i++)
{
    for (uint32_t j = 0; j < height; j++)
    {
        double x = x_min + i * dx;
        double y = y_max - j * dy;

        std::complex<double> z0(x, y);

        uint32_t color = acotado(z0);

        pixel_buffer[j * width + i] = color;
    }
}


}
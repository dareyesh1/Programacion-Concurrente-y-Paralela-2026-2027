
#include "fractal-serial.h"

#include <complex> //Trabajar con numeros complejos

#include <cstdint>

std::complex<double> c(-0.7, 0.27015);

int max_iteraciones = 10;

uint32_t acotado(std::complex<double> z0)
{

    int iter = 1;

    std::complex<double> z = z0;

    while (iter < max_iteraciones && std::abs(z) < 2.0)
    {
        z = z * z + c;
        iter++;
    }

    if (iter < max_iteraciones)
    {
        // norma>2
        return 0x00000000; // Blanco
    }

    // norma<=2
    return 0xFFFFFFFF; // Negro
}

void julia_serial(double x_min, double y_min, double x_max, double y_max,
                  uint32_t width, uint32_t height,
                  uint32_t *pixel_buffer)
{

    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            double x = x_min + i * dx;
            double y = y_max - j * dy;

            std::complex<double> z0(x, y);

            uint32_t color = acotado(z0);

            pixel_buffer[j * width + i] = color;
        }
    }
}

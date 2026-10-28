#include <iostream>
#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <cmath>

//Ejes coordenados XY
double x_min = -1;
double x_max = 1;
double y_min = -1;
double y_max = 1;
 
#define WIDTH 900
#define HEIGHT 900

//Imgagen / Textura
std::uint32_t*buffer = nullptr;

void dibujar(){
    double dx = (x_max - x_min) / WIDTH;
    double dy = (y_max - y_min) / HEIGHT;

    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            double x = x_min + i * dx;
            double y = y_max - j * dy;

            //Funcion a dibujar
            //double z = x*x + y*y;
            double z = cos(x*x) + sin(y*y);

            //Indexar
            buffer[j * WIDTH + i] = 0x0000FFFF; //Color azul

            int rojo = z * 255/2;

            buffer[j*WIDTH + i] = (rojo << 24) | (0x0000FFFF); //Color azul + rojo


        }
        
    }
}
int main(){

    buffer = new uint32_t[WIDTH * HEIGHT];
     // Create the main window
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML window");


    dibujar();

    sf::Texture texture({WIDTH, HEIGHT});
    sf::Sprite sprite(texture);
 
    texture.update((const uint8_t*)buffer);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Clear screen
        window.clear();

        // Draw the string
        window.draw(sprite);

        // Update the window
        window.display();
    }

}
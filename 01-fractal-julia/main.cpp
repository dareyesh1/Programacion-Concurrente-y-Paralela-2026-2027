#include <iostream>
#include <fmt/core.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp> //Busca en el sistema o en alguna dependencia

#include "fractal-serial.h" //Entre comillas si el módulo está dentro del Proyecto
#include <fmt/format.h>

// Ejes en coordenadas x,y
double x_min = -1.5;
double x_max = 1.5;
double y_min = -1.0;
double y_max = 1.0;

#define WIDTH 1600
#define HEIGHT 900

// imagen, textura
uint32_t *buffer = nullptr;

int main()
{

    buffer = new uint32_t[WIDTH * HEIGHT];

    // Create the main window
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Dibujo funcion");

    //Configurar Textos en el gràfico
    sf::Font font("ARIAL.TTF");
    sf::Text text(font, "Fractal", 24);
    text.setFillColor(sf::Color::White);
    text.setPosition({10,10});
    text.setStyle(sf::Text::Bold);

    // Load a sprite to display
    sf::Texture texture({WIDTH, HEIGHT});
    sf::Sprite sprite(texture);

    int frames = 0;
    int fps = 0;
    sf::Clock clock;

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

        julia_serial(x_min, y_min, x_max, y_max, WIDTH, HEIGHT, buffer);
        texture.update((const uint8_t *)buffer);

        // medir fps
        frames++;

        if (clock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frames;
            frames = 0;
            clock.restart();
        }

        std::cout<<"FPS: "<<fps<<std::endl;

        auto msg = fmt::format("Fractal - Iteraciones : {}, FPS: {}", 10, fps);
        text.setString(msg);
        
        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);
        window.draw(text);

        // Update the window
        window.display();
    }

    return 0;
}
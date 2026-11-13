#include <iostream>
#include <cstdint>
#include <string>
 
#include <fmt/core.h>
#include <fmt/format.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <complex>
#include <cstring>
 
#include "fractal-serial.h"
#include "fractal-simd.h"
 
// Ejes en coordenadas x,y
double x_min = -1.5; double x_max = 1.5; double y_min = -1.0; double y_max = 1.0;
 
 
 
#define WIDTH 1600
#define HEIGHT 900
 
// Buffer de imagen
uint32_t* buffer = nullptr;
 
//Constantes para saber que tipo es
 
enum class runtime_type{
    SERIAL_1,
    SIMD
};
 
int main()
{
    buffer = new uint32_t[WIDTH * HEIGHT];
 
    runtime_type r_type = runtime_type::SERIAL_1;
 
    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Dibujo funcion"
    );
 
    window.setFramerateLimit(60);
 
    sf::Font font("arial.ttf");
 
    sf::Text text(font, "Fractal", 24);
    text.setFillColor(sf::Color::White);
    text.setPosition({10.f, 10.f});
    text.setStyle(sf::Text::Bold);
 
    std::string options = "[1] Serial | [3] SIMD | Up/Down: Iterations";
 
    sf::Text text_options(font, options, 20);
    text_options.setFillColor(sf::Color::White);
    text_options.setPosition({10.f, 860.f});
    text_options.setStyle(sf::Text::Bold);
 
    sf::Texture texture({WIDTH, HEIGHT});
    sf::Sprite sprite(texture);
 
    int frames = 0;
    int fps = 0;
 
    sf::Clock clock;
 
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
 
            else if (const auto* evt =
                event->getIf<sf::Event::KeyReleased>())
            {
                switch (evt->scancode)
                {
                    case sf::Keyboard::Scancode::Up:
                        max_iteraciones += 10;
                        break;
 
                    case sf::Keyboard::Scancode::Down:
                        if (max_iteraciones > 10)
                            max_iteraciones -= 10;
                        break;
 
                    case sf::Keyboard::Scancode::Num1:
                        r_type = runtime_type::SERIAL_1;
                        break;
 
                    case sf::Keyboard::Scancode::Num3:
                        r_type = runtime_type::SIMD;
                        break;
 
                    default:
                        break;
                }
            }
        }
 
        if (!window.isOpen())
            break;
 
        std::string mode;
 
        if (r_type == runtime_type::SERIAL_1)
        {
            mode = "Serial";
 
            julia_serial(
                x_min, y_min,
                x_max, y_max,
                WIDTH, HEIGHT,
                buffer
            );
        }
        else if (r_type == runtime_type::SIMD)
        {
            mode = "SIMD";
 
            julia_simd(
                x_min, y_min,
                x_max, y_max,
                WIDTH, HEIGHT,
                buffer
            );
        }
 
        texture.update(
            reinterpret_cast<const std::uint8_t*>(buffer)
        );
 
        frames++;
 
        if (clock.getElapsedTime().asSeconds() >= 1.0f)
        {
            fps = frames;
            frames = 0;
            clock.restart();
        }
 
        auto msg = fmt::format(
            "Fractal {} - Iteraciones: {}, FPS: {}",
            mode,
            max_iteraciones,
            fps
        );
 
        text.setString(msg);
 
        window.clear();
 
        window.draw(sprite);
        window.draw(text);
        window.draw(text_options);
 
        window.display();
    }
 
    delete[] buffer;
 
    return 0;
}
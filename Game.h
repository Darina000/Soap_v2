#include <iostream>
#include "Flock.h"
#include "Boid.h"
#include "Pvector.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#ifndef GAME_H
#define GAME_H
#include <vector>


class Game {
private:
    sf::RenderWindow window;
    int window_width = 1100;
    int window_height = 1100;

    
    Flock flock;
    float boidsSize;
    
    std::vector<sf::CircleShape> shapes;
    std::vector<sf::RectangleShape> line;

    void Render();
    void HandleInput();
    
    void moving(int koeff, sf::Vector2<float>);
    
    ~Game(){};

public:
    Game();
    void Run();
};

#endif

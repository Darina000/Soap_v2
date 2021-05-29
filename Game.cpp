#include <iostream>
#include "Flock.h"
#include "Boid.h"
#include "Pvector.h"
#include "Game.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include <math.h>
#include <iostream>

#include <cstdlib>
#include <cmath>
#include <cerrno>
#include <cfenv>


#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

// Construct window using SFML
Game::Game()
{
    this->boidsSize = rand() % 50 ;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
   // this->window_height = desktop.height;
  //  this->window_width  = desktop.width;
    this->window.create(sf::VideoMode(window_width, window_height, desktop.bitsPerPixel), "Boids", sf::Style::None);
    this->window.setVerticalSyncEnabled(true);
    
    window.setFramerateLimit(60);
}

// Run the simulation. Run creates the boids that we'll display, checks for user
// input, and updates the view
void Game::Run()
{
    for (int i = 0; i < 100; i++) {
        Boid b(window_width / 3, window_height / 3); // Starts all boids in the center of the screen
        sf::CircleShape shape(1);

        // Changing the Visual Properties of the shape
        // shape.setPosition(b.location.x, b.location.y); // Sets position of shape to random location that boid was set to.
        shape.setPosition(window_width, window_height); // Testing purposes, starts all shapes in the center of screen.
      //  shape.setOutlineColor(sf::Color(0,255,0));
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::Cyan);
        shape.setOutlineThickness(1);
        shape.setRadius(rand() % 50);

        // Adding the boid to the flock and adding the shapes to the vector<sf::CircleShape>
        flock.addBoid(b);
        shapes.push_back(shape);
        
        
    }
    //blocks for ray (ray build from block)
    for (int i=0; i<200; ++i ){
        sf::RectangleShape block(sf::Vector2f(window_width/200 + 10.f, 50.f));
        block.setSize(sf::Vector2f(window_width/200+10.f, 20.f));
        block.setPosition(i*window_width/200, window_height/2);
        block.setFillColor(sf::Color::Red);
        line.push_back(block);
    }
    while (window.isOpen()) {
        HandleInput();
        Render();
    }
}

void Game::HandleInput()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        // "close requested" event: we close the window
        // Implemented alternate ways to close the window. (Pressing the escape, X, and BackSpace key also close the program.)
        if ((event.type == sf::Event::Closed) ||
            (event.type == sf::Event::KeyPressed &&
             event.key.code == sf::Keyboard::Escape) ||
            (event.type == sf::Event::KeyPressed &&
             event.key.code == sf::Keyboard::BackSpace) ||
            (event.type == sf::Event::KeyPressed &&
             event.key.code == sf::Keyboard::X))
             {
            window.close();
        }
    }

    //its for adding bubble by mouse, we dont need it, but  for usin - to change color and size
    /*
    // Check for mouse click, draws and adds boid to flock if so.
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        // Gets mouse coordinates, sets that as the location of the boid and the shape
        sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
        Boid b(mouseCoords.x, mouseCoords.y, false);
        sf::CircleShape shape(4.0f);

        // Changing visual properties of newly created boid
        shape.setPosition(mouseCoords.x, mouseCoords.y);
        shape.setOutlineColor(sf::Color(255, 0, 0));
      //  shape.setFillColor(sf::Color(0, 0, 0));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);
        shape.setRadius(boidsSize);

        // Adds newly created boid and shape to their respective data structure
        flock.addBoid(b);
        shapes.push_back(shape);
        
        // New Shape is drawn
        window.draw(shapes[shapes.size()-1]);
    }
    */
    
    
}
float distance(const sf::Vector2<float>& v1, const sf::Vector2<float>& v2){
    
    return sqrt((v1.x - v2.x)*(v1.x - v2.x)+(v1.y - v2.y)*(v1.y - v2.y));
}


void Game::moving(int koeff, sf::Vector2<float> v){
    
    for (int i = 0; i < line.size(); ++i){
        
        float dist = distance(v, line[i].getPosition());
                              
        if (dist < 5.0f){
            sf::RectangleShape shape(sf::Vector2f(70.f, 3.f));
            shape.rotate(90*dist);
            
            shape.setPosition(line[i].getPosition().x+ dist*30.0f, line[i].getPosition().y+ dist*koeff*(rand()%100) );

            shape.setFillColor(sf::Color(255, 0, 0));
         //   shape.setOutlineColor(sf::Color::White);
            line.push_back(shape);
        }
        float dis = abs(line[i].getPosition().y - window_height/2);
        line[i].setFillColor(sf::Color((510*dist)/window_height, 0, 0));
        
        if ((dis > 100.f) && line[i].getFillColor() == sf::Color(0, 0, 0)){
            line.erase(line.begin() + i);
        }
    }
    
    //just delete a part vector for member, so we can delete them by some paramter
    if (line.size() > 500){
     //   std::sort(line.begin(), line.end());
        line.erase(line.begin()+ 499);
    }
}

void Game::Render()
{
    window.clear();
    for (int j = 0; j < line.size(); ++j){
        window.draw(line[j]);
 
        
    }
    
    // Draws all of the Boids out, and applies functions that are needed to update.
    for (int i = 0; i < shapes.size(); i++) {
        int count_bottom = 0;
        int count_top = 0;
        window.draw(shapes[i]);

        // Matches up the location of the shape to the boid
        shapes[i].setPosition(flock.getBoid(i, 1).location.x, flock.getBoid(i, 1).location.y);

        // Calculates the angle where the velocity is pointing so that the triangle turns towards it.
        float theta = flock.getBoid(i, 1).angle(flock.getBoid(i, 1).velocity);
        shapes[i].setRotation(theta);

        // Prevent boids from moving off the screen through wrapping
        // If boid exits right boundary
        if (shapes[i].getPosition().x > window_width)
            shapes[i].setPosition(shapes[i].getPosition().x - window_width, shapes[i].getPosition().y);
        // If boid exits bottom boundary
        if (shapes[i].getPosition().y > window_height)
            shapes[i].setPosition(shapes[i].getPosition().x, shapes[i].getPosition().y - window_height);
        // If boid exits left boundary
        if (shapes[i].getPosition().x < 0)
            shapes[i].setPosition(shapes[i].getPosition().x + window_width, shapes[i].getPosition().y);
        // If boid exits top boundary
        if (shapes[i].getPosition().y < 0)
            shapes[i].setPosition(shapes[i].getPosition().x, shapes[i].getPosition().y + window_height);
        
      
        
           if (( shapes[i].getPosition().y - window_height/2) > 10 ){ //bottom
               moving(-1,shapes[i].getPosition());
           }else if(( shapes[i].getPosition().y - window_height/2) < 10 ){ //top
               moving(1,shapes[i].getPosition());
           }
        
    }
    flock.flocking();


    window.display();
}


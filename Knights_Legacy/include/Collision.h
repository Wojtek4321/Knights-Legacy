#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace Collision
{
    class Object
    {
    public:
        Object(sf::FloatRect rect);
        sf::FloatRect getBounds() const;

    private:
        sf::FloatRect bounds; 
    };

    
    void addObject(std::vector<Object>& objects, float x, float y, float width, float height);
    void removeLastObject(std::vector<Object>& objects);

    // XML OBSLUGA
    std::vector<Object> parseXML(const std::string& filename);
    void renderObjects(sf::RenderWindow& window, const std::vector<Object>& objects);
}

 class Blokada {
private:
    sf::FloatRect area; 
    bool isActive;  
    sf::RectangleShape rect;  

public:



    Blokada(float x1, float y1, float x2, float y2);
    bool checkCollision(const sf::FloatRect& playerBounds);

    void draw(sf::RenderWindow& window);

    bool isCollisionActive() const;
    void unlockCollision();
    
    
}; 

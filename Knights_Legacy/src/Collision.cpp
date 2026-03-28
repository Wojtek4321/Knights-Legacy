#include "Collision.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Collision
{
    // Konstruktor obiektu kolizji
    Object::Object(sf::FloatRect rect) : bounds(rect) {}


    // Funkcja zwracaj¹ca prostok¹t obiektu kolizji
    sf::FloatRect Object::getBounds() const
    {
        return bounds;
    }

    // Funkcja do parsowania pliku XML i tworzenia obiektów kolizji
    std::vector<Object> parseXML(const std::string& filename)
    {
        std::vector<Object> objects;
        std::ifstream file(filename);

        if (!file.is_open())
        {
            std::cerr << "!!!!!! nie otwarto pliku xml !!!!!!" << std::endl;
            return objects;
        }

        std::string line;
        bool insideObjectGroup = false;

        while (std::getline(file, line))
        {
            
            if (line.find("<objectgroup") != std::string::npos)
            {
                insideObjectGroup = true;
            }

            if (insideObjectGroup && line.find("<object") != std::string::npos)
            {
                size_t xPos = line.find("x=\"");
                size_t yPos = line.find("y=\"");
                size_t widthPos = line.find("width=\"");
                size_t heightPos = line.find("height=\"");

                if (xPos != std::string::npos && yPos != std::string::npos &&
                    widthPos != std::string::npos && heightPos != std::string::npos)
                {
                 
                    float x = std::stof(line.substr(xPos + 3, line.find("\"", xPos + 3) - (xPos + 3)));
                    float y = std::stof(line.substr(yPos + 3, line.find("\"", yPos + 3) - (yPos + 3)));
                    float width = std::stof(line.substr(widthPos + 7, line.find("\"", widthPos + 7) - (widthPos + 7)));
                    float height = std::stof(line.substr(heightPos + 8, line.find("\"", heightPos + 8) - (heightPos + 8)));

                  //dodajemy obiekty
                    addObject(objects, x, y, width, height);
                }
            }

            
            if (insideObjectGroup && line.find("</objectgroup>") != std::string::npos)
            {
                insideObjectGroup = false;
            }
        }

        return objects;
    }
    void addObject(std::vector<Object>& objects, float x, float y, float width, float height)
    {
        objects.emplace_back(sf::FloatRect(x, y, width, height));
    }

    void Collision::removeLastObject(std::vector<Object>& objects)
    {
        if (!objects.empty())
        {
            std::cout << "przed usuniêciem " << objects.size() << " obiektów" << std::endl;
            std::cout << "usuwam obiekt  "
                << "X: " << objects.back().getBounds().left
                << ", Y: " << objects.back().getBounds().top
                << ", Width: " << objects.back().getBounds().width
                << ", Height: " << objects.back().getBounds().height
                << std::endl;

            objects.pop_back();

            std::cout << "po usuniêciu " << objects.size() << " obiektów" << std::endl;
        }
        else
        {
            std::cerr << " Wektor jest pusty brak obiektów do usuniêcia blad" << std::endl;
        }
    
    }
    void renderObjects(sf::RenderWindow& window, const std::vector<Object>& objects)
    { 
        for (const auto& obj : objects)
        {
            //iliterowanie po object potem szer hig prostokata
            sf::RectangleShape rectangle(sf::Vector2f(obj.getBounds().width, obj.getBounds().height));
            rectangle.setPosition(obj.getBounds().left, obj.getBounds().top);
            rectangle.setFillColor(sf::Color::Red);

           
            window.draw(rectangle);
        }
    }
    

    
}

Blokada::Blokada(float x1, float y1, float x2, float y2)
{
    // Tworzymy prostok¹t na podstawie lewego górnego (x1, y1) i prawego dolnego (x2, y2) rogu
    area = sf::FloatRect(x1, y1, x2 - x1, y2 - y1); // Obliczamy szerokoœæ i wysokoœæ

    isActive = true;  // Kolizja jest aktywna na pocz¹tku

    rect.setSize(sf::Vector2f(area.width, area.height));
    rect.setFillColor(sf::Color(255, 0, 0, 128));  // Przezroczysty czerwony
    rect.setPosition(area.left, area.top);
}

// funkcja sprawdzaj¹ca kolizje
bool Blokada::checkCollision(const sf::FloatRect& playerBounds)
{
    if (isActive && area.intersects(playerBounds)) {
        return true;  // Kolizja wykryta
    }
    return false;
}

//funkcja rysuj¹ca obszar kolizji jeœli jest aktywna
void Blokada::draw(sf::RenderWindow& window)
{
    if (isActive) {
        window.draw(rect);  // Rysowanie prostok¹ta reprezentuj¹cego kolizjê
    }
}


bool Blokada::isCollisionActive() const
{
    return isActive;
}


void Blokada::unlockCollision()  // Funkcja do odblokowania kolizji
{
    isActive = false;  
}


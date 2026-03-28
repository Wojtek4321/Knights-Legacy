
#pragma once
#include <SFML/Graphics.hpp>

class Buildings
{
private:
    sf::Texture* destroyTexture;
    sf::Texture* buildTexture;
    sf::Texture* stayTexture;
	sf::Sprite bud;
	int hp;
    float plus;
    bool resp = true;
   
public:
	Buildings(float x,float y,int hp,sf::Texture* destroy,sf::Texture* build,sf::Texture* stay,float plus,bool isEnemy);

	void draw(sf::RenderWindow& window);
	void update();
	void takeDamage(int damage);
	void startBuilding();
    sf::Vector2f getPosition() const;
    bool canRespawn() const;
    bool isbuilding = false;
    bool isbuiled = false;
    sf::Clock buildClock;
    
    int health() const;
    float plus1() const;
    sf::Clock hitClock;
    
};


class Pawn
{
public:
    Pawn(float x, float y,sf::Texture* texture, float speed );

    void update(float deltaTime, int& mission, int treeP, int goldP);
    void draw(sf::RenderWindow& window);
    void moveTo(const sf::Vector2f target);
    void moveToAndBuild(Buildings& building);
    bool isPlayerClose(const sf::Vector2f& playerPos) ;
    void    NPC(int& mission,int treeP, int goldP);
    sf::Vector2f getPosition() const;
    
private:
    sf::Sprite sprite;
    sf::Texture* texture;
    float speed;
    bool isMoving = false;
    bool isBuilding = false;


    int currentFrame = 0;
    float frameTime = 0.1f;                      
    float animationTimer = 0.f;

    sf::Vector2f targetPost;
    bool moveTarget = false;

   
    Buildings* targetBuilding = nullptr;
    bool hasTargetBuilding = false;

    float buildTimer = 0.f; 
    const float buildDuration = 20.f; 
    float pluss;

    sf::RectangleShape timerBar;

    
    bool wykrzyknik = true;
    bool kropki = false;
    sf::Text wykrzyknikT;
    sf::Text kropkiT;
    sf::Font font;

    bool isClose = false;
    bool missionCompleted[17] = { false, false, false, false, false ,false,false, false, false, false, false ,false,false, false, false ,false };
};
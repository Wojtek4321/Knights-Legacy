#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "Collision.h"

enum EnemyState { IDLE, WALKING, ATTACKING, DEAD ,RUNNING}; //runing for sheep

class Enemy {

    //podstawy
protected:
    sf::Vector2f position;
    sf::Vector2f spawnpostiion;
    sf::Vector2f newSpawnPosition;
    float speed;
    float attackRange;
    EnemyState state;
    bool isFacingLeft;

    //anim
    int currentFrame;
    sf::Clock animationClock;
    sf::Sprite sprite;
    sf::Texture* texture;

    int health;
    
    int attackDamage;
    sf::Clock attackTimer;
    sf::Clock deathTimer;
    const float  healthfull;  //podane pierwsze hp w konst
    float scaleX = 5;
    float scaleY = 5;
    //boss

    std::string attackDirection;
    sf::RectangleShape healthBar;  // Pasek zdrowia
    sf::RectangleShape healthBarBackground;
    
    bool canRespawn = true;
    sf::Clock respawnClock;
    float respawnDelay = 3.0f; // sekundy
    bool readyToSpawn = false;
    bool fullyDead = false;
   

public:
    
    Enemy(float x, float y, float speed, float attackRange, sf::Texture* texture,int health);
    sf::Vector2f getPosition() const;
    float getHealth() const;
    void update(sf::Vector2f playerPosition, float& playerHealth, std::vector<Collision::Object>& objects);
    void moveTowards(sf::Vector2f target,std::vector<Collision::Object>& objects);
    void attack(float& playerHealth, sf::Vector2f playerPosition);
    void die();
    void takeDamage(float damage);
    void draw(sf::RenderWindow& window);
    void animate();
    sf::FloatRect getGloBou() const;
    void setRespawnAvailable(bool value, sf::Vector2f newSpawnPosition);
    void Respawn();
    bool FullDead();
};



class Tree {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Clock animationClock;
    sf::Clock regenClock;  
    sf::Clock attackClock;
    int frameIndex;    
    float frameTime;   // jedn¹ klatkê
    bool isChopped;    // sciete
    int health;        
    sf::Vector2f position;
    float topx;
    float topy;
    bool hasItemDropped = false;
    int iloscitemot = 1;
   
public:
    Tree(float x, float y, int hp, sf::Texture* tex, std::vector<Collision::Object>& objects);
    void draw(sf::RenderWindow& window);
    void takeDamage(int damage, std::vector<Collision::Object>& objects);
    void update(std::vector<Collision::Object>& objects);
    int getHealth() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    float TOPx();
    float TOPy();
    bool hasDroppedItem() const;
};



class Sheep : public Tree {
public:
    Sheep(float x, float y, int hp, sf::Texture* idleTex, std::vector<Collision::Object>& objects1);

    void update(sf::Vector2f playerPosition, std::vector<Collision::Object>& objects);
    void takeDamage(int damage) ;
    void draw(sf::RenderWindow& window) ;
    int getHealth() const;

    sf::Vector2f getPosition() const;
    bool hasDroppedItem1() const;

private:
    bool isRunning;
    sf::Clock runClock;
    sf::Texture runTexture; // Tekstura biegania
    sf::Vector2f spawnPosition;
    sf::Clock respawnClock;
    sf::Texture* texture;
    int iloscitemo = 1;
};


class GoldMine : public Tree
{
public :
    GoldMine(float x, float y, int hp, sf::Texture* destroy, std::vector<Collision::Object>& objects);

    void draw(sf::RenderWindow& window);
    bool hasDroppedItem2() const;
    void build(int damage);
    int getHealth1() const;
    void update();
    sf::Vector2f getPosition() const;

private:
    int iloscitemoG = 0;
    sf::Texture* texture;
    sf::Texture active;
    sf::Texture inactive;
    bool builded;
    bool isActive;
    sf::Clock droptime;
    

}; 
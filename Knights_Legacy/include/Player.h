#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Buildings_Story.h"
#include "Collision.h" 
#include "enemy.h"


class Player
{
private:
	
	sf::Sprite SpKnight;
	sf::Texture teKnight;
	sf::Vector2f position;
	//poruszanie
	sf::Vector2f velocity;
	float speed=0.2f;

	//animacja 
	sf::IntRect currentFrame;
	float animationTimer;
	int frameIndex;
	bool isMoving;
	std::string lastDirection;
	bool hit = false;

	

	//gra
	sf::Clock healthTimer;
	bool isDead = false;
	float hp;
	sf::Clock attackCooldown; // Licznik cooldownu ataku
	bool canAttack = true;
	bool oneatack = true; // bez wielu atakow na raz
	float attackCooldownTime = 1.0f;

	//conf
	sf::Texture aliveTexture;   // Tekstura dla ¿ywego rycerza
	sf::Texture deadTexture;

	sf::Text nick;

	int sheepitem = 0;
	int treeitem = 0;
	int golditem = 0;

	

public:



	Player();

	//animacja z poruszaniem

	void poruszanie(const std::vector<Collision::Object>& objects);

	void update(float deltaTime, const std::vector<Collision::Object>& objects, const std::vector<Tree>& trees, sf::Text& nick);

	void draw(sf::RenderWindow& window);

	sf::Vector2f getPosition() const;

	//pozycja calego prostokatu spirte
	sf::FloatRect getGloBou() const;

	//uderzenia
	void hits(std::vector<Enemy>& enemies,std::vector<Tree>& trees, std::vector<Sheep>& sheeps, std::vector<Collision::Object>& objects, std::vector<GoldMine>& mines, std::vector<Buildings>& buildings);

	void dead(const std::string& texturedead);

	//void Damage(int amount);

	float& getHealth();
	//void healtbar();

	void loadTextures(int selectknight);
	//void Itemy(bool dostalemS, bool dostalemT, bool dostalemG);
	void uzyjItem(int index);
	int sheepitem1() const;
	int treeitem1() const;
	int golditem1() const;
	void decreaseTreeItem(int amount);
	void decreaseGoldItem(int amount);
	~Player();

};


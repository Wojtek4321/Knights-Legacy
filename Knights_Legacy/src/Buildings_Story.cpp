#include "Buildings_Story.h"
#include <iostream>
Buildings::Buildings(float x, float y, int hp, sf::Texture* destroy, sf::Texture*  build, sf::Texture* stay,float plus,bool isEnemy) : plus(plus),hp(hp), destroyTexture(destroy), buildTexture(build), stayTexture(stay)
{
	
	bud.setOrigin(64, 96);
	bud.setPosition(x,y);
    if (isEnemy)
    {
        hp = 100;
        isbuiled = true;        // od razu zbudowany
        isbuilding = false;

        bud.setTexture(*stayTexture);
        //bud.setTextureRect(sf::IntRect(0,0,192,192));// gotowa baza
    }
    else
    {
        isbuiled = false;
        isbuilding = false;
        bud.setTexture(*destroyTexture); // wymaga budowy
    }
}
float Buildings::plus1() const
{
    return plus;
}
int Buildings::health() const
{
    return hp;
}

void Buildings::update()
{
    if (isbuiled)
    {
        bud.setTexture(*stayTexture);
    }
    else if (isbuilding)
    {
        if (buildClock.getElapsedTime().asSeconds() < 20.0f- plus1())
        {
            bud.setTexture(*buildTexture);
        }
        else
        {
            isbuiled = true;
            isbuilding = false;
            bud.setTexture(*stayTexture);
        }
    }
    else
    {
        resp = false;
        bud.setTexture(*destroyTexture);  //ZACZYNA
    }
    if (hitClock.getElapsedTime().asMilliseconds() < 100)
    {
        bud.setColor(sf::Color::Red);  // kolor b³ysku
    }
    else
    {
        bud.setColor(sf::Color::White); // powrót do normy
        //gotHit = false;
    }
}

void Buildings::takeDamage(int damage)
{
    hp -= damage;
    hitClock.restart();
    
    
    //bud.setColor(sf::Color::White);
    if (hp <= 0)
    {
        hp = 0;
        isbuiled = false;
        isbuilding = false;
        bud.setTexture(*destroyTexture);
        resp = false;
    }
}

void Buildings::startBuilding()
{
    if (!isbuilding && !isbuiled)
    {
        isbuilding = true;
        buildClock.restart();
    }
}
void Buildings::draw(sf::RenderWindow& window)
{
    window.draw(bud);
}
sf::Vector2f Buildings::getPosition() const
{
   
   
        return bud.getPosition();
        
}

bool Buildings::canRespawn() const
{
    return resp;
}






Pawn::Pawn(float x, float y, sf::Texture* texture, float speed): speed(speed),texture(texture)
{
    font.loadFromFile("assets/TekU/SMW Text 2 NC.ttf");

    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 192, 192)); // Idle
    sprite.setOrigin(96, 96);
    sprite.setPosition(x,  y); 

    timerBar.setFillColor(sf::Color::White);
    timerBar.setSize(sf::Vector2f(50.f, 10.f));

}

void Pawn::draw(sf::RenderWindow& window)
{
    if (wykrzyknik == true)
    {
        window.draw(wykrzyknikT);
    }
    if(!isMoving && !isBuilding && !hasTargetBuilding&& kropki==true)
    {
        window.draw(kropkiT);
    }
    
    window.draw(sprite);
    window.draw(timerBar);
}

void Pawn::update(float deltaTime,int& mission, int treeP, int goldP)
{
    
    wykrzyknikT.setPosition(sprite.getPosition().x - 10, sprite.getPosition().y - 80);

    kropkiT.setPosition(sprite.getPosition().x - 20, sprite.getPosition().y - 90);
    if (moveTarget)   //wybiera droge 
    {
        sf::Vector2f direction = targetPost - sprite.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance > 1.0f)  
        {
            direction /= distance;
            sprite.move(direction * speed * deltaTime);
            sprite.setScale((direction.x < 0) ? -1.f: 1.f, 1.f);
        }
        else
        {
            sprite.setPosition(targetPost);
            moveTarget = false;
            isMoving = false;

            if (hasTargetBuilding && targetBuilding)
            {
                pluss = targetBuilding->plus1();
                targetBuilding->startBuilding();
                isBuilding = true;
                buildTimer = 0.f;

                targetBuilding = nullptr;
                hasTargetBuilding = false;
            }
        }
    }

    if (isBuilding)
    {
        buildTimer += deltaTime;
        float totalTime = buildDuration - pluss;

        float timeProportion = buildTimer / totalTime;

        float currentWidth = (1 - timeProportion) * 50.f;

        // Upewnij siê, ¿e szerokoœæ paska nie przekroczy 0
        currentWidth = std::max(currentWidth, 0.f);

        // Ustaw szerokoœæ paska
        timerBar.setSize(sf::Vector2f(currentWidth, 10.f));  // Zmieniamy wysokoœæ paska na 10 pikseli

        // Ustaw pozycjê paska nad g³ow¹ postaci
        timerBar.setPosition(sprite.getPosition().x - timerBar.getSize().x / 2, sprite.getPosition().y - 50.f);
        if (buildTimer >= buildDuration-pluss)
        {
            isBuilding = false;

            // Po zakoñczeniu budowy wracamy na spawn
            targetPost = sf::Vector2f(4962, 2423);
            moveTarget = true;
            isMoving = true;
        }
    }

    // Animacja
    animationTimer += deltaTime;
    if (animationTimer >= frameTime)
    {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % 6;

        int rowY = 0; // Idle
        if (isMoving) rowY = 192;
        else if (isBuilding) rowY = 384;

        sprite.setTextureRect(sf::IntRect(currentFrame * 192, rowY, 192, 192));
    }
}

void Pawn::moveTo(const sf::Vector2f target)
{
    targetPost = target;
    moveTarget = true;
    isMoving = true;
}

void Pawn::moveToAndBuild(Buildings& building)
{
    targetBuilding = &building;
    hasTargetBuilding = true;
    moveTo(building.getPosition());
}


bool Pawn::isPlayerClose(const sf::Vector2f& playerPos) {
    sf::Vector2f pawnPos = sprite.getPosition();
    float dx = std::abs(playerPos.x - pawnPos.x);
    float dy = std::abs(playerPos.y - pawnPos.y);

    if (dx < 100.f && dy < 100.f)
    {
        isClose = true;
    }
    else
    {
        isClose = false;
    }

    return isClose; 
}

void Pawn::NPC(int& mission, int treeP, int goldP)
{




    // Tworzenie wykrzyknika (tekst)

    wykrzyknikT.setFont(font);
    wykrzyknikT.setString("!");
    wykrzyknikT.setCharacterSize(50); // Rozmiar tekstu
    wykrzyknikT.setFillColor(sf::Color::Red);
    wykrzyknikT.setOutlineThickness(5);
    wykrzyknikT.setOutlineColor(sf::Color::Black);
    

    kropkiT.setFont(font);
    kropkiT.setString("...");
    kropkiT.setCharacterSize(50); // Rozmiar tekstu
    kropkiT.setFillColor(sf::Color::White);
    kropkiT.setOutlineThickness(5);
    kropkiT.setOutlineColor(sf::Color::Black);
    std::cout << mission << std::endl;
    if (!isMoving && !isBuilding&& isClose)
    {


        switch (mission)
        {
        case 0:
            if (!missionCompleted[0]) {
                std::cout << "MISJA 0" << std::endl;
                wykrzyknik = true;
                missionCompleted[0] = true;  // Oznaczamy misjê jako wykonan¹
                mission++;  // Przechodzimy do kolejnej misji
            }
            break;

        case 1:
            if (!missionCompleted[1]) {
                std::cout << "MISJA 1" << std::endl;
                wykrzyknik = false;
                kropki = true;
                missionCompleted[1] = true;  // Oznaczamy misjê jako wykonan¹
                mission++;  // Przechodzimy do kolejnej misji
            }
            break;

        case 2:
          
            wykrzyknik = true;
            kropki = false;
            if (!missionCompleted[2] && treeP >= 10) {
                std::cout << "MISJA 2" << std::endl;

                //kropki = false;
                treeP -= 2;
                missionCompleted[2] = true;  // Oznaczamy misjê jako wykonan¹
                mission++;
                ;// Przechodzimy do kolejnej misji
            }
            break;

        case 3:
           
            if (!missionCompleted[3]) {
                std::cout << "MISJA 3" << std::endl;
                wykrzyknik = false;
                missionCompleted[3] = true; // Oznaczamy misjê jako wykonan¹
                mission++;// Przechodzimy do kolejnej misji
                kropki = true;
            }
            break;

        case 4:
            if (!missionCompleted[4])
            {
                wykrzyknik = true;
                kropki = false;
                std::cout << "Misja Poczatek ukonczona" << std::endl;
                std::cout << "MISJA 4" << std::endl;
                missionCompleted[4] = true;
                mission++;
            }
            break;  // Misja zakoñczona

        case 5:
            if (!missionCompleted[5])
            {
                wykrzyknik = false;
                kropki = true;
                std::cout << "MISJA 5" << std::endl;
                missionCompleted[5] = true;
                mission++;
            }
            break;  // Misja zakoñczona

        case 6:
            if (!missionCompleted[6])
            {
                wykrzyknik = false;
                kropki = true;
                //std::cout << "Misja Polnoc ukonczona" << std::endl;
                std::cout << "MISJA 6" << std::endl;
                missionCompleted[6] = true;
                mission++;
            }
            break;
        case 7:

            if (!missionCompleted[7])
            {
                std::cout << "Misja Polnoc rozpoczeta" << std::endl;
                std::cout << "MISJA 7" << std::endl;
                missionCompleted[7] = true;
            }
            break;
        case 8:
            if (!missionCompleted[8])
            {
                wykrzyknik = true;
                kropki = false;
                std::cout << "Misja Polnoc ukonczona" << std::endl;
                std::cout << "MISJA 8" << std::endl;
                missionCompleted[8] = true;
                mission++;
            }
            break;

        case 9:
            if (!missionCompleted[9])
            {
                wykrzyknik = false;
                kropki = true;
                std::cout << "Misja kopalnia rozpoczeta" << std::endl;
                std::cout << "MISJA 9" << std::endl;
                missionCompleted[9] = true;
                mission++;
            }
            break;
        case 10:

            if (!missionCompleted[10] && goldP >= 2)
            {


                std::cout << "Misja kopalnia zakonczona" << std::endl;
                std::cout << "MISJA 10" << std::endl;
                missionCompleted[10] = true;
                mission++;
            }
            break;

        case 11:

            if (!missionCompleted[11])
            {
                wykrzyknik = true;
                std::cout << "Misja " << std::endl;
                std::cout << "MISJA 11" << std::endl;
                missionCompleted[11] = true;
                mission++;
            }
            break;
        case 12:

            if (!missionCompleted[12])
            {
                wykrzyknik = false;
                kropki = true;
                std::cout << "Misja zachod rozpoczeta" << std::endl;
                std::cout << "MISJA 12" << std::endl;
                missionCompleted[1] = true;
                mission++;
            }
            break;
        case 13:

            if (!missionCompleted[13])
            {
                wykrzyknik = false;
                kropki = true;
                std::cout << "Misja zachod trwa" << std::endl;
                std::cout << "MISJA 13" << std::endl;
                missionCompleted[13] = true;
                mission++;
            }
            break;
        case 14:

            if (!missionCompleted[14])
            {
                //wykrzyknik = false;
                //kropki = true;
                std::cout << "Misja zachod trwa" << std::endl;
                std::cout << "MISJA 14" << std::endl;
                missionCompleted[14] = true;
                //mission++;
            }
            break;
        case 15:

            if (!missionCompleted[15])
            {
                wykrzyknik = true;
                kropki = false;
                std::cout << "Misja zachod zakonczona" << std::endl;
                std::cout << "MISJA 15" << std::endl;
                missionCompleted[15] = true;
                mission++;

            }
            break;
        case 16:

            if (!missionCompleted[16])
            {
                wykrzyknik = false;
                kropki = true;
                std::cout << "Misja zachod zakonczona" << std::endl;
                std::cout << "MISJA 16" << std::endl;
                missionCompleted[16] = true;
                mission++;
                //THE END
            }
            break;

        
        
        case 17:

            if (!missionCompleted[17])
            {
                //wykrzyknik = false;
                kropki = false;
                std::cout << "Koniec" << std::endl;
                std::cout << "MISJA 17" << std::endl;
                missionCompleted[17] = true;
                mission++;
                //THE END
            }
            break;


        }

    

    }
}
sf::Vector2f Pawn::getPosition() const
{
    return sprite.getPosition();
}
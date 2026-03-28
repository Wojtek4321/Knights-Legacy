#include "Camera.h"
#include <algorithm> 
#include <iostream>
#include <SFML/Graphics.hpp>
Camera::Camera(const sf::Vector2u& mapSize, const sf::Vector2u& screenSize): mapSize(mapSize), screenSize(screenSize), halfWidth(screenSize.x / 2.f), halfHeight(screenSize.y / 2.f) 
{
    view.setSize(static_cast<float>(screenSize.x), static_cast<float>(screenSize.y));
    

    // Pasek zdrowia 
    healthBarBg.setSize(sf::Vector2f(203.f, 20.f));  // Ramka
    healthBarBg.setFillColor(sf::Color::Transparent);
    healthBarBg.setPosition(60.f, 36.f);
    healthBarBg.setOutlineThickness(3.5f); // Obwodka
    healthBarBg.setOutlineColor(sf::Color::Black);
    // Wewnatrz
    healthBar.setSize(sf::Vector2f(200.f, 20.f)); // Zdrowie wewnatrz
    healthBar.setFillColor(sf::Color(200, 0, 0)); // Czerwony  
    healthBar.setPosition(60.f, 35.f);

    
    if (!heartTexture.loadFromFile("assets/TekU/Ui/hearts128.png")) {
        // Obs³uga b³êdu, jeœli ikona nie zostanie za³adowana
        std::cerr << "B³¹d podczas ³adowania ikony serca!" << std::endl;
    }

    // ikona sprite
    heartIcon.setTexture(heartTexture);
    heartIcon.setScale(0.5,0.5);
    heartIcon.setPosition(5.f, 0.f);
    
    
    //czcionka dla wszystkich
    if (!font.loadFromFile("assets/TekU/SMW Text 2 NC.ttf")) {
        std::cerr << "B³¹d ³adowania czcionki!" << std::endl;
    }


    //Item Ui
    if (!miesoTexture.loadFromFile("assets/TekU/Items/M_Idle.png")) {
        std::cerr << "B³¹d ³adowania mieso.png" << std::endl;
    }
    if (!drzewaTexture.loadFromFile("assets/TekU/Items/W_Idle.png")) {
        std::cerr << "B³¹d ³adowania drzewa.png" << std::endl;
    }
    if (!goldTexture.loadFromFile("assets/TekU/Items/G_Idle.png")) {
        std::cerr << "B³¹d ³adowania gold.png" << std::endl;
    }


    
    if (!Eq.loadFromFile("assets/TekU/Ui/butt.png")) {

        std::cerr << "Blad ladowania miejsca itemow" << std::endl;
    }



    Eqs.setTexture(Eq);
    Eqs1.setTexture(Eq);
    Eqs2.setTexture(Eq);
}

void Camera::update(const sf::Vector2f& playerPosition,float playerHealth) {
   
    float cameraX = std::min(std::max(playerPosition.x, halfWidth), static_cast<float>(mapSize.x) - halfWidth);
    float cameraY = std::min(std::max(playerPosition.y, halfHeight), static_cast<float>(mapSize.y) - halfHeight);

    view.setCenter(cameraX, cameraY);
    setHealth(playerHealth);
}
void Camera::setBossHelath(float hpb, sf::RenderWindow& window)
{

    if (hpb < 1000 )
    {

        bossHP = hpb;

        backgroundBar.setSize(sf::Vector2f(400.f, 25.f));
        backgroundBar.setFillColor(sf::Color(50, 50, 50, 200));
        backgroundBar.setPosition((1920.f - 400.f) / 2.f, 100.f); // na œrodku u góry

        // Pasek zdrowia
        float healthPercent = bossHP / bossMaxHP;
        healthBarboss.setSize(sf::Vector2f(400.f * healthPercent, 25.f));
        healthBarboss.setFillColor(sf::Color::Red);
        healthBarboss.setPosition((1920.f - 400.f) / 2.f, 100.f);
        hpBoss = true;
    }

    if (hpBoss)
    {
        bossHP = hpb; // aktualizuj jego HP

        // T³o paska

    }
    if (hpb <= 0) {
        hpBoss = false;
    }

 
}

void Camera::apply(sf::RenderWindow& window) 
{
    window.setView(view);
}
void Camera::drawUI(sf::RenderWindow& window, int sheepCount, int treecount, int goldCount) {
    
    
    
    // Domyslna cmaera
    window.setView(window.getDefaultView());

   
    window.draw(healthBarBg);

    
    window.draw(healthBar);


    for (int i = 0; i <= 100; i += 10) // Linia co 10
    { 
        sf::RectangleShape tick(sf::Vector2f(2.f, 20.f)); // Kreska
        tick.setFillColor(sf::Color::Black); // 
        tick.setOutlineThickness(1.5f); // Gruboœæ 
        tick.setOutlineColor(sf::Color::Black);

        tick.setPosition(60 + i * 2, 35.f); // Ustawienie wzgledem paska
        window.draw(tick);
        
    }
 
    if (hpBoss)
    {

    
    window.draw(backgroundBar);
    window.draw(healthBarboss);
    }
    window.draw(heartIcon);
    
    ItemUI(sheepCount, treecount, goldCount,window);

    


    
}


void Camera::setHealth(float hp) 
{
    //static float hpfull = hp;


    health = std::max(0.f, hp); // od 0 do 100
    healthBar.setSize(sf::Vector2f(2.f * health, 20.f)); 
    if (health > 70) {
        healthBar.setFillColor(sf::Color::Green); // Zielony
    }
    else if (health > 50)
    {
        healthBar.setFillColor(sf::Color::Yellow); // zolty 
    }
    else if (health > 20) {
        healthBar.setFillColor(sf::Color(255, 165, 0)); // pomarañczowy
    }
    else {
        healthBar.setFillColor(sf::Color::Red); // czerwony

    }

}

void Camera::ItemUI(int sheepCount, int treecount,int goldCount, sf::RenderWindow& window)
{
   

    Eqs.setPosition(1750.f, 930.f);
    Eqs1.setPosition(1600.f, 930.f);
    Eqs2.setPosition(1450.f, 930.f);
    Eqs.setScale(0.8f,0.8f);
    Eqs1.setScale(0.8f ,0.8f);
    Eqs2.setScale(0.8f, 0.8f);


    int miesoCount = 0;
    int drzewaCount = 0;
    int zlotoCount = 0;
    // Liczymy ile mamy ka¿dego przedmiotu
    miesoCount = sheepCount;
    drzewaCount = treecount;
    zlotoCount = goldCount;


    window.draw(Eqs);
    window.draw(Eqs1);
    window.draw(Eqs2);




 

    bossText.setFont(font);
    bossText.setCharacterSize(80);
    bossText.setFillColor(sf::Color::White);
    bossText.setString("Boss HP");
    bossText.setOutlineThickness(3.5f); // Obwodka
    bossText.setOutlineColor(sf::Color::Black);
    bossText.setPosition((1920.f - 400.f) / 2.f, 0.f);

  
    licznikText.setFont(font);
    licznikText.setCharacterSize(20);
    licznikText.setFillColor(sf::Color::White);
    if (miesoCount > 0) 
    {
        iconSprite.setTexture(miesoTexture);
        iconSprite.setPosition(1390.f, 835.f);
        iconSprite.setScale(2, 2);
        window.draw(iconSprite);

        licznikText.setString("x" + std::to_string(miesoCount));
        licznikText.setPosition(1520, 1030);  // lekko przesuniête
        window.draw(licznikText);
    }

    if (drzewaCount > 0) {
        iconSprite.setTexture(drzewaTexture);
        iconSprite.setPosition(1550.f, 830.f);
        iconSprite.setScale(2, 2);
        window.draw(iconSprite);

        licznikText.setString("x" + std::to_string(drzewaCount));
        licznikText.setPosition(1660.f, 1030 );
        window.draw(licznikText);
    }
    if (zlotoCount > 0) {
        iconSprite.setTexture(goldTexture);
        iconSprite.setPosition(1690.f, 840.f);
        iconSprite.setScale(2, 2);
        window.draw(iconSprite);

        licznikText.setString("x" + std::to_string(zlotoCount));
        licznikText.setPosition(1820.f, 1030);
        window.draw(licznikText);
    }


    if (hpBoss)
    {
        window.draw(bossText); //do poprawy nie wiadomo czemu musi byc tu
    }
  
   
}

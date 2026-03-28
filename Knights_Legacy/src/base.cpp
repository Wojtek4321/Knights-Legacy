// Knight's Legacy.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <SFML/System.hpp>
#include "Camera.h"

#include "Collision.h"
#include <fstream>
#include <string>
#include <sstream>


#include "Menu.h"
#include "enemy.h"


#include <SFML/Audio.hpp>

#include "Buildings_Story.h"
#include "dialog.h"

#include <future>

enum GameState { MENU, GAME,SETTINGS,CONF,RANKING};

void printPosition(const sf::Vector2f& position) {
    
  //std::cout << "Pozycja gracza: (" << position.x << ", " << position.y << ")\n";
}

struct Mapa
{
    sf::Texture lay1, lay2, lay3;
    bool loadAll()
    {
        return 
            lay1.loadFromFile("assets/TekU/Maps/Glowna mapa/V8glownaMap1.png") &&
            lay2.loadFromFile("assets/TekU/Maps/Glowna mapa/V8glownaMap2.png") &&
            lay3.loadFromFile("assets/TekU/Maps/Glowna mapa/V8glownaMapdodatki.png");
    }
};

int main()
{
    
    

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Knight's Legacy");  //bylo 1280
    Menu menu(1920, 1280);

    window.setFramerateLimit(250);    //rozwiazuje problem z dawanmie itemu i spowalnianuiem
    GameState state = MENU;
    static int volume = 50;          //pocz glos
    static int selectedOption = 0;  // menu select


    //int mision = 0;//WAZNE!!! 
    missionOperation missions;
    int mission = missions.getMission();
    bool m3Block = false;
    bool m4Block = false;
    bool m6Block = false;
    bool m7Block = false;
    bool m9Block = false;
    bool m10Block = false;
    bool m11Block = false;
    bool m12Block = false;
    bool m13Block = false;
    bool m14Block = false;
    bool m16Block = false;
    bool m17Block = false;

    int selectknight = 0; // skin knight

    int currentBlock = 0;
    bool confstate = false;

    sf::Clock clock;            
    sf::Clock gameClock;        //czasomierz
    

    //camera
    sf::Vector2u mapSize(6144, 4096);           
    sf::Vector2u screenSize(1920, 1280);
    Camera camera(mapSize, screenSize);

    //PLAYERTRR

    Player rycerz;
    rycerz.dead("assets/TekU/Kn/Dead.png");
    //map 
    
     //warstwa 0
    sf::Texture waterTexture;
    if (!waterTexture.loadFromFile("assets/TekU/Mp/water.png")) {
        return -1;
    }
    sf::Sprite background(waterTexture);
    background.setScale(
        (6144.f) / waterTexture.getSize().x, // poziom
        (4096.f) / waterTexture.getSize().y  // PION
    );
    /*
    //warstwa 1
    sf::Texture mapTexture1;
    if (!mapTexture1.loadFromFile("assets/TekU/Maps/Glowna mapa/V8glownaMap1.png")) {
        return -1;
    }
    sf::Sprite mapSprite1(mapTexture1);
    //warstwa 2
    sf::Texture mapTexture2;
    if (!mapTexture2.loadFromFile("assets/TekU/Maps/Glowna mapa/V8glownaMap2.png")) {
        return -1;
    }
    sf::Sprite mapSprite2(mapTexture2);
    //warstwa 3 dodatki
    sf::Texture mapTexture3;
    if (!mapTexture3.loadFromFile("assets/TekU/Maps/Glowna mapa/V8glownaMapdodatki.png")) {
        return -1;
    }
    sf::Sprite mapSprite3(mapTexture3);


    // nie zadowalacjy efekt
   /* sf::Texture mapTexture4;
    if (!mapTexture4.loadFromFile("assets/TekU/Mp/testy/polnoc.png")) {
        return -1;
    }
    sf::Sprite mapSprite4(mapTexture4);

    sf::Texture mapTexture5;
    if (!mapTexture5.loadFromFile("assets/TekU/Mp/testy/kopalnia.png")) {
        return -1;
    }
    sf::Sprite mapSprite5(mapTexture5);

    sf::Texture mapTexture6;
    if (!mapTexture6.loadFromFile("assets/TekU/Mp/testy/zachod.png")) {
        return -1;
    }
    sf::Sprite mapSprite6(mapTexture6);
    */
    
    //brama polnoc zamknieta
    bool brama1 = false;

    std::future<Mapa> mapFuture = std::async(std::launch::async, []() {
        Mapa map;
        if (!map.loadAll()) {
            std::cerr << "Nie zaladowano mapy";
        }
        return map;
        });

    sf::Texture gate1block;
    if (!gate1block.loadFromFile("assets/TekU/object/BRAMY/b1-block/b1b.png")) {
        return -1;
    }
    sf::Sprite gate1b(gate1block);
    gate1b.setPosition(4855.89, 1364.99);
    gate1b.setScale(0.4, 0.4);
    //brama polnoc otwara
    sf::Texture gate1openA;
    if (!gate1openA.loadFromFile("assets/TekU/object/BRAMY/b1open/b1-around.png")) {
        return -1;
    }
    sf::Sprite gate1oA(gate1openA);
    gate1oA.setPosition(4855.89, 1364.99);
    gate1oA.setScale(0.4, 0.4);
    sf::Texture gate1openB;
    if (!gate1openB.loadFromFile("assets/TekU/object/BRAMY/b1open/gate.png")) {
        return -1;
    }
    sf::Sprite gate1oB(gate1openB);
    gate1oB.setPosition(4948.49, 1364.99);
    gate1oB.setScale(0.4, 0.4);

    //barama zachód
    bool brama2 = false;
    sf::Texture gate2block;
    if (!gate2block.loadFromFile("assets/TekU/object/BRAMY/b2block/brammy2block.png")) {
        return -1;
    }
    sf::Sprite gate2b(gate2block);
    gate2b.setPosition(3305.79f, 2330.92f);
    gate2b.setScale(0.45, 0.5);
    sf::Texture gate2open;
    if (!gate2open.loadFromFile("assets/TekU/object/BRAMY/b2open/brammy2open2.png")) {
        return -1;
    }
    sf::Sprite gate2o(gate2open);
    gate2o.setPosition(3305.79f, 2330.92f);
    gate2o.setScale(0.45, 0.5);
    //kolizja 

    std::vector<Collision::Object> objects = Collision::parseXML("assets/TekU/Maps/Glowna mapa/collision/Ost/kolizjaWERSJAOS.xml");
    

   

    

    //enemy 

    std::vector<sf::Texture> textures(4);  // 3 tekstury
    textures[0].loadFromFile("assets/TekU/En/1.png");
    textures[1].loadFromFile("assets/TekU/En/1p.png");
    textures[2].loadFromFile("assets/TekU/En/1r.png");
    textures[3].loadFromFile("assets/TekU/En/1y.png");


    std::vector<Enemy> enemies;

    //POLNOC
    enemies.emplace_back(4476, 630.88, 0.3f, 70.0f, &textures[0], 100); //polnoc 6 
    enemies.emplace_back(4350.05, 826, 0.3f, 70.0f, &textures[1], 150);
    enemies.emplace_back(5614, 776, 0.3f, 70.0f, &textures[0], 100);
    enemies.emplace_back(5073,536, 0.3f, 70.0f, &textures[3], 200);
    enemies.emplace_back(5764, 1061, 0.3f, 70.0f, &textures[0], 100);
    enemies.emplace_back(5400, 736, 0.3f, 70.0f, &textures[1], 150);

    enemies.emplace_back(3600.18, 1971.25, 0.3f, 70.0f, &textures[0], 100);  //pilnuje kopalni

    enemies.emplace_back(2484.07, 1820.41, 0.3f, 70.0f, &textures[1], 150);
    enemies.emplace_back(2364.33, 2205.79, 0.3f, 70.0f, &textures[1], 150);
    enemies.emplace_back(2475.66, 2506.17, 0.3f, 70.0f, &textures[1], 150);
    enemies.emplace_back(2843.27, 2286.66, 0.3f, 70.0f, &textures[1], 150);
    enemies.emplace_back(2806.51, 2833.86, 0.3f, 70.0f, &textures[1], 150);
    enemies.emplace_back(2522.93, 1444.54, 0.3f, 70.0f, &textures[3], 200);
    enemies.emplace_back(3187.76, 1093.86, 0.3f, 70.0f, &textures[3], 200);
    enemies.emplace_back(3415.68, 1381.54, 0.3f, 70.0f, &textures[3], 200);
    enemies.emplace_back(2730.89, 1220.9, 0.07f, 200.0f, &textures[2], 1000);//zachod


   
    // Tworzenie drzewa
    std::vector<Tree> trees;
    std::vector<sf::Texture> textureT(1);  // 
    textureT[0].loadFromFile("assets/TekU/mobs/Tree.png");
    
    trees.emplace_back(5009.05f, 3005.88f, 100, &textureT[0], objects);
    trees.emplace_back(5309.05f, 3055.88f, 100, &textureT[0], objects);
    trees.emplace_back(5209.05f, 3045.88f, 100, &textureT[0], objects);
    trees.emplace_back(5109.05f, 3015.88f, 100, &textureT[0], objects);
    trees.emplace_back(4909.05f, 3035.88f, 100, &textureT[0], objects);
    trees.emplace_back(4809.05f, 3055.88f, 100, &textureT[0], objects);
    trees.emplace_back(4709.05f, 3025.88f, 100, &textureT[0], objects);
    trees.emplace_back(4609.05f, 3035.88f, 100, &textureT[0], objects);
    trees.emplace_back(4509.05f, 3015.88f, 100, &textureT[0], objects);
    trees.emplace_back(4409.05f, 3005.88f, 100, &textureT[0], objects);
    trees.emplace_back(4309.05f, 3055.88f, 100, &textureT[0], objects);
    trees.emplace_back(4209.05f, 3045.88f, 100, &textureT[0], objects);
    trees.emplace_back(4109.05f, 3015.88f, 100, &textureT[0], objects);
    trees.emplace_back(4009.05f, 3035.88f, 100, &textureT[0], objects);
    trees.emplace_back(3909.05f, 3055.88f, 100, &textureT[0], objects);
    trees.emplace_back(3809.05f, 3025.88f, 100, &textureT[0], objects);
    trees.emplace_back(3759.05f, 3065.88f, 100, &textureT[0], objects);
    trees.emplace_back(4213.55, 2395.51, 100, &textureT[0], objects);
    trees.emplace_back(4230.64, 2082.86, 100, &textureT[0], objects);
    trees.emplace_back(4017.72, 1364.54, 100, &textureT[0], objects);
    trees.emplace_back(5659.21, 1721, 100, &textureT[0], objects);

    // OWCA
    std::vector<Sheep> sheeps;
    std::vector<sf::Texture> textureS(2);  // 3 tekstury
    textureS[0].loadFromFile("assets/TekU/mobs/sheepidle.png");
    textureS[1].loadFromFile("assets/TekU/mobs/GoldMine_Destroy.png");

    sheeps.emplace_back(4993.34, 2610.65, 50, &textureS[0], objects);
    sheeps.emplace_back(4395.23, 2496.33, 50, &textureS[0],  objects);
    sheeps.emplace_back(3798.08, 2679.06, 50, &textureS[0], objects);
    sheeps.emplace_back(4211.15, 1557.48, 50, &textureS[0], objects);
    sheeps.emplace_back(5124.96, 1760.91, 50, &textureS[0], objects);
    //kopalnie
    std::vector<GoldMine> mines;
    //mines.emplace_back(4309, 3465.88f, 10, &textureS[1], objects);
    mines.emplace_back(3535.01, 1912.61, 10, &textureS[1], objects);
    
    
  //blokady 
    sf::Texture skrzynki;
    skrzynki.loadFromFile("assets/TekU/object/BRAMY/skrzynka.png");
    sf::Sprite sk;
    sk.setTexture(skrzynki);
    sk.setScale(0.11, 0.125);
    sk.setPosition(4000.7f, 1749.2f);
   

    //brama zachod otworzona
    Blokada collisionArea4(3325.44f, 2430.92f, 3421.4f, 2515.7f);
    Blokada collisionArea5(3334.4f, 2675.92f, 3388.4f, 2778.24f);
    Collision::addObject(objects, 3325.44f, 2430.92f, 96, 85);
    Collision::addObject(objects, 3334.4f, 2675.92f, 54, 103);

    //brama polnoc otworzona
    Blokada collisionArea6(4861.88, 1366.0f, 4935.2f, 1550.79f);
    Blokada collisionArea7(5067.67f, 1366.0f, 5140.2f, 1550);
    Collision::addObject(objects, 4861.88, 1366.0f, 74, 184);
    Collision::addObject(objects, 5067.67f, 1366.0f, 74, 184.0f);

    //b3 brama zachod zamknieta
    Blokada collisionArea3(3325.44f, 2430.92f, 3451.4f, 2778.24f);  
    Collision::addObject(objects, 3325.44f, 2430.92f, 125.96, 348.0f);
    

    //b1
    Blokada collisionArea1(4000.7f, 1749.2f, 4128.7f, 1870.2f);
    Collision::addObject(objects, 4000.7f, 1749.2f, 128, 128.0f);   //(objects, 3943.7f, 1776.2f, 400.0f, 60.0f);

    //b2 brama polnoc zamknieta
    Blokada collisionArea2(4865.67f, 1366.0f, 5130.2f, 1520.79f);
    Collision::addObject(objects, 4865.67f, 1366.0f, 264, 154);
    

    
    //muzyka 
    sf::Music menusmuisc;
    if (!menusmuisc.openFromFile("assets/TekU/music/BackMusicKrotko.mp3")) {
        return -1; // Błąd ładowania pliku
    }
    menusmuisc.setLoop(true);
    menusmuisc.play();
    menusmuisc.setVolume(10.f);

    bool isButtonPressed = false;
    bool methodCalled = false;
    bool methodCalled1 = false;
    bool methodCalled2 = false;
    bool methodCalled3 = false;
    



    std::vector<sf::Texture> texturesBuild(9);
    texturesBuild[0].loadFromFile("assets/TekU/object/budynki/TD.png");
    texturesBuild[1].loadFromFile("assets/TekU/object/budynki/TB.png");
    texturesBuild[2].loadFromFile("assets/TekU/object/budynki/TS.png");
    texturesBuild[3].loadFromFile("assets/TekU/object/budynki/CD.png");
    texturesBuild[4].loadFromFile("assets/TekU/object/budynki/CB.png");
    texturesBuild[5].loadFromFile("assets/TekU/object/budynki/CS.png");
    texturesBuild[6].loadFromFile("assets/TekU/object/budynki/HD.png");
    texturesBuild[7].loadFromFile("assets/TekU/object/budynki/HB.png");
    texturesBuild[8].loadFromFile("assets/TekU/object/budynki/HS.png");

    std::vector<Buildings> buildings;

    buildings.emplace_back(5308, 1786, 100, &texturesBuild[0], &texturesBuild[1], &texturesBuild[2], 15,false);     //tower1    
    buildings.emplace_back(5244, 2340, 100, &texturesBuild[0], &texturesBuild[1], &texturesBuild[2], 15, false);       //tower2
    buildings.emplace_back(5505, 1848, 100, &texturesBuild[3], &texturesBuild[4], &texturesBuild[5], 15, false);       //budenk1
    buildings.emplace_back(5613.24, 2240, 100, &texturesBuild[6], &texturesBuild[7], &texturesBuild[8], 15, false); //budenk 2
    buildings.emplace_back(5379, 2101, 100, &texturesBuild[6], &texturesBuild[7], &texturesBuild[8],15, false);  //castle


    sf::Texture pawnTexture;

    pawnTexture.loadFromFile("assets/TekU/object/NPC/PB.png");
    Pawn myPawn(4962, 2423, &pawnTexture, 100.f);


    std::vector<sf::Texture> texturesBuildENE(5);
    texturesBuildENE[0].loadFromFile("assets/TekU/object/budynki/ENE/GTD.png");
    texturesBuildENE[1].loadFromFile("assets/TekU/object/budynki/ENE/GTB.png");
    texturesBuildENE[2].loadFromFile("assets/TekU/object/budynki/ENE/GTS.png");
    texturesBuildENE[3].loadFromFile("assets/TekU/object/budynki/ENE/GD.png");
    texturesBuildENE[4].loadFromFile("assets/TekU/object/budynki/ENE/GS.png");

    std::vector<Buildings> buildene;

    

    buildene.emplace_back(4717.34, 588.147, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true); //domek  enemy 6 
    buildene.emplace_back(4521.64, 1014.07, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true);
    buildene.emplace_back(5670.66, 557.35, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true);
    buildene.emplace_back(5591.88, 1070.76, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true);
    buildene.emplace_back(5272.59, 763.135, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true);
    buildene.emplace_back(5861.81, 837.68, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true);

    buildene.emplace_back(3705.21, 1726.62, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true); // przy kopalni 

    buildene.emplace_back(2484.07, 1820.41, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true);
    buildene.emplace_back(2364.33, 2205.79, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true);
    buildene.emplace_back(2475.66, 2506.17, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true);
    buildene.emplace_back(2843.27, 2286.66, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true);
    buildene.emplace_back(2806.51, 2833.86, 100, &texturesBuildENE[3], &texturesBuildENE[1], &texturesBuildENE[4], 0, true);
    buildene.emplace_back(2522.93, 1444.54, 200, &texturesBuildENE[0], &texturesBuildENE[1], &texturesBuildENE[2], 0, true); //tower enemy
    buildene.emplace_back(3187.76, 1093.86, 200, &texturesBuildENE[0], &texturesBuildENE[1], &texturesBuildENE[2], 0, true); //tower enemy
    buildene.emplace_back(3415.68, 1381.54, 200,&texturesBuildENE[0], &texturesBuildENE[1], &texturesBuildENE[2], 0, true); //tower enemy


    bool deathflag = true;

    Dialog dialog;

    //dodatki
    sf::Vector2f spawnposbos(2730.89, 1220.9); //Miesje spawnu bossa


    //Glowna ob
    myPawn.NPC(mission, rycerz.treeitem1(), rycerz.golditem1());


    bool mapReady = false;
    Mapa mapData;
    sf::Sprite  layer1, layer2, layer3;

    
    while (window.isOpen()) {
        sf::Event event;

        if (!mapReady && mapFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            mapData = mapFuture.get(); // odbieramy dane

          

            layer1.setTexture(mapData.lay1);
            layer2.setTexture(mapData.lay2);
            layer3.setTexture(mapData.lay3);

            mapReady = true;
        }
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            {
                if (myPawn.isPlayerClose(rycerz.getPosition()) == true)
                {
                    myPawn.NPC(mission, rycerz.treeitem1(), rycerz.golditem1());
                    //if (!dialog.isFinished()) 
                    //{

                    dialog.skipOrNext();
                    //}
                }
                
            }
            if (mission == 3&& m3Block==false)
            {
                rycerz.decreaseTreeItem(10);
               myPawn.moveToAndBuild(buildings[3]);
               
                m3Block = true;
            }
            if(mission ==4&& m4Block == false)
            {
               myPawn.moveToAndBuild(buildings[4]);
                m4Block = true;
                
            }
            if (mission == 6&& m6Block==false)
            {
                Collision::removeLastObject(objects);
                std::cout << " removeLastObject pierwszy block" << std::endl;
                collisionArea2.unlockCollision();
                
                currentBlock = 1;

                m6Block = true;
            }
            if (mission == 7 && m7Block == false  )
            {
                int wszyscy = 0;
                for (int i = 0; i < 6; i++)
                {
                    if (enemies[i].FullDead() == true)
                    {
                        wszyscy++;
                    }

                }
                if (wszyscy == 6)
                {
                    mission++;
                    m7Block = true;
                }
                
            }
            if (mission == 9 && m9Block == false)
            {
                Collision::removeLastObject(objects);
                std::cout << " removeLastObject drugi block" << std::endl;
                collisionArea1 .unlockCollision();
                
                currentBlock = 2;
                m9Block = true;
            }
            if (mission == 11 && m11Block == false)
            {
                rycerz.decreaseGoldItem(2);
                
                myPawn.moveToAndBuild(buildings[0]);
                
                m11Block = true;

            }
            if (mission == 12 && m12Block == false)
            {
                
                
                myPawn.moveToAndBuild(buildings[1]);
                //mission++;
                m12Block = true;

            }
            if (mission == 13 && m13Block == false)
            {
                Collision::removeLastObject(objects);
                std::cout << " removeLastObject  drugi block" << std::endl;
                collisionArea3.unlockCollision();

                Collision::removeLastObject(objects);
                std::cout << " removeLastObject  drugi block" << std::endl;
                collisionArea3.unlockCollision();
                Collision::removeLastObject(objects);
                std::cout << " removeLastObject  drugi block" << std::endl;
                collisionArea3.unlockCollision();

                
                currentBlock = 3;
                m13Block = true;
            }
            if (mission == 14 && m14Block == false)
            {


                int wszyscy = 0;
                for (int i = 0; i < 16; i++)
                {
                    if (enemies[i].FullDead() == true)
                    {
                        wszyscy++;
                    }

                }
                if (wszyscy == 16)
                {
                    mission++;
                    m14Block = true;
                }

            }
            if (mission == 16 && m16Block == false)
            {


                myPawn.moveToAndBuild(buildings[2]);
                
                m16Block = true;

            }
            
            if (state == MENU) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                        menu.moveUp();
                    }
                    else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                        menu.moveDown();
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        if (menu.getSelectedIndex() == 0) {
                            state = CONF;

                        }
                        else if (menu.getSelectedIndex() == 1) {
                            state = SETTINGS;
                        }
                        else if (menu.getSelectedIndex() == 2) {
                            window.close();
                        }
                    }
                    else if(event.key.code == sf::Keyboard::Tab)
                    {
                        
                        state = RANKING;
                    }
                }
            }
            else if (state == SETTINGS) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down) {
                        selectedOption = (selectedOption == 0) ? 1 : 0;
                    }
                    else if (event.key.code == sf::Keyboard::Left && selectedOption == 0) {
                        volume = std::max(0, volume - 5);
                    }
                    else if (event.key.code == sf::Keyboard::Right && selectedOption == 0) {
                        volume = std::min(100, volume + 5);
                    }
                    else if (event.key.code == sf::Keyboard::Enter && selectedOption == 1) {
                        state = MENU;
                    }
                    else if (event.key.code == sf::Keyboard::Escape) {
                        state = MENU;
                    }
                }
                menusmuisc.setVolume(volume);
                
            }

            else if (state == CONF)
            {

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Left)
                    {
                        if (selectknight > 0)
                        {
                            selectknight--;
                        }
                        else
                        {
                            selectknight = 3;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Right)
                    {
                        if (selectknight < 3)
                        {
                            selectknight++;
                        }
                        else
                        {
                            selectknight = 0;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        rycerz.loadTextures(selectknight);
                        std::cout << "zaladowanie tesktrua" << std::endl;
                        state = GAME;

                    }
                    else if (event.key.code == sf::Keyboard::Escape) {
                        state = MENU;
                    }

                  
                }

            }
            else if (state == RANKING)
            {

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape) {
                        state = MENU;
                    }
                }


            }
            
            if (confstate == true)
            {

            
            std::cout << "spacja" << std::endl;
            menu.inputing(event,confstate);
            }
            confstate = (state == CONF);
        }

        //gra
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!methodCalled) {
                
                std::cout << "uderzenie" << std::endl;
                rycerz.hits(enemies, trees, sheeps, objects,mines,buildene);
                methodCalled = true;
            }
        }
        else {
            
            methodCalled = false;
        }

        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
            if (!methodCalled1) {
                
                std::cout << "ITEM1" << std::endl;
                rycerz.uzyjItem(1);
                methodCalled1 = true;
            }
        }
        else {
            
            methodCalled1 = false;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
            if (!methodCalled2) {
                
                std::cout << "ITEM2" << std::endl;
                rycerz.uzyjItem(2);
                methodCalled2 = true;
            }
        }
        else {
            
            methodCalled2 = false;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
            if (!methodCalled3) {
                
                std::cout << "ITEM3" << std::endl;
                rycerz.uzyjItem(3);
                methodCalled3 = true;
            }
        }
        else {
            
            methodCalled3 = false;
        }
        float deltaTime = clock.restart().asSeconds();


        
        myPawn.update(deltaTime, mission, rycerz.treeitem1(), rycerz.golditem1());
        
        rycerz.update(deltaTime, objects,trees,menu.getNick());
        
        


       
        

        camera.update(rycerz.getPosition(), rycerz.getHealth());
        rycerz.poruszanie(objects);
        //torch.setPosition((1920/2)-100,(1080/2)-100);
        printPosition(rycerz.getPosition());
        
        if (state == GAME)
        {

            dialog.updateDialogState(mission, myPawn.isPlayerClose(rycerz.getPosition()));
                // Gracz wchodzi w obszar
            dialog.update();
            for (Buildings& build1 : buildene) {
                build1.update();
            }
            
            for (Buildings& build : buildings) {
                build.update();
            }
            
            for (Tree& tree : trees) {
                tree.update(objects);
            }
            for (Sheep& sheep : sheeps) {
                sheep.update(rycerz.getPosition(),objects);
            }
            for (std::size_t i = 0; i < enemies.size()-1; i++) {
                enemies[i].update(rycerz.getPosition(), rycerz.getHealth(), objects);
                enemies[i].setRespawnAvailable(buildene[i].canRespawn(), buildene[i].getPosition());

                if (buildene[12].canRespawn() == false && buildene[14].canRespawn() == false && buildene[13].canRespawn() == false)
                {
                    bool wiezeZniszczone = false;
                    enemies[15].setRespawnAvailable(wiezeZniszczone, spawnposbos);
                }
                else
                {
                    bool wiezeZniszczone = true;
                    enemies[15].setRespawnAvailable(wiezeZniszczone, spawnposbos);
                }
                enemies[15].update(rycerz.getPosition(), rycerz.getHealth(), objects);
                
             
                
            }
            for (GoldMine& mine : mines) {
                mine.update();

            }
            
            float elapsedTime = gameClock.getElapsedTime().asSeconds();
            
        
            if (rycerz.getHealth() <=0 && deathflag==true)
            {
                rycerz.dead("assets/TekU/Kn/dead.png");
                deathflag = false;
                int minutes = elapsedTime / 60;
                int seconds = (int)elapsedTime % 60;  //ucina po ,1234567
                std::cout << seconds << "SEKUNDY" << minutes << "CZas" << std::endl;
                menu.Ranking(window, seconds, minutes,false,mission);
            }
            
        }
       
       
        window.clear();


       
        if (state == MENU) {
            window.draw(background);
            menu.draw(window, 1850);
        }
        else if (state == SETTINGS) {
            window.draw(background);
            menu.settings(window, selectedOption, volume);
        }
        else if (state == CONF)
        {
            menu.confi(window,selectknight);
        }
      
        else if (state == GAME) {
            //state = RANKING;
            
            
            //Mapa background
            camera.apply(window);
            window.draw(background);
            /*
            window.draw(mapSprite2);
            window.draw(mapSprite1);
            window.draw(mapSprite3);
            */
            window.draw(layer2);
            window.draw(layer1);
            
            window.draw(layer3);
            
            
            
            //mapa obiekty
            for (Buildings& build1 : buildene) {
                build1.draw(window);
            }
            
            for (Buildings& build : buildings) {
                build.draw(window);
            }
            
            for (Tree& tree : trees) {
                 
                tree.draw(window);  // Rysowanie drzewa
            }
            for (Sheep& sheep : sheeps) {
                sheep.draw(window);
            }
            for (GoldMine& mine : mines) {
                mine.draw(window);
            }
            myPawn.draw(window);
            for (Enemy& enemy : enemies) {
                enemy.draw(window);
            }

            //blokady mosty bramy
            //brama  most
            if (currentBlock == 0|| currentBlock == 1)
            {
                window.draw(sk);
            }
               //blokowane polnoc
            if (currentBlock == 0)
            {
                window.draw(gate1b);
            }
            window.draw(gate2b); 

            if (currentBlock == 1 || currentBlock == 2 || currentBlock == 3)
            {
                window.draw(gate1oA);
                
            }

            //glowna postac
            rycerz.draw(window);
            window.draw(menu.getNick());

            window.draw(gate1oB);


            if (currentBlock == 3)
            {
                window.draw(gate2o);
            }
            
            /*
            //Dodatki
            
            if (currentBlock == 0)
            {
                window.draw(mapSprite4);
            }
            if (currentBlock == 1 || currentBlock == 0)
            {
                window.draw(mapSprite5);
            }
            if (currentBlock == 0 || currentBlock == 1 || currentBlock == 2)
            {
                window.draw(mapSprite6);
            }
            */
        
           //blokady rys
           // collisionArea1.draw(window);
            //collisionArea2.draw(window);
            //collisionArea3.draw(window);
          // Collision::renderObjects(window, objects); // czerw
            

            //UI

            camera.setBossHelath(enemies[15].getHealth(), window);
            camera.drawUI(window, rycerz.sheepitem1(), rycerz.treeitem1(), rycerz.golditem1());
            dialog.draw(window);
            


            if (rycerz.getHealth() <=0 )
            {

                menu.gameover(window);
                
               
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                    
                        state = RANKING;
                    }
                    

            }
            if (m17Block == false && mission==18)
            {
                menu.gameWin(window);
            }
        }
        else if (state == RANKING)
        {
            menu.Ranking(window, 1, 1,true,mission);
        }
        window.display();
    }
}

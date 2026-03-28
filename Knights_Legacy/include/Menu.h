#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Menu {
public:
    Menu(float width, float height);
    void draw(sf::RenderWindow& window, float width);
    void moveUp();
    void moveDown();
    int getSelectedIndex();
    void gameover(sf::RenderWindow& window);
    void Pause(sf::RenderWindow& window);
    void settings(sf::RenderWindow& window, int selectedOption, int volume);
    void confi(sf::RenderWindow& window, int selectknight);
    void inputing(sf::Event& event, bool confstate);
    sf::Text& getNick();
    void Ranking(sf::RenderWindow& window, int seconds,int minutesM, bool menu,int missions);
    void gameWin(sf::RenderWindow& window);
private:
    sf::Font font;
    sf::Text menu[3]; // Play, Settings, Exit
    sf::Text tytul;
    sf::Texture titleTexture;
    sf::Texture arrowTexture;
    sf::Sprite titleSprite;
    sf::Sprite arrowSprite;
    int selectedItemIndex;
   
    sf::Sprite gameOver;
    sf::Texture gameOvertex;
    sf::Text gameOverTxt;

    sf::Text pausetxt;


    //conf
    sf::Texture K1;
    sf::Texture K2;
    sf::Texture K3;
    sf::Texture K4;

    sf::Sprite K1S;
    sf::Sprite K2S;
    sf::Sprite K3S;
    sf::Sprite K4S;

    sf::Texture Pole;
    sf::Sprite PoleS;

    sf::Texture PoleI;
    sf::Sprite PoleSI;

    sf::RectangleShape textbox;
    sf::Text textinput;
    std::string input;
    bool click;
    bool gameSaved = false;
    

};
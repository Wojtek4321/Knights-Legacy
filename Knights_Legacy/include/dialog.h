#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Dialog
{
private:
    std::vector<std::string> messages;
    sf::Font font;
    sf::Text speakerText, messageText;
    sf::RectangleShape background;
    std::string speaker = "Pierog";

    int currentIndex = 0;
    bool active = false;
    int currentMission = -1;

    sf::Clock letterClock;
    sf::Clock autoAdvanceClock;
    float letterDelay = 0.03f;
    float autoAdvanceTime = 3.5f;

    std::string fullText;
    std::string visibleText;
    bool lineComplete = false;
    size_t currentChar = 0;

public:
    Dialog();
    void update();
    void updateDialogState(int &mission, bool playerClose);
    void draw(sf::RenderWindow& window);
    void next();
    bool isActive() const;
    bool isFinished() const;
    void skipOrNext();
    void reset(const std::vector<std::string>& newMessages);
};

class missionOperation
{
private:
    
    int mision ;//WAZNE!!! 

public:
    int getMission() ;

    missionOperation();

    void update();

};
#include "Dialog.h"
#include <iostream>
Dialog::Dialog()
{
    if (!font.loadFromFile("assets/TekU/SMW Text 2 NC.ttf")) {
        std::cout << "Brak czcionki\n";
    }

    background.setSize(sf::Vector2f(1400, 120));
    background.setFillColor(sf::Color(50, 50, 50, 200));
    background.setPosition(20, 920);

    speakerText.setFont(font);
    speakerText.setCharacterSize(40);
    speakerText.setFillColor(sf::Color::Yellow);
    speakerText.setPosition(20, 920);
    speakerText.setString(speaker + ":");

    messageText.setFont(font);
    messageText.setCharacterSize(30);
    messageText.setFillColor(sf::Color::White);
    messageText.setPosition(20, 980);
}

void Dialog::reset(const std::vector<std::string>& newMessages)
{
    messages = newMessages;
    currentIndex = 0;
    active = true;
    if (!messages.empty()) {
        fullText = messages[0];
        visibleText = "";
        messageText.setString("");
        currentChar = 0;
        lineComplete = false;
        letterClock.restart();
        autoAdvanceClock.restart();
    }
    else {
        messageText.setString("");
        active = false;
    }
}
void Dialog::update()
{
    if (!active) return;

    if (!lineComplete) {
        if (letterClock.getElapsedTime().asSeconds() >= letterDelay) {
            if (currentChar < fullText.size()) {
                visibleText += fullText[currentChar++];
                messageText.setString(visibleText);
                letterClock.restart();
            }
            else {
                lineComplete = true;
                autoAdvanceClock.restart();
            }
        }
    }
    else {
        if (autoAdvanceClock.getElapsedTime().asSeconds() >= autoAdvanceTime) {
            next();
        }
    }
}

void Dialog::updateDialogState(int& mission, bool playerClose)
{
    if (!playerClose || active) return; // nie w³¹czamy nowego dialogu jeœli stary aktywny

    switch (mission)
    {
    case 1:
        reset({
            "Witaj Rycerzu!",
            "Zostales tu zeslany by pomoc mi pokonac gobliny.",
            "Mapa do jednej z ich baz znajduje sie w gruzach domku.",
            "Zdobadz dla mnie 10 sztuk drewna bym go odbudowal."

            });
        break;
        //currentMission = 0;
    case 2:
        reset({
           "Zdobadz dla mnie 10 sztuk drewna bym go odbudowal."
            });
        break;

    case 3:
        
            reset({
                "Dzieki za drewno!",
                "Zaraz odbuduje domek!",
                "Swietna robota, rycerzu!"
                });
            break;
            //currentMission = 2;
        
        
    case 6:
    
        reset({
                "OOOO Znalazlem klucz do bramy!",
                "Mozesz sie tam udac tylko....",
                "Uwazaj pokonaj ich i wroc caly a dam ci kolejne wskazowki"
            });
        break;
    case 7:

        reset({
                "Udaj sie na polnoc"
            });
        break;
    case 9:
    
        reset({
                "BRAWO UDALO CI SIE",
                "Tylko to nie koniec....",
                "By dostac sie do bramy zachod potrzebuje odbudowac wieze",
                "Do tego potrzebuje zlota",
                "Zloto znajduje sie w kopalni po przejsciu przez most",
                "Przynies je a wtedy zbuduje wieze i otworzym brame do ich glownej bazy"
            });
        break;

    case 10:

        reset({
                "Odnajdz kopalnie i wyjmi 2 sztuki zlota"

            });
        break;


    case 11:

        reset({
                "Oo zloto",
                "Teraz odbuje wieze",
                "By dostac sie do bramy zachod potrzebuje odbudowac wieze",
                "Znajde klucz do bramy",

                
            });
        break;

    case 12:

        reset({

                "Jesli chcesz udaj sie na zachod",
                "Przed toba najciezsza walka",
                "Krol goblinow jest ogromny i nie wielu go pokona",
                "Zniszcz ich a osiagniesz chwale" 
                "Uratujesz mnie na zawsze przed ta zgraja",
                "Jesli wygrasz przyjdz pogadac"
                "Powodzenia"

            });
        break;
 
    case 15:

        reset({
                "Super udalo sie",
                "Jestesmy wolni",
                "Bede do konca zycia bardzo ci wdzieczny",
                "Nie musialas a pomogles i pokonales zlo"


            });
        break;

    case 16:

        reset({
                "Teraz zbuduje swoj piekny i wymarzony zamek ",
                "Dziekuje ci"


            });
        break;
    default:
        break;
    }
}

void Dialog::draw(sf::RenderWindow& window)
{
    if (!active) return;

    window.draw(background);
    window.draw(speakerText);
    window.draw(messageText);
}

void Dialog::next()
{
    if (currentIndex + 1 < messages.size()) {
        currentIndex++;
        fullText = messages[currentIndex];
        visibleText = "";
        currentChar = 0;
        messageText.setString("");
        lineComplete = false;
        letterClock.restart();
        autoAdvanceClock.restart();
    }
    else {
        active = false;
        messageText.setString("");
    }
}
void Dialog::skipOrNext()
{
    if (!lineComplete) {
        visibleText = fullText;
        messageText.setString(visibleText);
        currentChar = fullText.size();
        lineComplete = true;
        autoAdvanceClock.restart();
    }
    else {
        next();
    }
}

bool Dialog::isActive() const
{
    return active;
}

bool Dialog::isFinished() const
{
    return !active;
}




missionOperation::missionOperation():mision(0)
{
    

}

int missionOperation::getMission() 
{
    return mision;
}

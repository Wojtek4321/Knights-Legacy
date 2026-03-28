#include "Menu.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <ranges>
namespace fs = std::filesystem;


Menu::Menu(float width, float height) {
    

    if (!font.loadFromFile("assets/TekU/SMW Text 2 NC.ttf")) {
        std::cerr << "Blad Fontu" << std::endl;
    }

    
    if (!titleTexture.loadFromFile("assets/TekU/Ui/backmenu.png")) {
        std::cerr << "Blad background menu" << std::endl;
    }

    titleSprite.setTexture(titleTexture);
    titleSprite.setPosition(0,0); // srodek

    
    std::string options[] = { "Play", "Settings", "Exit" };
    for (int i = 0; i < 3; i++) {
        menu[i].setFont(font);
        menu[i].setString(options[i]);
        menu[i].setCharacterSize(70);
        menu[i].setFillColor(i == 0 ? sf::Color::Cyan : sf::Color::White);


        // wysrodkowanie problematyczne
        menu[i].setPosition(1920 / 3, 1280 / 2 - 70 + i * 100);
    

    }
    std::string tytul1 = "Knight's Legacy";
    tytul.setString(tytul1);
    tytul.setFont(font);
    tytul.setCharacterSize(130);
    tytul.setFillColor(sf::Color::White);
    tytul.setPosition(330, 200);



    //zaczynamy od play
    selectedItemIndex = 0;
}

void Menu::draw(sf::RenderWindow& window,float width) {
    window.draw(titleSprite);
    for (int i = 0; i < 3; i++) {
        sf::FloatRect textRect = menu[i].getLocalBounds();
        menu[i].setPosition(width / 2+30 - textRect.width / 2, 500 + i * 220); // odsetep 1 2 3
        window.draw(menu[i]);
    }
    
    window.draw(arrowSprite);
    window.draw(tytul);
}
void Menu::moveUp() {
    if (selectedItemIndex > 0) {
        // inne to domyslny 
        menu[selectedItemIndex].setFillColor(sf::Color::White);

        selectedItemIndex--;

        // Ustawienie koloru dla nowego wybranego elementu
        if (selectedItemIndex == 0) {
            menu[selectedItemIndex].setFillColor(sf::Color::Cyan); // PLAY CYAN
        }
        else if (selectedItemIndex == 1) {
            menu[selectedItemIndex].setFillColor(sf::Color::Yellow); // SETTINGS yellow
        }
        else {
            menu[selectedItemIndex].setFillColor(sf::Color::Red); // Exit red
        }
    }
}

void Menu::moveDown() {
    if (selectedItemIndex < 2) {
        
        menu[selectedItemIndex].setFillColor(sf::Color::White);

        selectedItemIndex++;

        
        if (selectedItemIndex == 0) {
            menu[selectedItemIndex].setFillColor(sf::Color::Cyan); // Pierwszy element na cyan
        }
        else if (selectedItemIndex == 1) {
            menu[selectedItemIndex].setFillColor(sf::Color::Yellow); // Drugi element na yellow
        }
        else {
            menu[selectedItemIndex].setFillColor(sf::Color::Red); // Trzeci element na red
        }
    }
   
}
void Menu::gameover(sf::RenderWindow& window)
{
  
    
       
        if (!gameOvertex.loadFromFile("assets/TekU/Ui/board.png")) {
            std::cerr << "Blad board.png" << std::endl;
        }
        std::string strgameover = "YOU LOSE";
        gameOverTxt.setString(strgameover);
        gameOverTxt.setFont(font);
        gameOverTxt.setFillColor(sf::Color::White);
        gameOverTxt.setCharacterSize(140);
        gameOverTxt.setPosition(600 , 300);

        gameOver.setTexture(gameOvertex);
        //gameOver.setPosition(1920/2, 1280/2);

       //window.draw(gameOver);
        window.draw(gameOverTxt);
}
void Menu::Ranking(sf::RenderWindow& window, int seconds, int minutes, bool menu,int missions)
{
    sf::Texture board;
    sf::Sprite table;

    if (!board.loadFromFile("assets/TekU/Ui/board.png")) {
        std::cerr << "Blad board.png" << std::endl;
    }

    table.setTexture(board);
    table.setScale(1.5, 1.5);
    table.setPosition(300, 100);

    sf::Text Rank;
    Rank.setFont(font);
    Rank.setString("Ranking");
    Rank.setCharacterSize(70);
    Rank.setFillColor(sf::Color::White);
    Rank.setPosition(790, 170);
    
    window.draw(titleSprite);
    window.draw(table);
    window.draw(Rank);



   
   
    if (!fs::exists("Saves"))
    {
        fs::create_directory("Saves");
        std::cout << "utworzona saves" << std::endl;
        
       
    }
    int lastNumber = 0;
    std::ifstream inFile("Saves/Ranking.txt");
    std::string line;
    while (std::getline(inFile, line)) 
    {
        lastNumber++;
    }
    inFile.close();

    if (menu == true)
    {
        //std::cout << "Nie zapisuje" << std::endl;
    }
    else
    {
        // Zapis do pliku
        if (gameSaved == false ) //&& missions == 0
        {
            std::ofstream outFile("Saves/Ranking.txt", std::ios::app); // tryb dopisywania
            if (outFile.is_open()) {
                outFile << lastNumber + 1 << "." << input << " - " << minutes << " minuty " << seconds << " sekundy " <<"/n";
                outFile.close();
                gameSaved = true;
                std::cout << "wynik save\n";
            }
            else {
                std::cerr << "nie mo¿na otworzyc pliku do zapisu\n";
            }
        }
        else if (gameSaved) {
            std::cout << "wynik raz zostal zapisany , nie mozna jeszcze\n";
        }
    }
    


    //wczytywanie plikow
    std::vector<std::pair<int, std::string>> results;
    std::ifstream readFile("Saves/Ranking.txt");
    while (std::getline(readFile, line)) 
    {
        std::smatch match;
        std::regex pattern("(\\d*\\.*)?\\s*(.*?)\\s*-\\s*(\\d+)\\s*minuty\\s*(\\d+)\\s*sekundy");

        if (std::regex_search(line, match, pattern)) {
            int mins = std::stoi(match[3]); //przechwyt minut jako 3 
            int secs = std::stoi(match[4]);//przechwyt sekund jako 4 wyst
            int totalSeconds = mins * 60 + secs;
            results.emplace_back(totalSeconds, match[2].str() + " - " + match[3].str() + " minuty " + match[4].str() + " sekundy");
        }
    }
    readFile.close();

    // od nakrotszego czasu do najmniejszego czasu
    std::ranges::sort(results);

    auto Rezultat = results | std::views::reverse | std::views::take(8);


    // Rysuje ranking
    int y = 300;
    int index = 1;   //top 8
    for (const auto&[_,text]:Rezultat)
    {
        sf::Text resultText;
        resultText.setFont(font);

        std::string rankedResult = std::to_string(index) + ". " + text; //numer

        resultText.setString(rankedResult); // 
        resultText.setCharacterSize(40);
        resultText.setFillColor(sf::Color::White);
        resultText.setPosition(400, y);
        y += 60;
        window.draw(resultText);

        index++;//max5 kolejny

    }
}

void Menu::Pause(sf::RenderWindow& window)
{
    std::string pause = "Pause";
    pausetxt.setString(pause);
    pausetxt.setFont(font);
    pausetxt.setFillColor(sf::Color::White);
    pausetxt.setCharacterSize(100);
    pausetxt.setPosition(960, 649);

    sf::Text backText;
    backText.setFont(font);
    backText.setString("Back");
    backText.setCharacterSize(70);
    backText.setFillColor(sf::Color::White);
    backText.setPosition(1920 / 3, 600);

    
    window.draw(pausetxt);
    window.draw(backText);
}

void Menu::settings(sf::RenderWindow& window, int selectedOption, int volume) {
    if (!font.loadFromFile("assets/TekU/SMW Text 2 NC.ttf")) {
        std::cerr << "blad fontu" << std::endl;
    }

    
    sf::Text volumeText;
    volumeText.setFont(font);
    volumeText.setCharacterSize(70);
    volumeText.setFillColor(sf::Color::Cyan);
    volumeText.setPosition(200, 400);

    sf::RectangleShape volumeBar(sf::Vector2f(50, volume * 3));
    volumeBar.setFillColor(sf::Color::Cyan);
    volumeBar.setPosition(100, 400 + (300 - volume * 3));

    sf::RectangleShape volumeOutline(sf::Vector2f(50, 300));
    volumeOutline.setFillColor(sf::Color::Transparent);
    volumeOutline.setOutlineThickness(5);
    volumeOutline.setOutlineColor(sf::Color::White);
    volumeOutline.setPosition(100, 400);

    sf::Text backText;
    backText.setFont(font);
    backText.setString("Back");
    backText.setCharacterSize(70);
    backText.setFillColor(sf::Color::White);
    backText.setPosition(200, 600);


    volumeText.setString("Volume: " + std::to_string(volume));

    //volumeBar.setSize(sf::Vector2f(50, volume * 3));
    //volumeBar.setPosition(1700, 500 + (300 - volume * 3));

        if (selectedOption == 0) {
            volumeText.setFillColor(sf::Color::Yellow);
            backText.setFillColor(sf::Color::White);
        }
        else {
            volumeText.setFillColor(sf::Color::White);
            backText.setFillColor(sf::Color::Red);
        }


        sf::Texture Insturction;
        if (!Insturction.loadFromFile("assets/TekU/Ui/insturction.png"))
        {
            std::cerr<<"Brak insturkcji"<<std::endl;
        }
        sf::Sprite Ins;
        
        Ins.setTexture(Insturction);
        Ins.setPosition(800, 100);
        //Ins.setScale(0.7, 0.7);




        window.draw(titleSprite);
        window.draw(volumeText);
        window.draw(volumeOutline);
        window.draw(volumeBar);
        window.draw(backText);
        window.draw(Ins);
    
}


int Menu::getSelectedIndex() 
{ 
    return selectedItemIndex; 

}

void Menu::confi(sf::RenderWindow& window,int selectknight)
{
    sf::Text LEFT;
    LEFT.setFont(font);
    LEFT.setString("<---");
    LEFT.setCharacterSize(70);
    LEFT.setFillColor(sf::Color::White);
    LEFT.setPosition(350, 820);

    sf::Text RIGHT;
    RIGHT.setFont(font);
    RIGHT.setString("--->");
    RIGHT.setCharacterSize(70);
    RIGHT.setFillColor(sf::Color::White);
    RIGHT.setPosition(650, 820);

    Pole.loadFromFile("assets/TekU/UI/pb.png");
    PoleS.setTexture(Pole);
    PoleS.setPosition(350,200);
    PoleS.setScale(2.5, 3);
    PoleS.setColor(sf::Color::Blue);

    K1.loadFromFile("assets/TekU/Kn/K_Bblue.png");
    K1S.setTexture(K1);
    K1S.setPosition(300, 200);
    K1S.setScale(3,3);
    
    K2.loadFromFile("assets/TekU/Kn/K_Pp.png");
    K2S.setTexture(K2);
    K2S.setPosition(300, 200);
    K2S.setScale(3, 3);

    K3.loadFromFile("assets/TekU/Kn/K_Rr.png");
    K3S.setTexture(K3);
    K3S.setPosition(300, 200);
    K3S.setScale(3, 3);

    K4.loadFromFile("assets/TekU/Kn/K_Yy.png");
    K4S.setTexture(K4);
    K4S.setPosition(300, 200);
    K4S.setScale(3, 3);

 
    //wpisywanie oblusga
    PoleI.loadFromFile("assets/TekU/UI/pbi.png");
    PoleSI.setTexture(PoleI);
    PoleSI.setPosition(1100, 400);
    PoleSI.setScale(2.5, 3);
    PoleSI.setColor(sf::Color::Blue);

    textbox.setSize(sf::Vector2f(192, 192));
    textbox.setPosition(1105, 400);
    textbox.setFillColor(sf::Color::Transparent);
    
    textinput.setFont(font);
    textinput.setCharacterSize(65);
    textinput.setFillColor(sf::Color::White);
    textinput.setPosition(textbox.getPosition().x + 40, textbox.getPosition().y + 40);
    textinput.setOutlineColor(sf::Color::Black); 
    textinput.setOutlineThickness(5);

    sf::Text Twoj;
    Twoj.setFont(font);
    Twoj.setCharacterSize(65);
    Twoj.setFillColor(sf::Color::White);
    Twoj.setPosition(1105 -30,  400 - 60);
    Twoj.setOutlineColor(sf::Color::Black);
    Twoj.setOutlineThickness(5);
    Twoj.setString("Your nickname");


    sf::Text START;
    START.setFont(font);
    START.setString("START");
    START.setCharacterSize(70);
    START.setFillColor(sf::Color::White);
    START.setPosition(1220, 820);



    window.draw(titleSprite);
    window.draw(PoleS);
    window.draw(PoleSI);
    window.draw(LEFT);
    window.draw(RIGHT);
    window.draw(START);
    if (selectknight == 1)
    {
        window.draw(K2S);
    }
    if (selectknight == 2)
    {
        window.draw(K3S);
    }
    if (selectknight == 3)
    {
        window.draw(K4S);
    }
    if (selectknight == 0)
    {
        window.draw(K1S);
    }
    window.draw(textbox);
    window.draw(textinput);
    window.draw(Twoj);

}

void Menu::inputing(sf::Event& event, bool confstate)
{
    bool isFinalized = false;
    if (confstate == true)
    {


        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (textbox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                click = true;

            }
            else
            {
                click = false;
            }
        }
        if (click && event.type == sf::Event::TextEntered)
        {
            if (!isFinalized) {
                if (event.text.unicode == 8 && !input.empty()) { // Backspace
                    input.pop_back();
                }
                else if (event.text.unicode < 128 && input.size() < 8) { // ascii 
                    input += static_cast<char>(event.text.unicode);
                }

                if (event.text.unicode == 13) { // enter
                    isFinalized = true;
                }

                textinput.setString(input);
            }
        }
        std::cout << "Input: " << input << std::endl;
    }
}

void Menu::gameWin(sf::RenderWindow& window)
{
    sf::Text Congratulations;
    Congratulations.setFont(font);
    Congratulations.setString("Congratulations");
    Congratulations.setCharacterSize(100);
    Congratulations.setFillColor(sf::Color::White);
    Congratulations.setPosition(window.getSize().x / 2 - Congratulations.getGlobalBounds().width / 2, 100);
    Congratulations.setOutlineColor(sf::Color::Black);
    Congratulations.setOutlineThickness(5);

    sf::Text winText;
    winText.setFont(font);
    winText.setString("You Win!");
    winText.setCharacterSize(100);
    winText.setFillColor(sf::Color::White);
    winText.setPosition(window.getSize().x / 2 - winText.getGlobalBounds().width / 2, 300);
    winText.setOutlineColor(sf::Color::Black);
    winText.setOutlineThickness(5);

    sf::Text textinput2;
    textinput2.setFont(font);
    textinput2.setCharacterSize(200);
    textinput2.setString(input);

    sf::Color goldColor(255, 215, 0);
    textinput2.setFillColor(goldColor);
    textinput2.setOutlineColor(sf::Color::Black);
    textinput2.setOutlineThickness(5);

    sf::FloatRect inputRect = textinput2.getLocalBounds();
    textinput2.setOrigin(inputRect.left + inputRect.width / 2.0f, inputRect.top + inputRect.height / 2.0f);
    textinput2.setPosition(window.getSize().x / 2.0f, 500 + inputRect.height / 2.0f); 

    

    window.draw(titleSprite);
    window.draw(winText);
    window.draw(textinput2);
    window.draw(Congratulations);

}



sf::Text& Menu::getNick() 
{
    textinput.setString(input);
    return textinput;
}

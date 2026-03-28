#include "Player.h"
#include <string>

Player::Player()
{
    position = sf::Vector2f(4969.06f, 3425.88f);
    speed = 0.3f;
    SpKnight.setOrigin(96, 96);//srodek postaci

    //ANIMACJA
    currentFrame = sf::IntRect(0, 0, 192, 192);
    SpKnight.setTextureRect(currentFrame);

    animationTimer = 0.0f;
    frameIndex = 0;
    hp = 100;
    isMoving = false;
    isDead = false;
    canAttack = true; 
}

void Player::poruszanie(const std::vector<Collision::Object>& objects)
{   
    velocity = sf::Vector2f(0.0f, 0.0f);
    isMoving = false;

    if (!isDead)
    {

   
    // Ruch w górê
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W ) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        velocity.y = -speed;
        isMoving = true;
    }
    // Ruch w dó³
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        velocity.y = speed;
        isMoving = true;
    }
    // Ruch w lewo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        velocity.x = -speed;
        isMoving = true;
        SpKnight.setScale(-1.f, 1.f);
    }
    // Ruch w prawo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        velocity.x = speed;
        isMoving = true;
        SpKnight.setScale(1.f, 1.f);
    }
  
    //kolizja
  
    // Sprawdzamy kolizje w kierunku X
    if (velocity.x != 0.f)
    {
        sf::FloatRect nextPosition = getGloBou();
        nextPosition.left += velocity.x;

        bool canMoveX = true;
        for (const auto& object : objects)
        {
            if (nextPosition.intersects(object.getBounds()))
            {
                canMoveX = false; // block w poziomie
                break;
            }
        }

        if (canMoveX)
        {
            SpKnight.move(velocity.x, 0.f);
        }
        else
        {
            velocity.x = 0.f; // Zatrzymujemy gracza w poziomie
        }
    }

    // Sprawdzamy kolizje w kierunku Y
    if (velocity.y != 0.f)
    {
        sf::FloatRect nextPosition = getGloBou();
        nextPosition.top += velocity.y ;

        bool canMoveY = true;
        for (const auto& object : objects)
        {
            if (nextPosition.intersects(object.getBounds()))
            {
                canMoveY = false; // block pion
                break;
            }
        }

        if (canMoveY)
        {
            SpKnight.move(0.f, velocity.y); // Ruch w pionie
        }
        else
        {
            velocity.y = 0.f; //  stop gracza 
        }
    }


    }
}

void Player::dead(const std::string& texturedeath)
{
    if (hp ==0 && !isDead)
    {
        if (!teKnight.loadFromFile(texturedeath))
        {
            std::cout << "Tekstura œmierci rycerza nie za³adowana" << std::endl;
            return;
        }
        SpKnight.setTexture(teKnight);
        currentFrame = sf::IntRect(0, 0, 128, 128); // Poprawny rozmiar klatki
        frameIndex = 0;
        animationTimer = 0.0f; // Resetujemy timer animacji
        isDead = true;
        speed = 0.0f;
       
    }

}
void Player::update(float deltaTime, const std::vector<Collision::Object>& objects, const std::vector<Tree>& trees,sf::Text& nick)
{

    //std::cout << "speed" << speed << std::endl;
    nick.setPosition(SpKnight.getPosition().x-50, SpKnight.getPosition().y - 75);
    nick.setCharacterSize(20);
    nick.setOutlineThickness(2);
    
    if (!canAttack) {
        if (attackCooldown.getElapsedTime().asSeconds() > attackCooldownTime) {  //oczekiwanie na animacje ataku
            canAttack = true;  
        }
    }
    
    if (!isDead) // Gnie mratwy ruszac sie moze
    {
        animationTimer += deltaTime;
        poruszanie(objects);
        SpKnight.move(velocity);



       /* if (!canAttack && attackCooldown.getElapsedTime().asSeconds() >= 1.5f) {
            canAttack = true;
        }*/
            
        // attack
        if (hit) {
            if (animationTimer >= 0.10f) {
                animationTimer = 0.0f;

                if (lastDirection == "up") {
                    currentFrame.top = 1344;
                }
                else if (lastDirection == "down") {
                    currentFrame.top = 768;
                }
                else if (lastDirection == "right") {
                    currentFrame.top = 576;
                }
                else if (lastDirection == "left") {
                    currentFrame.top = 576;
                }

                frameIndex = (frameIndex + 1) % 6;

                // Koniec animacji resetuje atak
                if (frameIndex == 0) {
                    hit = false;
                }
            }
        }
        else {
            // bieg
            if (animationTimer >= 0.15f) {
                animationTimer = 0.0f;

                if (isMoving) {
                    if (velocity.x > 0) {
                        currentFrame.top = 192; 
                        frameIndex = (frameIndex + 1) % 6;
                        lastDirection = "right";
                    }
                    else if (velocity.x < 0) {
                        currentFrame.top = 192; 
                        frameIndex = (frameIndex + 1) % 6;
                        lastDirection = "left";
                    }
                    else if (velocity.y < 0) {
                        currentFrame.top = 192;
                        frameIndex = (frameIndex + 1) % 6;
                        lastDirection = "up";
                    }
                    else if (velocity.y > 0) {
                        currentFrame.top = 192; 
                        frameIndex = (frameIndex + 1) % 6;
                        lastDirection = "down";
                    }
                }
                else {
                    // Idle
                    if (lastDirection == "up") {
                        currentFrame.top = 1344; 
                        frameIndex = (frameIndex + 1) % 3;
                    }
                    else if (lastDirection == "down") {
                        currentFrame.top = 960; 
                        frameIndex = (frameIndex + 1) % 3;
                    }
                    else {
                        currentFrame.top = 0; 
                        frameIndex = (frameIndex + 1) % 6;
                    }
                }
            }
        }

        // zmiana pozycyjna tesktury
        currentFrame.left = frameIndex * 192;
        SpKnight.setTextureRect(currentFrame);
    }

    if (isDead)
    {
        animationTimer += deltaTime;

        if (animationTimer > 0.120f) // 120 ms na klatkê
        {
            if (frameIndex < 14) // ilosc klatek
            {
                if (frameIndex < 7)
                {
                    //Rzad 1
                    currentFrame.left = frameIndex * 128;
                    currentFrame.top = 0; // Pierwszy rz¹d
                }
                else
                {
                    // Rzad 2
                    currentFrame.left = (frameIndex - 6) * 128;
                    currentFrame.top = 128; // Drugi rz¹d
                }

                SpKnight.setTextureRect(currentFrame);
                frameIndex++;
                animationTimer = 0.0f; 
            }
            else
            {
                // Koniec ostatnia klatka
                currentFrame.left = (14 - 1) * 128;
                currentFrame.top = 128;
                SpKnight.setTextureRect(currentFrame);
            }
        }
        return;
    }
}

void Player::hits(std::vector<Enemy>& enemies, std::vector<Tree>& trees, std::vector<Sheep>& sheeps, std::vector<Collision::Object>& objects, std::vector<GoldMine>& mines, std::vector<Buildings>& buildings)
{
    if (canAttack && !isDead) 
    {
        hit = true;
        frameIndex = 0;
        animationTimer = 0.0f;
        canAttack = false;
        attackCooldown.restart();

        float attackRange = 140;
        float damage = 45;   //40

        for (Enemy& enemy : enemies) {  // Przechodzimy przez wszystkich wrogów
            sf::Vector2f direction = enemy.getPosition() - getPosition();
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance < attackRange) {  // Jeœli wróg jest w zasiêgu, otrzymuje obra¿enia
                enemy.takeDamage(damage);
                std::cout << "Player hits enemy" << enemy.getHealth() << std::endl;
            }
        }
        for (Tree& tree : trees) {
           
            sf::Vector2f direction = tree.getPosition() - (getPosition());
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance < attackRange) {
                tree.takeDamage(damage,objects);
                std::cout << "Player hits tree " << tree.getHealth() << std::endl;
                if (tree.hasDroppedItem() == true)
                {
                    std::cout << "dodalem drzewo" << std::endl;
                    treeitem++;
                    std::cout<<treeitem<<std::endl;

                }
            }
        }
        for (Sheep& sheep : sheeps) {
            // Przesuniêcie o 40 w lewo
            sf::Vector2f direction = sheep.getPosition() - (getPosition()-sf::Vector2f(40, 0));
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance < attackRange) {
                sheep.takeDamage(damage);
                std::cout << "Player hits sheep " << sheep.getHealth() << std::endl;
                if (sheep.hasDroppedItem1() == true)
                {
                    std::cout << "dodalem mieso" << std::endl;
                    sheepitem++;
               
                }
            }
        }
        for (GoldMine& mine : mines) {
            // Przesuniêcie o 40 w lewo
            sf::Vector2f direction = mine.getPosition() - (getPosition() - sf::Vector2f(40, 0));
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance < attackRange) {
                mine.build(damage);
                std::cout << "Player uzywa kopalni " << mine.getHealth() << std::endl;
                if (mine.hasDroppedItem2() == true)
                {
                    std::cout << "dodalem gold" << std::endl;
                    golditem++;

                }
            }
        }
        for (Buildings& build : buildings) {  // Przechodzimy przez wszystkich wrogów
            sf::Vector2f direction = build.getPosition() - getPosition();
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance < attackRange) {  // Jeœli wróg jest w zasiêgu, otrzymuje obra¿enia
                build.takeDamage(damage);
                std::cout << "Player hits enemy" << build.health() << std::endl;
            }
        }
        
    }
}
float& Player::getHealth() {
    //std::cout << hp << std::endl;
    return hp; // Zwraca aktualne zdrowie
}


sf::Vector2f Player::getPosition() const
{
		return SpKnight.getPosition();
}


sf::FloatRect Player::getGloBou() const
{
    sf::FloatRect bounds = SpKnight.getGlobalBounds();

    float scaleFactor = 0.2; // 1/2 oryginalnego rozmiaru

    float newWidth = bounds.width * scaleFactor;
    float newHeight = bounds.height * scaleFactor;
    float offsetX = (bounds.width - newWidth) / 2;
    float offsetY = (bounds.height - newHeight) / 2;

    return sf::FloatRect(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(SpKnight);

}

void Player::loadTextures(int selectknight)
{
    switch (selectknight) {
    case 1:
        if (!aliveTexture.loadFromFile("assets/TekU/Kn/K_P.png")) {
            std::cout << "Tekstura 2 " << std::endl;
        }
        break;
    case 2:
        if (!aliveTexture.loadFromFile("assets/TekU/Kn/K_R.png")) {
            std::cout << "Tekstur 3 " << std::endl;
        }
        break;
    case 3:
        if (!aliveTexture.loadFromFile("assets/TekU/Kn/K_Y.png")) {
            std::cout << "Tekstura 4" << std::endl;
        }
        break;
    default:
        if (!aliveTexture.loadFromFile("assets/TekU/Kn/K_B.png")) {
            std::cout << "Tekstura 1" << std::endl;
        }
    }
    SpKnight.setTexture(aliveTexture);
    SpKnight.setPosition(position);
}



void Player::uzyjItem(int index) {
    { 
        std::cout << "Wybrano przedmiot: " << std::endl;
        
        if (index == 1) {
            // Sprawdzamy, czy przedmiot to "Mieso"
            if (Player::getHealth() >= 100.0f) {
                //std::cout << "Full HP, nie mo¿na u¿yæ 'Mieso'." << std::endl;
            }
            else {
                // Leczenie gracza
                if (sheepitem > 0 && hp>0)

                {
                    hp += 20;
                    sheepitem--;
                    // Zapewniamy, ¿e zdrowie nie przekroczy 100 (zak³adaj¹c, ¿e 100 to maksymalne zdrowie)
                    if (hp > 100.0f)
                    {
                        hp = 100.0f;
                    }

                    //std::cout << "Zdrowie gracza: " << hp << std::endl;
                }
                else
                {
                    //std::cout << "brak miesa" << std::endl;
                }

            }
        }
        
        else  if (index == 2) {
            // Sprawdzamy, czy przedmiot to "drzewo"

            if (Player::getHealth() >= 100.0f) {
                //std::cout << "Full HP, nie mo¿na u¿yæ 'Drzewo'." << std::endl;
            }
            else {
                if (treeitem > 0 && hp > 0) {
                    // Leczenie gracza
                    hp += 10;
                    // Zapewniamy, ¿e zdrowie nie przekroczy 100 (zak³adaj¹c, ¿e 100 to maksymalne zdrowie)
                    if (hp > 100.0f) {
                        hp = 100.0f;
                    }
                    treeitem--;

                    //std::cout << "Zdrowie gracza: " << hp << std::endl;
                }
                else
                {
                    //std::cout << "brak drzewo" << std::endl;
                }

            }
        }
        else  if (index == 3) {
            // Sprawdzamy, czy przedmiot to "gold"

            if (Player::getHealth() >= 100.0f) {
                //std::cout << "Full HP, nie mo¿na u¿yæ 'Gold'." << std::endl;
            }
            else {
                if (golditem > 0 && hp > 0) {
                    // Leczenie gracza
                    hp += 5;
                    // Zapewniamy, ¿e zdrowie nie przekroczy 100 (zak³adaj¹c, ¿e 100 to maksymalne zdrowie)
                    if (hp > 100.0f) {
                        hp = 100.0f;
                    }
                    golditem--;

                    //std::cout << "Zdrowie gracza: " << hp << std::endl;
                }
                else
                {
                    //std::cout << "brak gold" << std::endl;
                }

            }
        }
    }
    // Na koniec informujemy o liczbie pozosta³ych przedmiotów w ekwipunku

}
int Player::sheepitem1() const
{
    return sheepitem;
}

int Player::treeitem1() const
{
    return treeitem;
}
int Player::golditem1() const
{
    return golditem;
}

void Player::decreaseTreeItem(int amount)  // Funkcja zmniejszaj¹ca drewno
{
    if (treeitem >= amount)
        treeitem -= amount;
}
void Player::decreaseGoldItem(int amount)  // Funkcja zmniejszaj¹ca drewno
{
    if (golditem >= amount)
        golditem -= amount;
}
Player::~Player() {}

#include "Enemy.h"
#include <vector>






Enemy::Enemy(float x, float y, float speed, float attackRange, sf::Texture* texture, int health)
    : position(x, y), speed(speed), attackRange(attackRange), texture(texture),health(health), attackDamage(5),
    state(IDLE), currentFrame(0), isFacingLeft(false),healthfull(health)
{

    if (!texture) {
        std::cerr << "brak tex wrogow" << std::endl;
        return;
    }
    //const int healthfull = health;
    sprite.setTexture(*texture);
    //spawnpostiion = sf::Vector2f(x, y);
    sprite.setPosition(position);
   sprite.setTextureRect(sf::IntRect(0, 0, 192, 192));  
   

   healthBarBackground.setSize(sf::Vector2f(50, 10));  
   healthBarBackground.setFillColor(sf::Color::Red);
   healthBarBackground.setOrigin(25, 2.5f);  // środek ekranu

   //kolor healtbaru
   healthBar.setSize(sf::Vector2f(50, 10));  
   healthBar.setFillColor(sf::Color::Green);
   healthBar.setOrigin(25, 2.5f);  //

   //boss
   std::cout << "healthfull: " << healthfull << std::endl;
   if (healthfull >= 1000)
   {
       sprite.setScale(scaleX, scaleY);
       sprite.setOrigin(320, 320);
   }
   else
   {
       sprite.setScale(1, 1);
       sprite.setOrigin(64, 64);
   }
}
float Enemy::getHealth() const
{
    return health;
}
void Enemy::update(sf::Vector2f playerPosition, float& playerHealth, std::vector<Collision::Object>& objects) 
{
    if (state == DEAD) 
    {
        if (readyToSpawn && canRespawn && respawnClock.getElapsedTime().asSeconds() >= respawnDelay)
        {
            Respawn();
        }
        return; 
    }

        float distance = std::sqrt(std::pow(playerPosition.x - position.x, 2) + std::pow(playerPosition.y - position.y, 2));  //odleg e - p 

        if (distance < attackRange) {
            attack(playerHealth, playerPosition);
        }
        else if (distance < 400) {
            moveTowards(playerPosition,objects);
        }
        else if (playerHealth == 0) {
            state = IDLE;
        }
        else {
            state = IDLE;
        }

        animate();
        sprite.setPosition(position);
        sprite.setOrigin(96, 96);

       
        float healthPercentage = health / healthfull;  // sredni hp

        
        if (health < 0) health = 0;
           

        // updatowane hp 
        healthBar.setSize(sf::Vector2f(50 * healthPercentage, 10));

        
        healthBar.setPosition(position.x, position.y - 100);  // 192-100
        healthBarBackground.setPosition(position.x, position.y - 100);

       
        //std::cout << "Enemy HP: " << health << " | Health Bar Size: " << healthBar.getSize().x << std::endl;
    }


void Enemy::moveTowards(sf::Vector2f target, std::vector<Collision::Object>& objects)
{
    sf::Vector2f direction = target - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0) 
    {
        direction /= length;
        position += direction * speed;
    }

    isFacingLeft = (direction.x < 0);
    state = WALKING;
    sf::FloatRect nextPosition = getGloBou();
    

    // kolizja 
    for (const auto& object : objects) 
    {
        if (object.getBounds().intersects(nextPosition)) 
        {
            
            position -= direction * speed;  
            break;  
        }
    }
}
void Enemy::attack(float& playerHealth, sf::Vector2f playerPosition) 
{
    if (playerHealth <= 0)
    {
        state = IDLE;  // player nie zyje ma byc idle
        return;
    }
    if (state != ATTACKING)   
    {   
        attackTimer.restart();
        state = ATTACKING;
        currentFrame = 0;  // reset anim
    }

    
    if (attackTimer.getElapsedTime().asSeconds() > (0.1f * 5.0)) { // 5.0 SPOWOLNIENIA    !!dostosowac pod animacje 5 - 1    bylo 9 -2
        playerHealth -= attackDamage;
        std::cout << "Enemy attacks! Player HP: " << playerHealth << std::endl;
        attackTimer.restart();  // Restartujemy czas ataku
        state = IDLE;  // do IDLE po ataku
    }

    sf::Vector2f direction = playerPosition - position;

    if (std::abs(direction.x) > std::abs(direction.y)) {
        if (direction.x > 0)
            attackDirection = "right";
        else
            attackDirection = "left";
    }
    else {
        if (direction.y > 0)
            attackDirection = "down";
        else
            attackDirection = "up";
    }
}



void Enemy::die() {
    state = DEAD;
    deathTimer.restart();
    readyToSpawn = true;
    respawnClock.restart();
    //sprite.setTextureRect(sf::IntRect(0, 384, 192, 192)); // Animacja śmierci
    std::cout << "Enemy dead" << std::endl;
}

void Enemy::animate() {
    int frameCount = 0;
    int startX = 0, startY = 0;
    switch (state) {
    case IDLE:
        frameCount = 7;
        startX = 0;
        startY = 0;
        break;
    case WALKING:
        frameCount = 6;
        startY = 192;
        startX = 0;
        break;
    case ATTACKING:
        frameCount = 6;
        if (attackDirection == "right") {
            startY = 384;  
            startX = 0;
        }
        else if (attackDirection == "left") {
            startY = 384;
            startX = 0;  
        }
        else if (attackDirection == "down") {
            startY = 576;  
            startX = 0; 
        }
        else if (attackDirection == "up") {
            startY = 768;
            startX = 0;  
        }
        break;
    case DEAD:
        return;
    }

    
    if (animationClock.getElapsedTime().asSeconds() > 0.1) { // 0.2 SPOWOLNIENIA
        currentFrame = (currentFrame + 1) % frameCount;
        animationClock.restart();
    }

    sprite.setTextureRect(sf::IntRect(startX + currentFrame * 192, startY, 192, 192));

    if (healthfull>= 1000)
    {
        if (isFacingLeft) {
            sprite.setScale(-5.f, 5.f);
        }
        else {
            sprite.setScale(5.f, 5.f);
        }
    }
    else
    {
        if (isFacingLeft) 
        {
        sprite.setScale(-1.f, 1.f);
        }
        else {
        sprite.setScale(1.f, 1.f);
        }
    }
    
}

void Enemy::takeDamage(float damage) {
    if (state == DEAD) return; // Jeśli wróg już martwy, nie przyjmuje obrażeń

    health -= damage;
    std::cout << "Enemy takes " << damage << " damage! HP: " << health << std::endl;


    if (health <= 0) {
        die();
    }
}

sf::Vector2f Enemy::getPosition() const
{
    return sprite.getPosition();
}



sf::FloatRect Enemy::getGloBou() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // Zmniejszenie rozmiaru
    float scaleFactor = 0.01; 

    float newWidth = bounds.width * scaleFactor;
    float newHeight = bounds.height * scaleFactor;

    // Obliczanie przesunięć
    float offsetX = (bounds.width - newWidth) / 2;
    float offsetY = (bounds.height - newHeight) / 2;

    
    return sf::FloatRect(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight); 
}


void Enemy::draw(sf::RenderWindow& window) {
    if (state != DEAD || deathTimer.getElapsedTime().asSeconds() < 0.3) {
        window.draw(sprite);
        window.draw(healthBarBackground);  // Najpierw tło
        window.draw(healthBar);
    }
}
void Enemy::Respawn()
{
    health = healthfull;
    state = IDLE;
    position = spawnpostiion;
    sprite.setPosition(position);
    readyToSpawn = false;
    std::cout << "Enemy respawned!" << std::endl;
}

void Enemy::setRespawnAvailable(bool value, sf::Vector2f newSpawnPosition)
{
    spawnpostiion = newSpawnPosition;
    canRespawn = value;
}
bool Enemy::FullDead() {
    if (state == DEAD && !canRespawn ) {
        fullyDead = true;  // Ustawiamy stan na "pełną śmierć" tylko wtedy, gdy oba warunki są spełnione
    }
    return fullyDead;  // Zwracamy aktualny stan "pełnej śmierci"
}



Tree::Tree(float x, float y, int hp, sf::Texture* tex, std::vector<Collision::Object>& objects)
    : position(x, y), health(hp), frameIndex(0), frameTime(0.2f), isChopped(false) {

    topx = x - 56;  // dookreslenie wielkosc kolziji
    topy = y - 96;
   
    objects.emplace_back(sf::FloatRect(topx, topy, 117,170 ));
    
    std::cout << "utworzono obiekt"<<x<<y << std::endl;

    
    sprite.setTexture(*tex);
    sprite.setOrigin(96, 96);
    sprite.setPosition(position);
    sprite.setTextureRect(sf::IntRect(0, 0, 192, 192)); // Pierwsza klatka animacji
}

void Tree::update(std::vector<Collision::Object>& objects) {
    if (isChopped) 
    {
        if (regenClock.getElapsedTime().asSeconds() > 40.0f)  //po 40 regeneracja
        {
            isChopped = false;
            health = 100;
            frameIndex = 0;
            std::cout << "Tree regenerated!"<<std::endl;
            for (auto& obj : objects) {
                if (obj.getBounds().left == 0 && obj.getBounds().top == 0) { // szukamy obiektu drzewa w object kolizjach
                    obj = Collision::Object(sf::FloatRect(TOPx(), TOPy(), 117, 170));
                    break;
                }
            }
        }
        return;
    }

    // idle
    if (animationClock.getElapsedTime().asSeconds() > frameTime) {
        animationClock.restart();


        sprite.setTextureRect(sf::IntRect(frameIndex * 192, 0, 192, 192));




        frameIndex = (frameIndex + 1) % 4; // 4 klatki
    }
}

void Tree::takeDamage(int damage, std::vector<Collision::Object>& objects) {
    if (!isChopped) 
    {
        health -= damage;

        if (health <= 0) {
            isChopped = true; //sciete
            regenClock.restart();
            sprite.setTextureRect(sf::IntRect(0, 384, 192, 192)); // Po ścięciu 3 rzad
            std::cout << "Tree chopped down!"<<std::endl;
            for (auto& obj : objects) {
                if (obj.getBounds().left == TOPx() && obj.getBounds().top == TOPy()) {
                    obj = Collision::Object(sf::FloatRect(0, 0, 0, 0)); // Ustawiamy kolizję na 0,0,0,0
                    break;
                }
            }
            if (iloscitemot == 1)
            {

                std::cout << "wypadl item" << std::endl;
                hasItemDropped = true;
                std::cout << "tree dalem item" << std::endl;
                iloscitemot--;

            }
            
        }
        else {
            // dostaje atak
            sprite.setTextureRect(sf::IntRect(frameIndex * 128, 192, 128, 192)); // drugi rzad
            attackClock.restart(); // reset anim zegar
        }
    }
    else
    {
        hasItemDropped = false;
    }
}
sf::FloatRect Tree::getBounds() const {
    return sprite.getGlobalBounds(); // prosotoka sprita
}
void Tree::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

int Tree::getHealth() const {
    return health;
}

sf::Vector2f Tree::getPosition() const {
    return position;
}

float Tree::TOPx() {
    return topx; 
}

float Tree::TOPy() {
    return topy;  
}

bool Tree::hasDroppedItem() const {
    return hasItemDropped; 
}


Sheep::Sheep(float x, float y, int hp, sf::Texture* idleTex, std::vector<Collision::Object>& objects)
    : Tree(x, y, hp, idleTex,objects) ,isRunning(false) 
{

    health = 70.0f;
    // Ładujemy teksturę biegania
    if (!runTexture.loadFromFile("assets/TekU/mobs/sheeprun.png")) {
        std::cerr << "blad nie ma tekstury sheep"<<std::endl;
    }

    Collision::removeLastObject(objects);  // PROBLEM tworzy kolizje owce dlatego USUWAM JĄ  !!!!!!!!!!!!
    this->texture = idleTex;

    sprite.setTexture(*idleTex);
    sprite.setOrigin(64, 64);
    spawnPosition = sf::Vector2f(x, y);
    sprite.setPosition(x, y); 
 
    sprite.setTextureRect(sf::IntRect(0, 0, 128, 128)); 
}



void Sheep::update(sf::Vector2f playerPosition, std::vector<Collision::Object>& objects) {
    if (isChopped) {
        sprite.setColor(sf::Color(255, 255, 255, 100)); // Zmniejszenie widoczności 


        if (respawnClock.getElapsedTime().asSeconds() > 30.0f) 
        {
            //REGENERACJA owcy
            iloscitemo++;
            health = 70.0f;  
            isChopped = false;  // niemartwa
            sprite.setColor(sf::Color(255, 255, 255, 255));
            sprite.setTexture(*texture);
            sprite.setPosition(spawnPosition);  // pierwotna pozyc
            position = spawnPosition;  //ROZWIAZANIE BUGOWANIA 
           
            // Tutaj możesz dodać logikę do wypuszczania przedmiotu
        }
             
        
        return;  // Jeżeli owca dead to nic nie robie
    }

    // bieganie owcy
    if (isRunning) {
        sprite.setTexture(runTexture); 

       
        sf::Vector2f direction = position - playerPosition;  // Wektor od gracza do owcy
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length != 0) {
            direction /= length;  // uwaga przed bledem
        }

        float speed = 0.12f;  // Szybkość ucieczki

        // ucieka owca
        position += direction * speed;

        // kolizja owca odbija sie od blokow
        for (const auto& object : objects) 
        {
            if (object.getBounds().intersects(sprite.getGlobalBounds())) 
            {
                
                position -= direction * speed;  // cofam owce
                break;  
            }
        }

        
        sprite.setPosition(position);

        // chodzienie anim
        if (animationClock.getElapsedTime().asSeconds() > frameTime) 
        {
            animationClock.restart();
            sprite.setTextureRect(sf::IntRect(frameIndex * 128, 0, 128, 128));
            frameIndex = (frameIndex + 1) % 6;
        }

        
        if (runClock.getElapsedTime().asSeconds() > 3.0f)  // po 3 sekundach przestaje owca uciekać 
        {
            isRunning = false;
            sprite.setTexture(*texture);  // Powrot do idle
            frameIndex = 0;
        }
    }
    else {
        //jak nic to idle
        if (animationClock.getElapsedTime().asSeconds() > frameTime) {
            animationClock.restart();
            sprite.setTextureRect(sf::IntRect(frameIndex * 128, 0, 128, 128));
            frameIndex = (frameIndex + 1) % 8;
        }
    }
}




void Sheep::takeDamage(int damage) {
    
    if (!isChopped)
    {
        health -= damage;

        if (health <= 0) //zabita
        {
            health = 0;
            isChopped = true;
            respawnClock.restart(); //15sekund respawn
            sprite.setColor(sf::Color(255, 255, 255, 100)); // Owca "zanika"
            std::cout << "Sheep zabita" << std::endl;
            if (iloscitemo == 1)
            {

                std::cout << "wypadl item" << std::endl;
                hasItemDropped = true;
                std::cout << "owca dalem item" << std::endl;
                iloscitemo--;

            }
        }

        else
        {
            isRunning = true; 
            runClock.restart();
            frameIndex = 0;
            std::cout << "Sheep run away"<<std::endl;
        }
    }
    else
    {
        hasItemDropped = false;
    }
}


void Sheep::draw(sf::RenderWindow& window) {
    if (!isChopped) {
        window.draw(sprite);
    }
}

int Sheep::getHealth() const {
    return health;
}

sf::Vector2f Sheep::getPosition() const {
    return position;
}
bool Sheep::hasDroppedItem1() const {
    return hasItemDropped;
    
}

GoldMine::GoldMine(float x, float y, int health, sf::Texture* destroy, std::vector<Collision::Object>& objects) :Tree(x,y,health,destroy,objects)
{
    
    if (!active.loadFromFile("assets/TekU/mobs/GoldMine_active.png")) {
        std::cerr << "blad nie ma tekstury aktywnej kopalni" << std::endl;
    }

    if (!inactive.loadFromFile("assets/TekU/mobs/GoldMine_Inactive.png")) {
        std::cerr << "blad nie ma tekstury nieaktywnej kopalni" << std::endl;
    }

    this->texture = destroy;
    sprite.setTexture(*texture);
    sprite.setPosition(x, y);
    sprite.setOrigin(96, 96);
    Collision::removeLastObject(objects);
    builded = false;
    isActive = false;

    

    

}
void GoldMine::build(int damage) {
    
    if(health>0) 
    {
        
        health -= damage;
        std::cout << "Zdrowie kopalni: " << health << std::endl;

        builded = true;
        
        std::cout << "Kopalnia odbudowana" << std::endl;
        sprite.setTexture(inactive);
        droptime.restart();


        isActive = false;
        
    }        
   
}
void GoldMine::update() {
    if (!isActive && builded) {
        
        
        hasItemDropped = false;
        std::cout << "upadete" << std::endl;
        if (droptime.getElapsedTime().asSeconds() >= 5.0f) // Po 5 sekundach kopalnia active
        {
            iloscitemoG++;
            
            isActive = true;
            
            builded = true;
           
            health = 1;
            
            sprite.setTexture(active);
            
            if (iloscitemoG == 1)
            {
                
                hasItemDropped = true;
                iloscitemoG--;
                std::cout << "Gracz zebrał item z kopalni!" << std::endl;
                
                droptime.restart();
            }

            std::cout << "Kopalnia została odbudowana i jest aktywna!" << std::endl;

        }
        else {
            
        
              std::cout << "Kopalnia nieaktywna (" << 5 - (int)droptime.getElapsedTime().asSeconds() << "s)" << std::endl;
  

        }
    }
}

void GoldMine::draw(sf::RenderWindow& window)
{
   
    
    window.draw(sprite);
}

int GoldMine::getHealth1() const {
    return health;
}

sf::Vector2f GoldMine::getPosition() const {
    return position;
}
bool GoldMine::hasDroppedItem2() const {
    return hasItemDropped;

}
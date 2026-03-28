#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(const sf::Vector2u& mapSize, const sf::Vector2u& screenSize);

    void update(const sf::Vector2f& playerPosition,float playerHealth);
    void apply(sf::RenderWindow& window);
    void drawUI(sf::RenderWindow& window, int sheepCount, int treecount, int goldCount);

    void setHealth(float hp); //wstepne hp
    void setBossHelath(float hpb,sf::RenderWindow& window);
    void ItemUI(int sheepCount, int treecount, int goldCount,sf::RenderWindow& window);

private:
    sf::View view;
    sf::Vector2u mapSize;
    sf::Vector2u screenSize;
    float halfWidth;
    float halfHeight;
    sf::RectangleShape healthBarBg; // T³o paska
    sf::RectangleShape healthBar;   // Pasek hp
    sf::Texture heartTexture;       // ikon Tekstura
    sf::Sprite heartIcon;           // ikona serca
    float health;

    sf::Texture Eq;
    sf::Sprite Eqs;
    sf::Sprite Eqs1;
    sf::Sprite Eqs2;
    sf::Font font;



    sf::Sprite iconSprite;
    sf::Text licznikText;

    sf::Texture miesoTexture;

    sf::Texture drzewaTexture;

    sf::Texture goldTexture;

    float hpb;
    bool hpBoss = false;
    float bossHP = 1000.0f;
    float bossMaxHP = 1000.0f;
    sf::RectangleShape healthBarboss;
    sf::RectangleShape backgroundBar;
    sf::Text bossText;
};

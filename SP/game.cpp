#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
RenderWindow window(sf::VideoMode(2560, 1440), "Game", sf::Style::Fullscreen);
Event event;
RectangleShape bg;
Texture bgTexture;
Sprite bgSprite;
float bgCounter = 0;

void bgSetup() 
{
    bgTexture.setRepeated(true);
    bg.setSize(sf::Vector2f(2560.f, 1440.f));
    bgTexture.loadFromFile("Textures/BG Sprite Sheet.png");
    bgSprite.setTexture(bgTexture);
}

void windowclose() 
{
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void windowfunction() 
{

    while (window.isOpen())
    {
        bgCounter += 0.0075;
        if (bgCounter > 8)
            bgCounter -= 8;
        bgSprite.setTextureRect(IntRect((int)bgCounter * 2560, 0, 2560, 1440));
        windowclose();
        window.clear();
        window.draw(bgSprite);
        window.display();
    }
}

int main()
{
    bgSetup();
    windowfunction();
    return 0;
}
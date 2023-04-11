#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;
RenderWindow window(sf::VideoMode(1920, 1080), "Game");
Event event;
RectangleShape player, ground[100];
Texture bgTexture;
Sprite bgSprite;
float bgCounter = 0;
View view(Vector2f(0, 0), Vector2f(1920, 1080));
Mouse ms;

// DECLRATIONS
void bgSetup();
void windowclose();
void windowfunction();

int main()
{
    bgSetup();
    windowfunction();
    return 0;
}


//DEFINITIONS
void bgSetup() 
{
    player.setSize(Vector2f(50, 50));
    player.setPosition(600, 600);
    bgTexture.loadFromFile("Level 1-A BG.png");
    bgSprite.setTexture(bgTexture);
    bgSprite.setPosition(-370, -53);
}
void windowfunction()
{

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Key::Right) && player.getPosition().x < 9063 - 370 - 50) {
            player.move(20, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Left) && player.getPosition().x > -370) {
            player.move(-20, 0);
        }
        if (player.getPosition().x <= 7740 && player.getPosition().x >= 600)
            view.setCenter(player.getPosition());
        else if (player.getPosition().x > 7740) {
            view.setCenter(7741, player.getPosition().y);
        }
        else if (player.getPosition().x < 600)
            view.setCenter(599, player.getPosition().y);
        windowclose();
        window.clear();
        window.draw(bgSprite);
        window.draw(player);
        window.setView(view);
        window.display();
    }
}
void windowclose() 
{
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
            window.close();
        }
    }
}
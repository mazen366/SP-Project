#include <SFML/Graphics.hpp>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
using namespace std;
using namespace sf;
RenderWindow window(sf::VideoMode(1920, 1080), "Game");
Event event;
RectangleShape player, ground[100];
Texture bgTexture;
Sprite bgSprite;
float bgCounter = 0;
bool isBlackscreen=false;
Clock blackscreenTimer;
View view(Vector2f(0, 0), Vector2f(1920, 1080));

struct Player
{
    Texture Playertex;
    Sprite PlayerSprite;
}mc;

// DECLRATIONS
void Playersetup(Player&);
void bgSetup();
void windowclose();
void windowfunction();
void cameraView();
bool canMoveRight(RectangleShape,int);
bool canMoveleft(RectangleShape object,int xPoisition);

int main()
{
    Playersetup(mc);
    bgSetup();
    windowfunction();
    return 0;
}


//DEFINITIONS
void bgSetup()
{
    int bgCounter=0;
    player.setSize(Vector2f(50, 50));
    player.setPosition(600, 600);
    bgTexture.loadFromFile(resourcePath()+"Level 1-A BG.png");
    bgSprite.setTexture(bgTexture);
    bgSprite.setPosition(-370, -53);
}
void windowfunction()
{

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Key::Right) && canMoveRight(player,8643))
        {
            player.move(0.25, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Left) && canMoveleft(player, -370))
        {
            player.move(-0.25, 0);
        }
        cameraView();
        windowclose();
        window.clear();
        window.draw(bgSprite);
        window.draw(player);
        window.draw(mc.PlayerSprite);
        window.setView(view);
        if(player.getPosition().x>8633)
        {
            window.clear(Color::Black);
            window.display();
            player.setPosition(600, 600);
            sleep(2);
        }
        else
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
void cameraView()
{
    if (player.getPosition().x <= 7740 && player.getPosition().x >= 600)
        view.setCenter(player.getPosition());
    else if (player.getPosition().x > 7740) {
        view.setCenter(7741, player.getPosition().y);
    }
    else if (player.getPosition().x < 600)
        view.setCenter(599, player.getPosition().y);
    
}
void Playersetup(Player& mc)
{
    
    mc.Playertex.loadFromFile(resourcePath()+"Yellow Naruto.png");
    mc.PlayerSprite.setTexture(mc.Playertex);
    mc.PlayerSprite.setPosition(0, 0);
    mc.PlayerSprite.setScale(5, 5);
}
bool canMoveRight(RectangleShape object,int xPoisition)
{
    if(object.getPosition().x < xPoisition)
        return true;
    
    return false;
    
}

bool canMoveleft(RectangleShape object,int xPoisition)
{
    if(object.getPosition().x > xPoisition)
        return true;
    
    return false;
    
}

#include <SFML/Graphics.hpp>
#include<thread>
#include <iostream>
using namespace std;
using namespace sf;
RenderWindow window(sf::VideoMode(1920, 1080), "Game");
Event event;
RectangleShape player, ground[100];
Texture bgTexture[30];
Sprite bgSprite[30];
int bgCounter = 0, leftx = -370, rightx = 8643;
bool isBlackscreen = false, isMoved = false;
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
void transition();
void transition_reverse();
bool canMoveRight(RectangleShape, int);
bool canMoveleft(RectangleShape object, int xPoisition);

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
    player.setSize(Vector2f(50, 50));
    player.setPosition(600, 600);
    ground[0].setSize(Vector2f(9200, 30));
    ground[0].setPosition(-370, 800);
    bgTexture[0].loadFromFile("Level 1-A BG.png");
    bgSprite[0].setTexture(bgTexture[0]);
    bgSprite[0].setPosition(-370, -53);

    ground[1].setSize(Vector2f(9200, 30));
    ground[1].setPosition(-370, 800);
    bgTexture[1].loadFromFile("Level 1-B-1 BG.png");
    bgSprite[1].setTexture(bgTexture[1]);
    bgSprite[1].setPosition(10000, 0);

}
void windowfunction()
{

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Key::Right) && canMoveRight(player, rightx))
        {
            player.move(20, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Left) && canMoveleft(player, leftx))
        {
            player.move(-20, 0);
        }
        cameraView();
        windowclose();
        window.clear();
        window.draw(bgSprite[0]);
        window.draw(bgSprite[1]);
        window.draw(player);
        // window.draw(mc.PlayerSprite);
        window.setView(view);
        if (player.getPosition().x > 8633 && bgCounter == 0)
        {
            transition();
            transition_reverse();
            bgCounter = 1;
            this_thread::sleep_for(chrono::milliseconds(300));

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
    if (bgCounter == 0) {
        if (player.getPosition().x <= 7740 && player.getPosition().x >= 600)
            view.setCenter(player.getPosition());
        else if (player.getPosition().x > 7740) {
            view.setCenter(7741, player.getPosition().y);
        }
        else if (player.getPosition().x < 600)
            view.setCenter(599, player.getPosition().y);
    }
    else if (bgCounter == 1) 
    {
        if (!isMoved)
        {
            player.setPosition(10400, 600);
            isMoved = true;
        }
        leftx = 10000;
        rightx = 14771;
        //4771    1192
        view.setCenter(player.getPosition());
    }

}
void Playersetup(Player& mc)
{

    // mc.Playertex.loadFromFile("Yellow Naruto.png");
     //mc.PlayerSprite.setTexture(mc.Playertex);
    // mc.PlayerSprite.setPosition(0, 0);
    // mc.PlayerSprite.setScale(5, 5);
}
void transition()
{
    Clock t1;
    for (int i = 4; i <= 7; i++)
    {
        window.clear(Color{ Uint8(84 - 12 * i), Uint8(84 - 12 * i), Uint8(84 - 12 * i) });
        window.display();
        while (true)
        {
            if (t1.getElapsedTime().asMilliseconds() > 300)
            {
                t1.restart();
                break;
            }
        }
    }
}
void transition_reverse()
{
    Clock t1;
    for (int i = 0; i < 4; i++)
    {
        window.clear(Color{ Uint8(12 * i), Uint8(12 * i), Uint8(12 * i) });
        window.display();
        while (true)
        {
            if (t1.getElapsedTime().asMilliseconds() > 300)
            {
                t1.restart();
                break;
            }
        }
    }
}
bool canMoveRight(RectangleShape object, int xPoisition)
{
    if (object.getPosition().x <= xPoisition)
        return true;

    return false;

}

bool canMoveleft(RectangleShape object, int xPoisition)
{
    if (object.getPosition().x >= xPoisition)
        return true;

    return false;

}
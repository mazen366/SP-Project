#include <SFML/Graphics.hpp>
#include<thread>
#include <iostream>
using namespace std;
using namespace sf;

RenderWindow window(sf::VideoMode(1920, 1080), "Game");
Event event;

//player
RectangleShape player, ground[30], wall[30];

// plVelocity 
Vector2f playerVelocity = { 0,0 };

Texture bgTexture[30];
Sprite bgSprite[30];

int bgCounter = 0, leftx = -370, rightx = 8643;
bool isBlackscreen = false, isMoved = false;
Clock blackscreenTimer;
View view(Vector2f(0, 0), Vector2f(1920, 1080));

//animation backGround lv1 a 
Texture lampTex;
Sprite lamp;
float lampIndecator = 0;

struct Player
{
    Texture Playertex;
    Sprite PlayerSprite;
}mc;

// DECLRATIONS
//void lampAnimation();
void plmovement();
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

    ground[1].setSize(Vector2f(2889 + 3000, 30));
    ground[1].setPosition(10000, 907);
    bgTexture[1].loadFromFile("Level 1-B-1 BG.png");
    bgSprite[1].setTexture(bgTexture[1]);
    bgSprite[1].setPosition(10000, -50);

    bgTexture[2].loadFromFile("Level 1-B-2 BG.png");
    bgSprite[2].setTexture(bgTexture[2]);
    bgSprite[2].setPosition(14771, -940);
    //15840  776
    wall[0].setSize(Vector2f(10, 907 - 776));
    wall[0].setPosition(15840, 776);
    //animation BG lv 1 a
   // lampTex.loadFromFile("Level 1-A Lamps.png");
  //  lamp.setTexture(lampTex);
  //  lamp.setTextureRect(IntRect(0, 0, 9063 , 1192));
 //   lamp.setPosition(Vector2f(0, 0));

}
void windowfunction()
{

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Key::W)) {
            cout << player.getPosition().x << "  " << Mouse::getPosition(window).y << endl;
        }
        plmovement();
        // lampAnimation();
        cameraView();
        windowclose();
        window.clear();
        for (int i = 0; i < 3; i++)
            window.draw(bgSprite[i]);
        window.draw(lamp);
        window.draw(player);
        window.draw(ground[0]);
        window.draw(ground[1]);
        window.draw(wall[0]);
        // window.draw(mc.PlayerSprite);
        //15840  776
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
        leftx = 10020;
        rightx = 17551;
        if (player.getPosition().x <= 14771 + 1900 && player.getPosition().x >= 11000)
            view.setCenter(player.getPosition());
        else if (player.getPosition().x > 14771 + 1900) {
            view.setCenter(14771 + 1900, player.getPosition().y);
        }
        else if (player.getPosition().x < 11000)
            view.setCenter(10999, player.getPosition().y);
    }

}
void Playersetup(Player& mc)
{

    mc.Playertex.loadFromFile("Yellow Naruto.png");
    mc.PlayerSprite.setTexture(mc.Playertex);
    mc.PlayerSprite.setPosition(0, 0);
    mc.PlayerSprite.setScale(5, 5);
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
void plmovement()
{

    if (Keyboard::isKeyPressed(Keyboard::Key::Right) && canMoveRight(player, rightx))
    {
        playerVelocity.x = 20;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Key::Left) && canMoveleft(player, leftx))
    {
        playerVelocity.x = -20;
    }
    else
    {
        playerVelocity.x = 0;
    }
    player.move(playerVelocity);
}
//void lampAnimation()
//{
//    lampIndecator += 1;
//   	if (lampIndecator >= 2)
//    			lampIndecator = 0;
//    lamp.setTextureRect(IntRect((short int)lampIndecator * (9063), 0,9063 , 1192));
//}
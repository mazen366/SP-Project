#include <SFML/Graphics.hpp>
#include<thread>
#include <iostream>
using namespace std;
using namespace sf;

RenderWindow window(sf::VideoMode(1920, 1080), "Game");
Event event;

//player
struct Player
{
    RectangleShape playerRec;
    Texture playerTex;
    Sprite playerSprite;
};
Player player;

//ground & wall
RectangleShape ground[30], wall[30];

// plVelocity
Vector2f playerVelocity = { 0,0 };

Texture bgTexture[30];
Sprite bgSprite[30];

int bgCounter = 0, leftx = -370, rightx = 8643;
bool isBlackscreen = false, isMoved = false, ismoved2 = 0, ismoved3 = 0;
Clock blackscreenTimer;
View view(Vector2f(0, 0), Vector2f(1920, 1080));

//animation backGround lv1 a


//make clock & timer to plmovement

// DECLRATIONS
void plmovement();
void Playersetup();
void bgSetup();
void windowclose();
void windowfunction();
void cameraView();
void transition();
void transition_reverse();
bool canMoveRight(RectangleShape, int);
bool canMoveleft(RectangleShape object, int xPoisition);
void create(RectangleShape arr[], int index, int sizeX, int sizeY, int xPosition, int yPostions);

int main()
{
    window.setFramerateLimit(60);
    bgSetup();
    windowfunction();
    return 0;
}
//DEFINITIONS
void bgSetup()
{

    player.playerRec.setSize(Vector2f(50, 50));
    player.playerRec.setPosition(600, 600);
    ground[0].setSize(Vector2f(9200, 30));
    ground[0].setPosition(-370, 800);
    bgTexture[0].loadFromFile( "Level 1-A BG.png");
    bgSprite[0].setTexture(bgTexture[0]);
    bgSprite[0].setPosition(-370, -53);

    ground[1].setSize(Vector2f(5939, 30));
    ground[1].setPosition(10000, 907);
    bgTexture[1].loadFromFile("Level 1-B-1 BG.png");
    bgSprite[1].setTexture(bgTexture[1]);
    bgSprite[1].setPosition(10000, -50);
    
    create(ground, 2, 300, 18, 15930, 840);

    create(ground, 3, 150, 10, 16230, 625);

    create(ground, 4, 160, 10, 16370, 390);

    create(ground, 5, 325, 18, 16520, 173);

    create(ground, 6, 700, 10, 16830, 40);
    
    bgTexture[2].loadFromFile("Level 1-B-2 BG.png");
    bgSprite[2].setTexture(bgTexture[2]);
    bgSprite[2].setPosition(14771, -940);
   
    
    create(wall, 0, 10, 80, 15930, 860);

    create(wall, 1, 10, 220, 16230, 635);

    create(wall, 2, 10, 220, 16370, 404);

    create(wall, 3, 10, 200, 16520, 193);

    create(wall, 4, 10, 140, 16830, 40);
    
    
    bgTexture[3].loadFromFile("Level 1-C BG.png");
    bgSprite[3].setTexture(bgTexture[3]);
    bgSprite[3].setPosition(18000, 0);

    ground[7].setSize(Vector2f(1640, 20));
    ground[7].setPosition(18000, 970);

    bgTexture[4].loadFromFile("Level 1-D BG.png");
    bgSprite[4].setTexture(bgTexture[4]);
    bgSprite[4].setPosition(20000, 0);

}
void windowfunction()
{

    while (window.isOpen())
    {
        
        plmovement();
        // lampAnimation();
        cameraView();
        windowclose();
        window.clear();
        for (int i = 0; i < 5; i++)
            window.draw(bgSprite[i]);
        window.draw(player.playerRec);
        for (int i = 0; i < 8; i++)
            window.draw(ground[i]);
        for (int i = 0; i < 5; i++)
            window.draw(wall[i]);;
        // window.draw(mc.PlayerSprite);
        window.setView(view);
        if (player.playerRec.getPosition().x > rightx && bgCounter == 0)
        {
            transition();
            transition_reverse();
            bgCounter = 1;
            this_thread::sleep_for(chrono::milliseconds(300));

        }
        else if (player.playerRec.getPosition().x > rightx && bgCounter == 1)
        {
            transition();
            transition_reverse();
            bgCounter = 2;
            this_thread::sleep_for(chrono::milliseconds(300));

        }
        else if (player.playerRec.getPosition().x > rightx && bgCounter == 2)
        {
            transition();
            transition_reverse();
            bgCounter = 3;
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
        if (player.playerRec.getPosition().x <= 7740 && player.playerRec.getPosition().x >= 600)
            view.setCenter(player.playerRec.getPosition());
        else if (player.playerRec.getPosition().x > 7740) {
            view.setCenter(7741, player.playerRec.getPosition().y);
        }
        else if (player.playerRec.getPosition().x < 600)
            view.setCenter(599, player.playerRec.getPosition().y);
    }
    else if (bgCounter == 1)
    {
        if (!isMoved)
        {
            player.playerRec.setPosition(10400, 600);
            isMoved = true;
        }
        leftx = 10020;
        rightx = 17551;
        if (player.playerRec.getPosition().x <= 16671  && player.playerRec.getPosition().x >= 11000)
            view.setCenter(player.playerRec.getPosition());
        else if (player.playerRec.getPosition().x > 16671) {
            view.setCenter(16671, player.playerRec.getPosition().y);
        }
        else if (player.playerRec.getPosition().x < 11000)
            view.setCenter(10999, player.playerRec.getPosition().y);
    }
    else if (bgCounter == 2)
    {
        if (!ismoved2)
        {
            player.playerRec.setPosition(18400, 800);
            ismoved2 = true;
        }
        leftx = 18100;
        rightx = 19500;
        view.setCenter(18820,596);
        view.setSize(1600, 1080);
    }
    else if (bgCounter == 3)
    {
        if (!ismoved3)
        {
            view.setSize(1920, 1180);
            player.playerRec.setPosition(20400, 600);
            ismoved3 = true;
        }
        leftx = 20000;
        rightx = 24771;
        view.setCenter(player.playerRec.getPosition());
        //if (player.playerRec.getPosition().x <= 14771 + 1900 && player.playerRec.getPosition().x >= 11000)
        //    view.setCenter(player.playerRec.getPosition());
        //else if (player.playerRec.getPosition().x > 14771 + 1900) {
        //    view.setCenter(14771 + 1900, player.playerRec.getPosition().y);
        //}
        //else if (player.playerRec.getPosition().x < 11000)
        //    view.setCenter(10999, player.playerRec.getPosition().y);
    }

}
void Playersetup()
{

    player.playerTex.loadFromFile("Yellow Naruto.png");
    player.playerSprite.setTexture(player.playerTex);
    player.playerSprite.setPosition(0, 0);
    player.playerSprite.setScale(5, 5);
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



    if (Keyboard::isKeyPressed(Keyboard::Key::Right) && canMoveRight(player.playerRec, rightx))
    {
        playerVelocity.x = 20;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Key::Left) && canMoveleft(player.playerRec, leftx))
    {
        playerVelocity.x = -20;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Key::Down))
    {
        playerVelocity.y = 20;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Key::Up))
    {
        playerVelocity.y = -20;
    }
    else
    {
        playerVelocity.x = 0;
        playerVelocity.y = 0;

    }
    player.playerRec.move(playerVelocity);
}
void create(RectangleShape arr[], int index, int sizeX, int sizeY, int xPosition, int yPostions)
{
    arr[index].setSize(Vector2f(sizeX, sizeY));
    arr[index].setPosition(xPosition, yPostions);
}

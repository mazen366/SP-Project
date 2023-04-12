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
bool isBlackscreen = false, isMoved = false;
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
    bgTexture[0].loadFromFile(resourcePath()+"Level 1-A BG.png");
    bgSprite[0].setTexture(bgTexture[0]);
    bgSprite[0].setPosition(-370, -53);

    ground[1].setSize(Vector2f(2889 + 3000, 30));
    ground[1].setPosition(10000, 907);
    bgTexture[1].loadFromFile(resourcePath()+"Level 1-B-1 BG.png");
    bgSprite[1].setTexture(bgTexture[1]);
    bgSprite[1].setPosition(10000, -50);
    
    ground[2].setSize(Vector2f( 300,5));
    ground[2].setPosition(15840, 766);
    
    ground[3].setSize(Vector2f( 200,5));
    ground[3].setPosition(16140, 545);
    
    ground[4].setSize(Vector2f( 160,5));
    ground[4].setPosition(16340, 364);
    
    ground[5].setSize(Vector2f( 350,5));
    ground[5].setPosition(16500, 173);
    
    ground[6].setSize(Vector2f( 700,5));
    ground[6].setPosition(16850, 63);
    
    bgTexture[2].loadFromFile(resourcePath()+"Level 1-B-2 BG.png");
    bgSprite[2].setTexture(bgTexture[2]);
    bgSprite[2].setPosition(14771, -940);
    //15840  776
    wall[0].setSize(Vector2f(10, 131));
    wall[0].setPosition(15840, 776);
    
    wall[1].setSize(Vector2f(10, 231));
    wall[1].setPosition(16140, 545);
    
    wall[2].setSize(Vector2f(10, 181));
    wall[2].setPosition(16340, 364);
    
    wall[3].setSize(Vector2f(10, 191));
    wall[3].setPosition(16500, 173);
    
    wall[3].setSize(Vector2f(10, 191));
    wall[3].setPosition(16500, 173);
    
    wall[4].setSize(Vector2f(10, 110));
    wall[4].setPosition(16850, 63);
}
void windowfunction()
{

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Key::W)) {
            cout << player.playerRec.getPosition().x << "  " << Mouse::getPosition(window).y << endl;
        }
        plmovement();
        // lampAnimation();
        cameraView();
        windowclose();
        window.clear();
        for (int i = 0; i < 3; i++)
            window.draw(bgSprite[i]);
        window.draw(player.playerRec);
        for (int i = 0; i < 7; i++)
            window.draw(ground[i]);
        for (int i = 0; i < 5; i++)
            window.draw(wall[i]);;
        // window.draw(mc.PlayerSprite);
        //15840  776
        window.setView(view);
        if (player.playerRec.getPosition().x > 8633 && bgCounter == 0)
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
        if (player.playerRec.getPosition().x <= 14771 + 1900 && player.playerRec.getPosition().x >= 11000)
            view.setCenter(player.playerRec.getPosition());
        else if (player.playerRec.getPosition().x > 14771 + 1900) {
            view.setCenter(14771 + 1900, player.playerRec.getPosition().y);
        }
        else if (player.playerRec.getPosition().x < 11000)
            view.setCenter(10999, player.playerRec.getPosition().y);
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
    else
    {
        playerVelocity.x = 0;
    }
    player.playerRec.move(playerVelocity);
}

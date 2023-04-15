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
    float plcounter = 0;
    Texture playerTex;
    Sprite playerSprite;
    RectangleShape rec;//make rectangle to better collision
    Vector2f Velocity = { 0,0 };
    Texture playerHPTex;
    Sprite playerHPSprite;
    float health = 100.0;
};
Player player;

//ground & wall
RectangleShape ground[30], wall[30];

//gravity
float gravity = 0.7;
bool canDoubleJump;

//make clock & timer to plmovement
Clock clock_pl;
float dt = 0;
float delay = 0.1f;
float timer = 0;

//background Textures&Sprites
Texture bgTexture[30];
Sprite bgSprite[30];

int bgCounter = 0, leftx = -300, rightx = 8643;
bool isBlackscreen = false, isMoved = false, ismoved2 = 0, ismoved3 = 0;
Clock blackscreenTimer;
View view(Vector2f(0, 0), Vector2f(1920, 1080));

//lvl 1-b-2 animation Exit lamp
Texture  ExitlampTex;
Sprite  Exitlamp;
float  ExitLampIndecator = 0;

//Lvl 1-C animation Fire Torches
Texture FireTrochestex;
Sprite FireTroches;
float FireTrochesIndecator = 0;

//lvl 1 FG
Texture lvl1FGtex[30];
Sprite Lvl1FG[30];

// DECLRATIONS
void BGanimation();
void plmovement();
void onlymove();
void jump();
void move_with_animation();
void Playersetup();
void bgSetup();
void windowclose();
void windowfunction();
void cameraView();
void transition();
void transition_reverse();
void transition_pos_check();
bool canMoveRight(Sprite, int);
bool canMoveleft(Sprite object, int xPoisition);
bool collisonPl(RectangleShape[], int);
void create(RectangleShape arr[], int index, int sizeX, int sizeY, int xPosition, int yPostions);


int main()
{
    window.setFramerateLimit(60);
    bgSetup();
    Playersetup();
    windowfunction();
    return 0;
}
//DEFINITIONS
void bgSetup()
{

    ground[0].setSize(Vector2f(8830, 30));
    ground[0].setPosition(-370, 800);
    bgTexture[0].loadFromFile("Level 1-A BG.png");
    bgSprite[0].setTexture(bgTexture[0]);
    bgSprite[0].setPosition(-370, -53);
    //lvl 1 ->A FG
    lvl1FGtex[0].loadFromFile("Level 1-A FG.png");
    Lvl1FG[0].setTexture(lvl1FGtex[0]);
    Lvl1FG[0].setPosition(-370, -53);

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

    //animation lv1-B-2(Exit lamp)
    ExitlampTex.loadFromFile("Level 1-B-2 Exit Lamp.png");
    Exitlamp.setTexture(ExitlampTex);
    Exitlamp.setTextureRect(IntRect(0, 0, 17328 / 6, 2087));
    Exitlamp.setPosition(14771, -940);

    create(wall, 0, 20, 80, 15930, 860);

    create(wall, 1, 20, 220, 16230, 635);

    create(wall, 2, 20, 220, 16370, 404);

    create(wall, 3, 20, 200, 16520, 193);

    create(wall, 4, 20, 140, 16830, 40);


    bgTexture[3].loadFromFile("Level 1-C BG.png");
    bgSprite[3].setTexture(bgTexture[3]);
    bgSprite[3].setPosition(18000, 0);

    //fire troches lv1 ->c
    FireTrochestex.loadFromFile("Level 1-C Fire Torches.png");
    FireTroches.setTexture(FireTrochestex);
    FireTroches.setTextureRect(IntRect(0, 0, 13120 / 8, 1192));
    FireTroches.setPosition(18000, 0);
    //lvl 1 ->c FG
    lvl1FGtex[3].loadFromFile("Level 1-C FG.png");
    Lvl1FG[3].setTexture(lvl1FGtex[3]);
    Lvl1FG[3].setPosition(18000, 0);

    ground[7].setSize(Vector2f(1640, 20));
    ground[7].setPosition(18000, 970);

    bgTexture[4].loadFromFile("Level 1-D BG.png");
    bgSprite[4].setTexture(bgTexture[4]);
    bgSprite[4].setPosition(20000, 0);
    //4771 × 1192
    ground[8].setSize(Vector2f(4771, 20));
    ground[8].setPosition(20000, 900);

}
void windowfunction()
{

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(sf::Keyboard::T))
            player.playerSprite.setPosition(20000, 800);
        dt = clock_pl.getElapsedTime().asMicroseconds();
        dt /= 750;
        clock_pl.restart();
        cameraView();
        plmovement();
        player.playerHPSprite.setPosition(view.getCenter().x - 960, view.getCenter().y - 500);
        BGanimation();
        windowclose();
        window.clear();
        for (int i = 0; i < 5; i++)
            window.draw(bgSprite[i]);
        //window.draw(Exitlamp);
        window.draw(FireTroches);

        window.draw(player.playerSprite);
        /* for (int i = 0; i <= 8; i++)
             window.draw(ground[i]);
         for (int i = 0; i <=6; i++)
             window.draw(wall[i]);
        */
        //window.draw(player.rec);
        window.draw(player.playerSprite);
        window.draw(Lvl1FG[0]);
        window.draw(Lvl1FG[3]);
        window.draw(player.playerHPSprite);
        window.setView(view);
        transition_pos_check();
    }
}
void BGanimation()
{
    //Exite lamp;
    ExitLampIndecator += 0.17 * dt;
    if (ExitLampIndecator > 6)
        ExitLampIndecator = 0;
    Exitlamp.setTextureRect(IntRect(int(ExitLampIndecator) * (17328 / 6), 0, 17328 / 6, 2087));

    //Fire Troches
    FireTroches.setTextureRect(IntRect((int)FireTrochesIndecator * 13120 / 8, 0, 13120 / 8, 1192));
    FireTrochesIndecator += 0.01 * dt;
    if (FireTrochesIndecator > 8)
        FireTrochesIndecator = 0;

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


        if (player.playerSprite.getPosition().x <= 7740 && player.playerSprite.getPosition().x >= 600)
            view.setCenter(player.playerSprite.getPosition().x, 600);
        else if (player.playerSprite.getPosition().x > 7740)
            view.setCenter(7741, 600);
        else if (player.playerSprite.getPosition().x < 600)
            view.setCenter(599, 600);




    }
    else if (bgCounter == 1)
    {
        if (!isMoved)
        {
            player.playerSprite.setPosition(10400, 600);
            isMoved = true;
        }
        leftx = 10100;
        rightx = 17551;
        //14771

        if (player.playerSprite.getPosition().x <= 14771 && player.playerSprite.getPosition().x >= 11000)
            view.setCenter(player.playerSprite.getPosition().x, 600);
        else if (player.playerSprite.getPosition().x < 11000)
            view.setCenter(10999, 600);

        if (player.playerSprite.getPosition().x > 14771) {
            view.setCenter((14771 + rightx) / 2, 110);
            view.setSize(rightx - 14771, 2087);
        }
        else view.setSize(1920, 1080);
    }
    else if (bgCounter == 2)
    {
        if (!ismoved2)
        {
            player.playerSprite.setPosition(18400, 800);
            ismoved2 = true;
        }
        leftx = 18030;
        rightx = 19650;
        view.setCenter(18820, 596);
        view.setSize(1600, 1080);
    }
    else if (bgCounter == 3)
    {
        if (!ismoved3)
        {
            view.setSize(1920, 1080);
            player.playerSprite.setPosition(20400, 600);
            ismoved3 = true;
        }
        leftx = 20000;
        rightx = 24771;
        view.setSize(1920, 1190);
        if (player.playerSprite.getPosition().x <= 23800 && player.playerSprite.getPosition().x >= 20950)
            view.setCenter(player.playerSprite.getPosition().x, 600);
        else if (player.playerSprite.getPosition().x > 23800)
            view.setCenter(23801, 600);
        else if (player.playerSprite.getPosition().x < 20950)
            view.setCenter(20949, 600);
    }

}
void Playersetup()
{
    //sprite
    player.playerTex.loadFromFile("Running Sprite Sheet u.png");
    player.playerSprite.setTexture(player.playerTex);
    player.playerSprite.setTextureRect(IntRect(0, 0, 1324 / 12, 133));
    player.playerSprite.setPosition(600, 600);
    //rectangle
    player.rec.setPosition(player.playerSprite.getPosition().x - 50, player.playerSprite.getPosition().y);
    player.rec.setSize(Vector2f(75, 130));
    //HP
    player.playerHPTex.loadFromFile("Gradient_Health_Bar.png");
    player.playerHPSprite.setTexture(player.playerHPTex);
    player.playerHPSprite.setTextureRect(IntRect(0, 0, 204, 30));
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
void transition_pos_check()
{
    if (player.playerSprite.getPosition().x > rightx && bgCounter == 0)
    {
        transition();
        transition_reverse();
        bgCounter = 1;
        this_thread::sleep_for(chrono::milliseconds(300));

    }
    else if (player.playerSprite.getPosition().x > rightx && bgCounter == 1)
    {
        transition();
        transition_reverse();
        bgCounter = 2;
        this_thread::sleep_for(chrono::milliseconds(300));

    }
    else if (player.playerSprite.getPosition().x > rightx && bgCounter == 2)
    {
        transition();
        transition_reverse();
        bgCounter = 3;
        this_thread::sleep_for(chrono::milliseconds(300));

    }
    else
        window.display();
}
bool canMoveRight(Sprite object, int xPoisition)
{
    if (object.getPosition().x <= xPoisition && !collisonPl(wall, 4))
        return true;

    return false;

}

bool canMoveleft(Sprite object, int xPoisition)
{
    if (object.getPosition().x >= xPoisition)
        return true;

    return false;

}

void plmovement()
{
    //gravity
    player.playerSprite.setOrigin(1324 / 24, 0);
    if (!canMoveleft(player.playerSprite, leftx) || !canMoveRight(player.playerSprite, rightx)) {
        player.Velocity.x = 0;
        player.Velocity.y += gravity * 0.9;

    }
    if (collisonPl(wall, 4))
        player.Velocity.x = 0;

    if (!collisonPl(ground, 8))
    {
        if (Keyboard::isKeyPressed(Keyboard::A) && canDoubleJump)
        {
            //  cout << "A jump" << endl;
            jump();
            canDoubleJump = 0;
        }
        else
        {

        }
        player.Velocity.y += gravity * 0.9;
        onlymove();//function -> movement in air
    }
    else
    {

        move_with_animation();//functoin -> movement & animation
    }

    player.playerSprite.move(player.Velocity);
    player.rec.setPosition(player.playerSprite.getPosition().x - 50, player.playerSprite.getPosition().y);
}
void onlymove()
{

    if (Keyboard::isKeyPressed(Keyboard::Right) && canMoveRight(player.playerSprite, rightx))
    {

        player.Velocity.x = 0.17 * dt;
        player.playerSprite.setScale(1, 1);
        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            player.Velocity.x *= 2;
        }
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left) && canMoveleft(player.playerSprite, leftx))
    {

        player.Velocity.x = -0.17 * dt;
        player.playerSprite.setScale(-1, 1);
        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            player.Velocity.x *= 2;
        }
    }
    else
    {
        player.Velocity.x = 0;
    }

}
void move_with_animation()
{
    player.Velocity.y = 0;
    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
        jump();

        canDoubleJump = 1;
    }
    else
        canDoubleJump = 0;
    if (Keyboard::isKeyPressed(Keyboard::Left) && canMoveleft(player.playerSprite, leftx))
    {


        player.playerSprite.setScale(-1, 1);
        player.playerSprite.setTextureRect(IntRect((int)player.plcounter * (1324 / 12), 0, (1324 / 12), 133));
        player.plcounter += 0.01 * dt;
        if (player.plcounter > 11.9) player.plcounter = 0;
        player.Velocity.x = -0.17 * dt;
        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            player.Velocity.x *= 2;
        }
    }
    else if (Keyboard::isKeyPressed(Keyboard::Right) && canMoveRight(player.playerSprite, rightx))
    {

        player.playerSprite.setScale(1, 1);
        player.playerSprite.setTextureRect(IntRect((int)player.plcounter * (1324 / 12), 0, (1324 / 12), 133));
        player.plcounter += 0.01 * dt;
        if (player.plcounter > 11.9) player.plcounter = 0;
        player.Velocity.x = 0.17 * dt;
        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            player.Velocity.x *= 2;
        }
    }

    else
    {
        player.Velocity.x = 0;
    }

}
void jump()
{
    player.Velocity.y = -15;
    player.Velocity.x /= 2;
}
bool collisonPl(RectangleShape arr[], int size)
{
    for (int i = 0; i <= size; i++)
    {
        if (player.playerSprite.getGlobalBounds().intersects(arr[i].getGlobalBounds()))
            return true;
    }
    return false;

}


void create(RectangleShape arr[], int index, int sizeX, int sizeY, int xPosition, int yPostions)
{
    arr[index].setSize(Vector2f(sizeX, sizeY));
    arr[index].setPosition(xPosition, yPostions);
}
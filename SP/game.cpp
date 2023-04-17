#include <SFML/Graphics.hpp>
#include<thread>
#include <iostream>
using namespace std;
using namespace sf;

RenderWindow window(sf::VideoMode(1920, 1080), "Game");
Event event;

//player
const float idle = 0.001;
struct Player
{
    float plcounter = 0;
    Texture upperbodyTex, lowerbodyTex;
    Sprite upperbodySprite, lowerbodySprite;
    float upperlowerdistance;
    RectangleShape rec;//make rectangle to better collision
    Vector2f Velocity = { 0,0 };
    Texture playerHPTex;
    Sprite playerHPSprite;
    float health = 100.0;
    // shooting
    int arr[5] = {};
    float cooldown = 0;
    bool canshoot = 0;
    int last_key = 0;
};
Player player;
// shooting
struct Pistol
{
    float speed;
    RectangleShape coll; // hitbox 
    Texture pistol_Ammo_Tex;
    Sprite pistol_Ammo_Sprite;
    float cooldown;
};
Pistol pistol;


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

//Lvl 1-C animation Fire Torches
Texture FireTrochestex;
Sprite FireTroches;

float animiindecator[30];

//lvl 1 FG
Texture lvl1FGtex[30];
Sprite Lvl1FG[30];

// DECLRATIONS
void BGanimation();
void animation(Sprite& s, float maxframe, float x, float y, float delay, int index);
void plmovement(Sprite& s, float maxframe, float x, float y, float delay, int index);
void onlymove(Sprite& s);
void jump();
void move_with_animation(Sprite& s, float maxframe, float x, float y, float delay, int index);
void Pistolsetup(); // pistol setup
void moviebullet(Player&, Pistol&); // moving bullets
void cooldown(Player&); // cooldown for shooting
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
    Pistolsetup();
    Playersetup();
    windowfunction();
    return 0;
}
//DEFINITIONS

void bgSetup()
{
    // LEVEL 1 A SET UP
    bgTexture[0].loadFromFile("Level 1-A BG.png");
    bgSprite[0].setTexture(bgTexture[0]);
    bgSprite[0].setPosition(-370, -53);
    create(ground, 0, 8830, 30, -370, 800);
    //lvl 1 ->A FG
    lvl1FGtex[0].loadFromFile("Level 1-A FG.png");
    Lvl1FG[0].setTexture(lvl1FGtex[0]);
    Lvl1FG[0].setPosition(-370, -53);

    // LEVEL 1 B->1 SET UP
    bgTexture[1].loadFromFile("Level 1-B-1 BG.png");
    bgSprite[1].setTexture(bgTexture[1]);
    bgSprite[1].setPosition(10000, -50);
    create(ground, 1, 5939, 30, 10000, 907);
    create(ground, 2, 300, 18, 15930, 840);
    create(ground, 3, 150, 10, 16245, 625);
    create(ground, 4, 160, 10, 16380, 390);
    create(ground, 5, 325, 18, 16555, 173);
    create(ground, 6, 700, 10, 16850, 40);

    // LEVEL 1 B->2 SET UP
    bgTexture[2].loadFromFile("Level 1-B-2 BG.png");
    bgSprite[2].setTexture(bgTexture[2]);
    bgSprite[2].setPosition(14771, -940);

    //animation lv1-B-2(Exit lamp)
    ExitlampTex.loadFromFile("Level 1-B-2 Exit Lamp.png");
    Exitlamp.setTexture(ExitlampTex);
    Exitlamp.setTextureRect(IntRect(0, 0, 17328 / 6, 2087));
    Exitlamp.setPosition(14771, -940);

    //FG Lv1-B-2
    lvl1FGtex[2].loadFromFile("Level 1-B-2 FG.png");
    Lvl1FG[2].setTexture(lvl1FGtex[2]);
    Lvl1FG[2].setPosition(14771, -940);

    create(wall, 0, 20, 80, 15938, 870);
    create(wall, 1, 20, 220, 16235, 640);
    create(wall, 2, 20, 220, 16370, 409);
    create(wall, 3, 10, 200, 16520, 198);
    create(wall, 4, 10, 140, 16840, 60);

    //LEVEL 1 C SET UP   
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
    create(ground, 7, 1640, 20, 18000, 970);

    //LEVEL 1 D SET UP
    bgTexture[4].loadFromFile("Level 1-D BG.png");
    bgSprite[4].setTexture(bgTexture[4]);
    bgSprite[4].setPosition(20000, 0);
    create(ground, 8, 1050, 20, 20000, 900);
    create(ground, 9, 1200, 20, 21700, 900);
    create(ground, 10, 750, 20, 23120, 900);
    create(ground, 11, 800, 20, 24150, 900);
}
void windowfunction()
{

    while (window.isOpen())
    {
        //delta time
        dt = clock_pl.getElapsedTime().asMicroseconds();
        dt /= 750;
        clock_pl.restart();

        //shooting
        moviebullet(player, pistol);
        cooldown(player);

        if (Keyboard::isKeyPressed(Keyboard::R))
        {
            for (int i = 0; i < 5; i++)
            {
                player.arr[i] == 1;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::J))
        {
            player.cooldown = 30;
            player.canshoot = -1;
            pistol.coll.setPosition(Vector2f(player.upperbodySprite.getPosition().x + 30, player.upperbodySprite.getPosition().y + 50));

        }

        if (Keyboard::isKeyPressed(sf::Keyboard::T))
            player.upperbodySprite.setPosition(20000, 800);

        cameraView();
        plmovement(player.lowerbodySprite, 11.9, 408 / 12, 41, 0.004, 2);
        // plmovement(player.upperbodySprite, 11.9, 408/12, 41, 2);
        player.playerHPSprite.setPosition(view.getCenter().x - 960, view.getCenter().y - 500);
        BGanimation();
        windowclose();
        window.clear();
        for (int i = 0; i < 5; i++)
            window.draw(bgSprite[i]);
        //   window.draw(Exitlamp);
        window.draw(FireTroches);
        //window.draw(player.rec);
        window.draw(player.lowerbodySprite);
        window.draw(player.upperbodySprite);
        /*for (int i = 0; i < 7; i++)
            window.draw(ground[i]);
        for (int i = 0; i < 5; i++)
            window.draw(wall[i]);*/

        for (int i = 0; i < 4; i++)
        {
            if (i == 1)continue;
            window.draw(Lvl1FG[i]);
        }
        window.draw(player.playerHPSprite);
        window.draw(pistol.coll);
        window.setView(view);
        transition_pos_check();
    }
}
void BGanimation()
{
    animation(Exitlamp, 6, 17328 / 6, 2087, 0.01, 0);
    animation(FireTroches, 8, 13120 / 8, 1192, 0.01, 1);
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


        if (player.upperbodySprite.getPosition().x <= 7740 && player.upperbodySprite.getPosition().x >= 600)
            view.setCenter(player.upperbodySprite.getPosition().x, 600);
        else if (player.upperbodySprite.getPosition().x > 7740)
            view.setCenter(7741, 600);
        else if (player.upperbodySprite.getPosition().x < 600)
            view.setCenter(599, 600);

    }
    else if (bgCounter == 1)
    {
        if (!isMoved)
        {
            player.upperbodySprite.setPosition(10400, 600);
            player.lowerbodySprite.setPosition(10400, 600);
            isMoved = true;
        }
        leftx = 10100;
        rightx = 17651;
        //14771

        if (player.upperbodySprite.getPosition().x <= 14771 && player.upperbodySprite.getPosition().x >= 11000)
            view.setCenter(player.upperbodySprite.getPosition().x, 600);
        else if (player.upperbodySprite.getPosition().x < 11000)
            view.setCenter(10999, 600);

        if (player.upperbodySprite.getPosition().x > 14771) {
            view.setCenter((14771 + rightx) / 2, 110);
            view.setSize(rightx - 14771, 2087);
        }
        else view.setSize(1920, 1080);
    }
    else if (bgCounter == 2)
    {
        if (!ismoved2)
        {
            player.upperbodySprite.setPosition(18400, 800);
            player.lowerbodySprite.setPosition(18400, 800);
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
            player.upperbodySprite.setPosition(20400, 600);
            player.lowerbodySprite.setPosition(20400, 600);
            ismoved3 = true;
        }
        leftx = 20000;
        rightx = 24771;
        view.setSize(1920, 1190);
        if (player.upperbodySprite.getPosition().x <= 23800 && player.upperbodySprite.getPosition().x >= 20950)
            view.setCenter(player.upperbodySprite.getPosition().x, 600);
        else if (player.upperbodySprite.getPosition().x > 23800)
            view.setCenter(23801, 600);
        else if (player.upperbodySprite.getPosition().x < 20950)
            view.setCenter(20949, 600);
    }

}
void Playersetup()
{
    //sprite
    player.upperbodyTex.loadFromFile("Running (Pistol) Sprite Sheet Upper Body.png");
    player.upperbodySprite.setTexture(player.upperbodyTex);
    player.upperbodySprite.setTextureRect(IntRect(0, 0, 408 / 12, 41));
    player.upperbodySprite.setPosition(600, 600);
    player.upperbodySprite.setScale(3.25, 3.25);

    player.lowerbodyTex.loadFromFile("Running (Pistol) Sprite Sheet Lower Body.png");
    player.lowerbodySprite.setTexture(player.lowerbodyTex);
    player.lowerbodySprite.setTextureRect(IntRect(0, 0, 408 / 12, 41));
    player.lowerbodySprite.setPosition(600, 600);
    player.lowerbodySprite.setScale(3.25, 3.25);
    //rectangle
    player.rec.setPosition(player.upperbodySprite.getPosition().x - 50, player.upperbodySprite.getPosition().y);
    player.rec.setSize(Vector2f(75, 130));
    //HP
    player.playerHPTex.loadFromFile("Gradient_Health_Bar.png");
    player.playerHPSprite.setTexture(player.playerHPTex);
    player.playerHPSprite.setTextureRect(IntRect(0, 0, 204, 30));
}
void Pistolsetup()
{
    pistol.coll.setSize(Vector2f(15, 10.5));
    pistol.speed = 25;
    pistol.cooldown = 3000;
}
void cooldown(Player& player)
{
    if (player.cooldown > 0)
    {
        player.cooldown -= 0.0000000001;
    }
    if (player.cooldown <= 0)
    {
        player.canshoot = 1;
    }
}
void moviebullet(Player& player, Pistol& pistol)
{
    if (player.last_key == 1)
    {
        pistol.coll.move(Vector2f(-1 * pistol.speed, 0));
    }
    else
    {
        pistol.coll.move(Vector2f(pistol.speed, 0));
    }


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
    if (player.upperbodySprite.getPosition().x > rightx && bgCounter == 0)
    {
        transition();
        transition_reverse();
        bgCounter = 1;
        this_thread::sleep_for(chrono::milliseconds(300));

    }
    else if (player.upperbodySprite.getPosition().x > rightx && bgCounter == 1)
    {
        transition();
        transition_reverse();
        bgCounter = 2;
        this_thread::sleep_for(chrono::milliseconds(300));

    }
    else if (player.upperbodySprite.getPosition().x > rightx && bgCounter == 2)
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

void plmovement(Sprite& s, float maxframe, float x, float y, float delay, int index)
{
    //gravity
    s.setOrigin(x / 2, 0);
    player.upperbodySprite.setOrigin(x / 2, 0);
    if (!canMoveleft(s, leftx) || !canMoveRight(s, rightx)) {
        player.Velocity.x = 0;
        player.Velocity.y += gravity * 0.9;

    }
    if (collisonPl(wall, 4))
        player.Velocity.x = 0;

    if (!collisonPl(ground, 12))
    {
        if (Keyboard::isKeyPressed(Keyboard::A) && canDoubleJump)
        {
            //  cout << "A jump" << endl;
            jump();
            canDoubleJump = 0;
        }
        player.Velocity.y += gravity * 0.9;
        onlymove(s);//function -> movement in air
        onlymove(player.upperbodySprite);
    }
    else
    {

        move_with_animation(s, maxframe, x, y, delay, index);//functoin -> movement & animation
        move_with_animation(player.upperbodySprite, maxframe, x, y, delay, index);
    }

    s.move(player.Velocity);
    player.upperbodySprite.move(player.Velocity);
    player.rec.setPosition(s.getPosition().x - 50, s.getPosition().y);
}
void onlymove(Sprite& s)
{

    if (Keyboard::isKeyPressed(Keyboard::Right) && canMoveRight(s, rightx))
    {
        player.upperbodyTex.loadFromFile("Running (Pistol) Sprite Sheet Upper Body.png");
        player.lowerbodyTex.loadFromFile("Running (Pistol) Sprite Sheet Lower Body.png");
        player.Velocity.x = 0.17 * dt;
        s.setScale(3.25, 3.25);
        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            player.Velocity.x *= 2;
        }
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left) && canMoveleft(s, leftx))
    {
        player.upperbodyTex.loadFromFile("Running (Pistol) Sprite Sheet Upper Body.png");
        player.lowerbodyTex.loadFromFile("Running (Pistol) Sprite Sheet Lower Body.png");
        player.Velocity.x = -0.17 * dt;
        s.setScale(-3.25, 3.25);
        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            player.Velocity.x *= 2;
        }
    }
    else
    {
        player.upperbodyTex.loadFromFile("Idle (Pistol) Sprite Sheet Upper Body.png");
        animation(player.upperbodySprite, 3.9, 128 / 4, 37, idle, 3);

        player.lowerbodyTex.loadFromFile("Idle (Pistol) Sprite Sheet Lower Body.png");
        animation(player.lowerbodySprite, 3.9, 128 / 4, 37, idle, 3);

        player.Velocity.x = 0;
    }

}
void move_with_animation(Sprite& s, float maxframe, float x, float y, float delay, int index)
{
    player.Velocity.y = 0;
    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
        jump();

        canDoubleJump = 1;
    }
    else
        canDoubleJump = 0;
    if (Keyboard::isKeyPressed(Keyboard::Left) && canMoveleft(s, leftx))
    {
        player.upperbodyTex.loadFromFile("Running (Pistol) Sprite Sheet Upper Body.png");
        player.lowerbodyTex.loadFromFile("Running (Pistol) Sprite Sheet Lower Body.png");
        player.last_key = 1;
        s.setScale(-3.25, 3.25);
        player.Velocity.x = -0.17 * dt;
        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            player.Velocity.x *= 2;
            animation(s, maxframe, x, y, 0.007, index);
        }
        else
        {
            animation(s, maxframe, x, y, delay, index);
        }
    }
    else if (Keyboard::isKeyPressed(Keyboard::Right) && canMoveRight(s, rightx))
    {
        player.upperbodyTex.loadFromFile("Running (Pistol) Sprite Sheet Upper Body.png");
        player.lowerbodyTex.loadFromFile("Running (Pistol) Sprite Sheet Lower Body.png");
        player.last_key = 2;
        s.setScale(3.25, 3.25);
        player.Velocity.x = 0.17 * dt;
        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            player.Velocity.x *= 2;

            animation(s, maxframe, x, y, 0.007, index);
        }
        else
        {
            animation(s, maxframe, x, y, delay, index);
        }
    }

    else
    {
        player.upperbodyTex.loadFromFile("Idle (Pistol) Sprite Sheet Upper Body.png");
        animation(player.upperbodySprite, 3.9, 128 / 4, 37, idle, 3);

        player.lowerbodyTex.loadFromFile("Idle (Pistol) Sprite Sheet Lower Body.png");
        animation(player.lowerbodySprite, 3.9, 128 / 4, 37, idle, 3);

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
        if (player.rec.getGlobalBounds().intersects(arr[i].getGlobalBounds()))
            return true;
    }
    return false;

}
void animation(Sprite& s, float maxframe, float x, float y, float delay, int index)
{
    animiindecator[index] += delay * dt;
    if (animiindecator[index] > maxframe)
        animiindecator[index] = 0;
    s.setTextureRect(IntRect(int(animiindecator[index]) * x, 0, x, y));
}

void create(RectangleShape arr[], int index, int sizeX, int sizeY, int xPosition, int yPostions)
{
    arr[index].setSize(Vector2f(sizeX, sizeY));
    arr[index].setPosition(xPosition, yPostions);
}
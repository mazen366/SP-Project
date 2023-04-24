#include <SFML/Graphics.hpp>
#include<thread>
#include <iostream>
#define START 1
#define OPTIONS 2
#define EXIT 3
#define RIGHT 4
#define LEFT 5
#define LEVEL_1_A_BG 0
#define LEVEL_1_B_BG 1
#define LEVEL_1_C_BG 2
#define LEVEL_1_D_BG 3
using namespace std;
using namespace sf;

RenderWindow window(sf::VideoMode(1920, 1080), "Game");
Event event;

//make the variable=""
string pathh=resourcePath();
const float idle = 0.001,
shooting_delay = 0.007,
plVelocity = 0.2,
plScale = 3.25,
melee_delay = 0.003;

//player
struct Player
{
    //?
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
    bool crouch = 0;
    bool is_shooting = 0;
    bool one_sprite_needed = 0;
    int last_key = 0;
};
Player player;

// shooting
struct Pistol
{
    //code الصيني
    float speed;
    int direction = 0; // Right/Left
    RectangleShape coll; // hitbox
    Texture pistol_Ammo_Tex;
    Sprite pistol_Ammo_Sprite;
    float cooldown;
    void setup(Pistol pistol)
    {
        pistol.coll.setSize(Vector2f(15, 10.5));
        pistol.speed = 25;
        pistol.cooldown = 3000;
    }

    //new code
    int damage = 1;


} pistol;
float shoot_timer = 0;
vector<pair<RectangleShape, int>>rects;

//pause manu
struct Pause
{
    int selected = 1;
    bool is_paused = false;

    void exit()
    {
        is_paused = false;
    }
    void move_up()
    {
        selected = (selected == 1 ? 3 : selected - 1);
    }
    void move_down()
    {
        selected = (selected == 3 ? 1 : selected + 1);
    }
}pause_menu;

//menu
struct Menu
{
    int selected = 1;
    bool start_selected = false, options_selected = false, is_paused = false;

    void move_up()
    {
        selected = (selected == 1 ? 3 : selected - 1);
    }
    void move_down()
    {
        selected = (selected == 3 ? 1 : selected + 1);
    }

} menu;
//ground & wall
RectangleShape ground[30], wall[30];

//enemy map 1
//make clock & timer to plmovement
Clock clock_pl;
float dt = 0;
int ntimer[10] = { };
void EnemiAnimation(Sprite& s, float maxframe, float x, float y, float delay, float& indecator);
struct Enemy1
{
    Texture texture;
    Sprite sprite;
    RectangleShape rec;
    Vector2f velocity = { 0,0 };
    int damage = 1;
    int health = 10;
    float indicator[10];
    bool isEqu = 0;
    bool isdead = 0;
    //int distance= player.upperbodySprite.getPosition().x - enemy1[i].rec.getPosition().x
    bool live = 1;
    void setup(Enemy1 enemy1[10])
    {
        for (int i = 0; i < 10; i++)
        {
            enemy1[i].texture.loadFromFile(pathh+"RS Running Sprite Sheet.png");
            enemy1[i].sprite.setTexture(enemy1[i].texture);
            enemy1[i].sprite.setTextureRect(IntRect(0, 0, 312 / 12, 40));
        }
        for (int i = 0; i < 10; i++)
        {
            enemy1[i].rec.setSize(Vector2f(50, 100));
            enemy1[i].rec.setPosition(Vector2f(500 + 700 * i, 600));
            enemy1[i].sprite.setPosition(Vector2f(500 + 700 * i, 600));
            enemy1[i].sprite.setScale(plScale, plScale);
        }
    }
    void movement(Enemy1 enemy1[10])
    {
        for (int i = 0; i < 10; i++)
        {
            if (abs(player.upperbodySprite.getPosition().x - enemy1[i].sprite.getPosition().x) < 700)
            {
                if (abs(player.upperbodySprite.getPosition().x - enemy1[i].sprite.getPosition().x) <= 300)
                {
                    if (!enemy1[i].isEqu)
                    {
                        enemy1[i].velocity.x = 0;
                        enemy1[i].texture.loadFromFile(pathh+"RS Equipping Sprite Sheet.png");
                        enemy1[i].sprite.setOrigin(322 / 7 / 2, 0);
                        //   animation(enemy1[i].sprite, 6.9, 322 / 7, 44, 0.002, 9);
                        enemy1[i].indicator[1] += 0.2;
                        if (enemy1[i].indicator[1] > 6.9)
                        {
                            enemy1[i].indicator[1] = 0;
                            enemy1[i].isEqu = 1;
                        }
                        enemy1[i].sprite.setTextureRect(IntRect(int(enemy1[i].indicator[1]) * 322 / 7, 0, 322 / 7, 44));
                    }
                    else
                    {

                        enemy1[i].velocity.x = 0;
                        enemy1[i].texture.loadFromFile(pathh+"RS Shooting Sprite Sheet.png");
                        enemy1[i].sprite.setOrigin(183 / 3 / 3, 0);
                        EnemiAnimation(enemy1[i].sprite, 2.9, 183 / 3, 38, 0.008, enemy1[i].indicator[0]);
                        if (player.upperbodySprite.getPosition().x > enemy1[i].sprite.getPosition().x)
                            enemy1[i].sprite.setScale(-plScale, plScale);
                        else
                            enemy1[i].sprite.setScale(plScale, plScale);
                    }
                }
                else
                {
                    enemy1[i].isEqu = 0;
                    enemy1[i].texture.loadFromFile(pathh+"RS Running Sprite Sheet.png");
                    enemy1[i].sprite.setOrigin(312 / 12 / 3, 0);
                    EnemiAnimation(enemy1[i].sprite, 11.9, 312 / 12, 40, 0.017, enemy1[i].indicator[2]);
                    if (player.upperbodySprite.getPosition().x > enemy1[i].sprite.getPosition().x)
                    {
                        enemy1[i].velocity.x = 2;
                        enemy1[i].sprite.setScale(-plScale, plScale);
                    }
                    else
                    {
                        enemy1[i].velocity.x = -2;
                        enemy1[i].sprite.setScale(plScale, plScale);
                    }
                }
            }
            else
            {
                enemy1[i].velocity.x = 0;
            }
        }
        for (int i = 0; i < 10; i++)
        {
            enemy1[i].sprite.move(enemy1[i].velocity);
        }
    }
    void Gravity(Enemy1 enemy1[10])
    {
        for (int j = 0; j < 10; j++)
        {
            if (enemy1[j].sprite.getGlobalBounds().intersects(ground[0].getGlobalBounds()))
            {
                enemy1[j].velocity.y = 0;
            }
            else
            {
                enemy1[j].velocity.y += 0.7 * 0.9;
            }

        }
    }
    void Damage(Enemy1 enemy1[10])
    {
        for (int i = 0; i < 10; i++)
        {
            if (enemy1[i].live)
            {
                for (int j = 0; j < rects.size(); j++)
                {
                    if (enemy1[i].sprite.getGlobalBounds().intersects(rects[j].first.getGlobalBounds()) && rects[j].second != 0)
                    {
                        enemy1[i].health -= 1;
                        rects[j].second = 0;
                    }
                }
                if (enemy1[i].health <= 0)
                {
                    if (!enemy1[i].isdead)
                    {
                        enemy1[i].velocity.x = 0;
                        enemy1[i].texture.loadFromFile(pathh+"RS Dying Sprite Sheet.png");
                        enemy1[i].sprite.setOrigin(341 / 11 / 2, 0);
                        enemy1[i].indicator[1] += 0.1;
                        if (enemy1[i].indicator[1] > 10.9)
                        {
                            enemy1[i].indicator[1] = 0.2;
                            enemy1[i].isdead = 1;
                        }
                        enemy1[i].sprite.setTextureRect(IntRect(int(enemy1[i].indicator[1]) * 341 / 11, 0, 341 / 11, 39));


                    }
                    else
                    {
                        enemy1[i].live = 0;
                    }
                }

            }
        }
    }
}enemy1[10];


//gravity
float gravity = 0.7;
bool canDoubleJump;



//background Textures&Sprites
Texture bgTexture[30];
Sprite bgSprite[30];

int bgCounter = 0, leftEnd, rightEnd;//indicator for current map, start of map, end of map
bool  isMoved = false, ismoved2 = 0, ismoved3 = 0;//flags for clearing the previous map
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
void shooting();
void jumpingAnimation(float);
void meeleAnimation();
void ShootingAnimation();
void crouchingAnimation();
void IdleAnimation();
void Menu();
void BGanimation();
void plmovement(Sprite&, float, float, float, float, int);
void onlymove(Sprite&);
void jump();
void moveToRight(Sprite&);
void moveToLeft(Sprite&);
void move_with_animation(Sprite&, float, float, float, float, int);
void moviebullet(Player&, Pistol&);
void shootingCooldown(Player&);
void Playersetup();
void bgSetup();
void windowclose();
void windowfunction();
void cameraView();
void transition();
void transition_reverse();
void transition_pos_check();
bool canMoveRight(Sprite, int);
bool canMoveleft(Sprite, int);
bool collisonPl(RectangleShape[], int);
void create(RectangleShape[], int, int, int, int, int);//make ground
void animation(Sprite&, float, float, float, float, int);

int main()
{
    window.setFramerateLimit(60);
    enemy1->setup(enemy1);
    bgSetup();
    pistol.setup(pistol);
    Playersetup();
    Menu();
    return 0;
}
//DEFINITIONS

void Menu()
{
    while (window.isOpen())
    {
        if (pause_menu.is_paused)
        {
            if (Keyboard::isKeyPressed(Keyboard::Escape))
            {
                Clock timer;
                while (true)
                {
                    if (timer.getElapsedTime().asMilliseconds() > 300)
                    {
                        pause_menu.is_paused = false;
                        break;
                    }
                }
            }
            else if (pause_menu.selected == 1 && Keyboard::isKeyPressed(Keyboard::Enter))
                pause_menu.exit();
            else if (pause_menu.selected == 3 && Keyboard::isKeyPressed(Keyboard::Enter))
                window.close();
            else if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                Clock timer1;
                while (true)
                {
                    if (timer1.getElapsedTime().asMilliseconds() > 300)
                    {
                        pause_menu.move_up();
                        break;
                    }
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                Clock timer1;
                while (true)
                {
                    if (timer1.getElapsedTime().asMilliseconds() > 300)
                    {
                        pause_menu.move_down();
                        break;
                    }
                }
            }
        }
        else if ((menu.selected == START && Keyboard::isKeyPressed(Keyboard::Enter)) || menu.start_selected)
        {
            menu.start_selected = true;
            windowfunction();
        }

        else if (menu.selected == EXIT && Keyboard::isKeyPressed(Keyboard::Enter))
            window.close();

        else if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            Clock timer1;
            while (true)
            {
                if (timer1.getElapsedTime().asMilliseconds() > 300)
                {
                    menu.move_up();
                    break;
                }
            }

        }
        else if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            Clock timer1;
            while (true)
            {
                if (timer1.getElapsedTime().asMilliseconds() > 300)
                {
                    menu.move_down();
                    break;
                }
            }
        }
    }
}
void bgSetup()
{
    // LEVEL 1 A SET UP
    bgTexture[0].loadFromFile(pathh+"Level 1-A BG.png");
    bgSprite[0].setTexture(bgTexture[0]);
    bgSprite[0].setPosition(-370, -53);
    create(ground, 0, 8830, 30, -370, 800);
    //lvl 1 ->A FG
    lvl1FGtex[0].loadFromFile(pathh+"Level 1-A FG.png");
    Lvl1FG[0].setTexture(lvl1FGtex[0]);
    Lvl1FG[0].setPosition(-370, -53);

    // LEVEL 1 B->1 SET UP
    bgTexture[1].loadFromFile(pathh+"Level 1-B-1 BG.png");
    bgSprite[1].setTexture(bgTexture[1]);
    bgSprite[1].setPosition(10000, -50);
    create(ground, 1, 5939, 30, 10000, 907);
    create(ground, 2, 300, 18, 15930, 840);
    create(ground, 3, 150, 10, 16245, 625);
    create(ground, 4, 160, 10, 16380, 390);
    create(ground, 5, 325, 18, 16555, 173);
    create(ground, 6, 700, 10, 16850, 40);

    // LEVEL 1 B->2 SET UP
    bgTexture[2].loadFromFile(pathh+"Level 1-B-2 BG.png");
    bgSprite[2].setTexture(bgTexture[2]);
    bgSprite[2].setPosition(14771, -940);

    //animation lv1-B-2(Exit lamp)
    ExitlampTex.loadFromFile(pathh+"Level 1-B-2 Exit Lamp.png");
    Exitlamp.setTexture(ExitlampTex);
    Exitlamp.setTextureRect(IntRect(0, 0, 17328 / 6, 2087));
    Exitlamp.setPosition(14771, -940);

    //FG Lv1-B-2
    lvl1FGtex[2].loadFromFile(pathh+"Level 1-B-2 FG.png");
    Lvl1FG[2].setTexture(lvl1FGtex[2]);
    Lvl1FG[2].setPosition(14771, -940);

    create(wall, 0, 20, 80, 15938, 870);
    create(wall, 1, 20, 220, 16235, 640);
    create(wall, 2, 20, 220, 16370, 409);
    create(wall, 3, 10, 200, 16520, 198);
    create(wall, 4, 10, 140, 16840, 60);

    //LEVEL 1 C SET UP
    bgTexture[3].loadFromFile(pathh+"Level 1-C BG.png");
    bgSprite[3].setTexture(bgTexture[3]);
    bgSprite[3].setPosition(18000, 0);

    //fire troches lv1 ->c
    FireTrochestex.loadFromFile(pathh+"Level 1-C Fire Torches.png");
    FireTroches.setTexture(FireTrochestex);
    FireTroches.setTextureRect(IntRect(0, 0, 13120 / 8, 1192));
    FireTroches.setPosition(18000, 0);

    //lvl 1 ->c FG
    lvl1FGtex[3].loadFromFile(pathh+"Level 1-C FG.png");
    Lvl1FG[3].setTexture(lvl1FGtex[3]);
    Lvl1FG[3].setPosition(18000, 0);
    create(ground, 7, 1640, 20, 18000, 970);

    //LEVEL 1 D SET UP
    bgTexture[4].loadFromFile(pathh+"Level 1-D BG.png");
    bgSprite[4].setTexture(bgTexture[4]);
    bgSprite[4].setPosition(20000, 0);
    create(ground, 8, 1050, 20, 20000, 900);
    create(ground, 9, 1200, 20, 21700, 900);
    create(ground, 10, 750, 20, 23120, 900);
    create(ground, 11, 800, 20, 24150, 900);
    //first platform
    create(ground, 12, 500, 20, 20300, 700);

}
void windowfunction()
{

    //delta time
    dt = clock_pl.getElapsedTime().asMicroseconds();
    dt /= 750;
    clock_pl.restart();

    //shooting
    moviebullet(player, pistol);
    shootingCooldown(player);

    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
        Clock timer;
        while (true)
        {
            if (timer.getElapsedTime().asMilliseconds() > 300)
            {
                pause_menu.is_paused = true;
                break;
            }
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::R))
    {
        for (int i = 0; i < 5; i++)
        {
            player.arr[i] = 1;
        }
    }


    //map shortcut
    if (Keyboard::isKeyPressed(sf::Keyboard::T))
        player.upperbodySprite.setPosition(20000, 800);

    cameraView();
    enemy1->movement(enemy1);
    enemy1->Gravity(enemy1);
    enemy1->Damage(enemy1);

    plmovement(player.lowerbodySprite, 11.9, 408 / 12, 41, 0.004, 2);
    player.playerHPSprite.setPosition(view.getCenter().x - 960, view.getCenter().y - 500);
    BGanimation();
    windowclose();
    window.clear();
    for (int i = 0; i < 5; i++)
        window.draw(bgSprite[i]);
    //   window.draw(Exitlamp);
    window.draw(FireTroches);
    //window.draw(player.rec);
    for (int i = 0; i < 10; i++) {
        if (enemy1[i].live == 1) {
            //window.draw(enemy1[i].rec);
            window.draw(enemy1[i].sprite);
        }
    }
    window.draw(ground[12]);

    window.draw(player.lowerbodySprite);
    if (!player.one_sprite_needed)
        window.draw(player.upperbodySprite);


    for (int i = 0; i < 4; i++)
    {
        if (i == 1)continue;
        window.draw(Lvl1FG[i]);
    }
    for (int x = 0; x < rects.size(); x++)
    {
        // Only move the rect here, don't set the origin.

        if (rects[x].second == LEFT)
        {
            rects[x].first.move(-10, 0);
            window.draw(rects[x].first);
        }
        else if (rects[x].second == RIGHT)
        {
            window.draw(rects[x].first);
            rects[x].first.move(10, 0);
        }
    }
    window.draw(player.playerHPSprite);
    window.draw(pistol.coll);
    window.setView(view);
    transition_pos_check();
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
    }
}
void cameraView()
{
    //first map
    if (bgCounter == LEVEL_1_A_BG) {

        leftEnd = -300;
        rightEnd = 8643;
        //area where no black edges can appear
        if (player.upperbodySprite.getPosition().x <= 7740 && player.upperbodySprite.getPosition().x >= 600)
            view.setCenter(player.upperbodySprite.getPosition().x, 600);  //camera focus on player
        //area where  black edge appear from right
        else if (player.upperbodySprite.getPosition().x > 7740)  //camera stop
            view.setCenter(7741, 600);
        //area where  black edge appear from left
        else if (player.upperbodySprite.getPosition().x < 600)
            view.setCenter(599, 600);  //camera stop

    }
    else if (bgCounter == LEVEL_1_B_BG)
    {
        //second map

        if (!isMoved)
        {   //intial position for the map
            player.upperbodySprite.setPosition(10400, 600);
            player.lowerbodySprite.setPosition(10400, 600);
            isMoved = true;
        }
        leftEnd = 10100;
        rightEnd = 17651;

        //area where no black edges can appear
        if (player.upperbodySprite.getPosition().x <= 14771 && player.upperbodySprite.getPosition().x >= 11000)
            view.setCenter(player.upperbodySprite.getPosition().x, 600);  //camera focus on player
        //area where  black edge appear from left
        else if (player.upperbodySprite.getPosition().x < 11000)
            view.setCenter(10999, 600);  //camera stop

        if (player.upperbodySprite.getPosition().x > 14771)//enter area of staires
        {
            view.setCenter((14771 + rightEnd) / 2, 110);//camera stop
            view.setSize(rightEnd - 14771, 2087);//zoomed out view for stairs
        }
        else view.setSize(1920, 1080);//normal size view
    }
    else if (bgCounter == LEVEL_1_C_BG)
    {
        // third map
        if (!ismoved2)
        {//start postion of the map
            player.upperbodySprite.setPosition(18400, 800);
            player.lowerbodySprite.setPosition(18400, 800);
            ismoved2 = true;
        }
        leftEnd = 18030;
        rightEnd = 19650;
        view.setCenter(18820, 596);//no player tracing
        view.setSize(1600, 1080);//whole map size
    }
    else if (bgCounter == LEVEL_1_D_BG)
    {
        //forth map
        if (!ismoved3)
        {   //start postion of nao
            player.upperbodySprite.setPosition(20400, 600);
            player.lowerbodySprite.setPosition(20400, 600);
            ismoved3 = true;
        }
        leftEnd = 20000;
        rightEnd = 24771;
        view.setSize(1920, 1190);//whole map hight

        //area where no black edges can appear
        if (player.upperbodySprite.getPosition().x <= 23800 && player.upperbodySprite.getPosition().x >= 20950)
            view.setCenter(player.upperbodySprite.getPosition().x, 600);
        //area where  black edge appear from right
        else if (player.upperbodySprite.getPosition().x > 23800)
            view.setCenter(23801, 600);
        //area where  black edge appear from left
        else if (player.upperbodySprite.getPosition().x < 20950)
            view.setCenter(20949, 600);
    }

}
void Playersetup()
{
    //sprite upperbody
    player.upperbodyTex.loadFromFile(pathh+"Running (Pistol) Sprite Sheet Upper Body.png");
    player.upperbodySprite.setTexture(player.upperbodyTex);
    player.upperbodySprite.setTextureRect(IntRect(0, 0, 408 / 12, 41));
    player.upperbodySprite.setPosition(600, 600);
    player.upperbodySprite.setScale(plScale, plScale);
    //sprite lowerbody
    player.lowerbodyTex.loadFromFile(pathh+"Running (Pistol) Sprite Sheet Lower Body.png");
    player.lowerbodySprite.setTexture(player.lowerbodyTex);
    player.lowerbodySprite.setTextureRect(IntRect(0, 0, 408 / 12, 41));
    player.lowerbodySprite.setPosition(600, 600);
    player.lowerbodySprite.setScale(plScale, plScale);
    //rectangle
    player.rec.setPosition(player.upperbodySprite.getPosition().x - 50, player.upperbodySprite.getPosition().y);
    player.rec.setSize(Vector2f(75, 130));
    //HP
    player.playerHPTex.loadFromFile(pathh+"Gradient_Health_Bar.png");
    player.playerHPSprite.setTexture(player.playerHPTex);
    player.playerHPSprite.setTextureRect(IntRect(0, 0, 204, 30));
}
void shootingCooldown(Player& player)
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
    if (player.last_key == RIGHT || pistol.direction == RIGHT)
    {
        pistol.direction = RIGHT;
        pistol.coll.move(Vector2f(pistol.speed, 0));
    }
    else
    {
        pistol.coll.move(Vector2f(-1 * pistol.speed, 0));
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
    if (player.upperbodySprite.getPosition().x > rightEnd && bgCounter == 0)
    {
        transition();
        transition_reverse();
        bgCounter = 1;
        this_thread::sleep_for(chrono::milliseconds(300));

    }
    else if (player.upperbodySprite.getPosition().x > rightEnd && bgCounter == 1)
    {
        transition();
        transition_reverse();
        bgCounter = 2;
        this_thread::sleep_for(chrono::milliseconds(300));

    }
    else if (player.upperbodySprite.getPosition().x > rightEnd && bgCounter == 2)
    {
        transition();
        transition_reverse();
        bgCounter = 3;
        this_thread::sleep_for(chrono::milliseconds(300));

    }
    else
        window.display();
}
bool canMoveRight(Sprite object, int rightLimit)
{
    if (object.getPosition().x < rightLimit && !collisonPl(wall, 4))
        return true;

    return false;

}
bool canMoveleft(Sprite object, int leftLimit)
{
    if (object.getPosition().x > leftLimit)
        return true;

    return false;

}

void plmovement(Sprite& s, float maxframe, float x, float y, float delay, int index)
{
    //s==lower body
    s.setOrigin(x / 2, 0);
    player.upperbodySprite.setOrigin(x / 2, 0);

    //check collision
    if (!canMoveleft(s, leftEnd) || !canMoveRight(s, rightEnd))
    {
        player.Velocity.x = 0;
        //gravity
        player.Velocity.y += gravity * 0.9;
    }

    if (collisonPl(wall, 4))
    {
        player.Velocity.x = 0;
    }

    if (!collisonPl(ground, 20))
    {
        //gravity
        player.Velocity.y += gravity * 0.9;

        //function -> movement in air
        onlymove(s);
        onlymove(player.upperbodySprite);

        jumpingAnimation(delay);
        shoot_timer = 0;
    }
    else
    {
        //functoin -> movement & animation
        move_with_animation(s, maxframe, x, y, delay, index);
        move_with_animation(player.upperbodySprite, maxframe, x, y, delay, index);
    }

    //move player
    s.move(player.Velocity);
    player.upperbodySprite.move(player.Velocity);
    player.rec.setPosition(s.getPosition().x - 50, s.getPosition().y);
}

void onlymove(Sprite& s)
{
    //double jump
    if (Keyboard::isKeyPressed(Keyboard::A) && canDoubleJump)
    {
        jump();
        canDoubleJump = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::Right) && canMoveRight(s, rightEnd))
        moveToRight(s);
    else if (Keyboard::isKeyPressed(Keyboard::Left) && canMoveleft(s, leftEnd))
        moveToLeft(s);
    else
        player.Velocity.x = 0;
}
void move_with_animation(Sprite& s, float maxframe, float x, float y, float delay, int index)
{
    player.Velocity.y = 0;
    // player crouches
    if (Keyboard::isKeyPressed(Keyboard::C))
    {
        player.Velocity.x = 0;
        crouchingAnimation();
        //shoot_timer = 0;
    }
    else
    {
        player.one_sprite_needed = 0;
        player.crouch = 0;
        player.lowerbodySprite.setPosition(player.upperbodySprite.getPosition());
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            jump();
            canDoubleJump = 1;
        }
        else
            canDoubleJump = 0;
        if (Keyboard::isKeyPressed(Keyboard::Left) && canMoveleft(s, leftEnd))
        {
            player.last_key = LEFT;
            // if player shoots while running
            if (Keyboard::isKeyPressed(Keyboard::J))
            {
                shooting();
                ShootingAnimation();//when pl (move&&shoot)
            }
            //if player move load running sprite sheet
            else
            {
                shoot_timer = 0;
                player.upperbodyTex.loadFromFile(pathh+"Running (Pistol) Sprite Sheet Upper Body.png");
                player.lowerbodyTex.loadFromFile(pathh+"Running (Pistol) Sprite Sheet Lower Body.png");
                animation(s, maxframe, x, y, delay, index);
            }
            player.last_key = 1;

            moveToLeft(s);

        }
        else if (Keyboard::isKeyPressed(Keyboard::Right) && canMoveRight(s, rightEnd))
        {
            player.last_key = RIGHT;
            // if player shoots while running
            if (Keyboard::isKeyPressed(Keyboard::J))
            {
                shooting();
                ShootingAnimation();//when pl (move&&shoot)
            }
            //if player move load running sprite sheet
            else
            {
                shoot_timer = 0;
                player.upperbodyTex.loadFromFile(pathh+"Running (Pistol) Sprite Sheet Upper Body.png");
                player.lowerbodyTex.loadFromFile(pathh+"Running (Pistol) Sprite Sheet Lower Body.png");
                animation(s, maxframe, x, y, delay, index);
            }
            player.last_key = 2;

            moveToRight(s);

        }

        else
        {
            if (Keyboard::isKeyPressed(Keyboard::J))
            {

                shooting();
                // shooting animation when pl standing
                player.lowerbodyTex.loadFromFile(pathh+"Idle (Pistol) Sprite Sheet Lower Body.png");
                animation(player.lowerbodySprite, 3.9, 128 / 4, 37, 0.04, 3);
                player.upperbodyTex.loadFromFile(pathh+"Shooting - Standing (Pistol) Sprite Sheet Upper Body.png");
                animation(player.upperbodySprite, 9.9, 520 / 10, 41, shooting_delay, 10);
            }
            else  if (Keyboard::isKeyPressed(Keyboard::K))
            {
                meeleAnimation();
                shoot_timer = 0;
            }
            //if player not move then load idle sprite sheet
            else
            {
                IdleAnimation();
                shoot_timer = 0;
            }

            player.Velocity.x = 0;


        }

    }


}
void moveToRight(Sprite& s)
{
    s.setScale(plScale, plScale);
    player.Velocity.x = plVelocity * dt;
    if (Keyboard::isKeyPressed(Keyboard::LShift))
    {
        player.Velocity.x *= 1.5;
    }
    player.last_key = RIGHT;
}
void moveToLeft(Sprite& s)
{
    s.setScale(-plScale, plScale);
    player.Velocity.x = -plVelocity * dt;
    if (Keyboard::isKeyPressed(Keyboard::LShift))
    {
        player.Velocity.x *= 1.5;
    }
    player.last_key = LEFT;
}
void jump()
{
    player.Velocity.y = -15;
    player.Velocity.x /= 2;
}
void IdleAnimation()
{
    player.lowerbodyTex.loadFromFile(pathh+"Idle (Pistol) Sprite Sheet Lower Body.png");
    animation(player.lowerbodySprite, 3.9, 128 / 4, 37, idle, 3);
    player.is_shooting = 0;
    player.upperbodyTex.loadFromFile(pathh+"Idle (Pistol) Sprite Sheet Upper Body.png");
    animation(player.upperbodySprite, 3.9, 128 / 4, 37, idle, 3);
}
void ShootingAnimation()
{
    player.lowerbodyTex.loadFromFile(pathh+"Running (Pistol) Sprite Sheet Lower Body.png");
    animation(player.lowerbodySprite, 11.9, 408 / 12, 41, 0.004, 2);
    player.upperbodyTex.loadFromFile(pathh+"Shooting - Standing (Pistol) Sprite Sheet Upper Body.png");
    animation(player.upperbodySprite, 9.9, 520 / 10, 41, shooting_delay, 10);
}
void crouchingAnimation()
{
    player.crouch = 1;
    player.one_sprite_needed = 1;
    if (Keyboard::isKeyPressed(Keyboard::J))
    {
        shooting();
        player.lowerbodyTex.loadFromFile(pathh+"Shooting - Crouching (Pistol) Sprite Sheet.png");
        player.lowerbodySprite.setPosition(player.upperbodySprite.getPosition().x, player.upperbodySprite.getPosition().y + player.upperbodyTex.getSize().y - 10);
        animation(player.lowerbodySprite, 9.9, 520 / 10, 29, shooting_delay, 14);
    }
    else if (Keyboard::isKeyPressed(Keyboard::K))
    {
        player.lowerbodyTex.loadFromFile(pathh+"Melee - Crouching (Pistol) Sprite Sheet.png");
        player.lowerbodySprite.setPosition(player.upperbodySprite.getPosition().x, player.upperbodySprite.getPosition().y + player.upperbodyTex.getSize().y - 30);
        animation(player.lowerbodySprite, 6.9, 294 / 7, 35, shooting_delay, 15);
    }
    else
    {
        player.lowerbodyTex.loadFromFile(pathh+"Idle - Crouching (Pistol) Sprite Sheet.png");
        player.lowerbodySprite.setPosition(player.upperbodySprite.getPosition().x, player.upperbodySprite.getPosition().y + player.upperbodyTex.getSize().y);
        animation(player.lowerbodySprite, 3.9, 136 / 4, 24, idle, 4);
    }

}
void meeleAnimation()
{
    // melee animation when pl standing
    player.lowerbodyTex.loadFromFile(pathh+"Melee (Pistol) Sprite Sheet Lower Body.png");
    animation(player.lowerbodySprite, 5.9, 288 / 6, 53, melee_delay, 12);
    player.upperbodyTex.loadFromFile(pathh+"Melee (Pistol) Sprite Sheet Upper Body.png");
    animation(player.upperbodySprite, 5.9, 288 / 6, 53, melee_delay, 12);

}
void jumpingAnimation(float delay)
{
    player.upperbodyTex.loadFromFile(pathh+"Jumping (Pistol) Sprite Sheet Upper Body.png");
    player.lowerbodyTex.loadFromFile(pathh+"Jumping (Pistol) Sprite Sheet Lower Body.png");
    animation(player.upperbodySprite, 10.9, 319.0 / 11, 49, delay, 5);
    animation(player.lowerbodySprite, 10.9, 319.0 / 11, 49, delay, 6);
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
void EnemiAnimation(Sprite& s, float maxframe, float x, float y, float delay, float& indecator)
{
    indecator += delay * dt;
    if (indecator > maxframe)
        indecator = 0;
    s.setTextureRect(IntRect(int(indecator) * x, 0, x, y));
}
void shooting()
{
    shoot_timer += 0.03;
    if (shoot_timer > 1 || shoot_timer == 0.03) {
        Vector2f pl = player.lowerbodySprite.getPosition();
        RectangleShape rect(sf::Vector2f(40, 10));
        rect.setOrigin(-pl.x, -(pl.y + 50));
        rects.push_back({ rect ,player.last_key });
        shoot_timer = 0;
    }
}
void create(RectangleShape arr[], int index, int width, int hight, int xPosition, int yPostions)
{
    arr[index].setSize(Vector2f(width, hight));
    arr[index].setPosition(xPosition, yPostions);
}
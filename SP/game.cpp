#include <SFML/Graphics.hpp>
#include<thread>
#include <SFML/Audio.hpp>
#include <iostream>
#define START 1
#define OPTIONS 2
#define CREDITS 3
#define EXIT 4
#define RIGHT 4
#define LEFT 5
#define LEVEL_1_A_BG 0
#define LEVEL_1_B_BG 1
#define LEVEL_1_C_BG 2
#define LEVEL_1_D_BG 3
#define PISTOL 1
#define RIFLE 2
#define ENEMY_PLAYER_RANGE 700
#define ENEMY_PLAYER_SHOOTING_RANGE 400
using namespace std;
using namespace sf;

RenderWindow window(sf::VideoMode(1920, 1080), "Game", sf::Style::Default);
Event event;

//make the variable=""
string pathh = "";
const float idle = 0.001,
pistolshooting_delay = 0.007, rifleshooting_delay = 0.005,
plVelocity = 0.2,
plScale = 3.25,
melee_delay = 0.003;
Texture TS_BGTex, TS_TandGTex, TS_LTex, TS_VTex, TS_LogoTex, TS_PETex, TS_buttonsTex;
Sprite TS_BGSpr, TS_TandGSpr, TS_LSpr, TS_VSpr, TS_LogoSpr, TS_PESpr, TS_buttonsSpr;
Music TS_BGTheme;
Music TS_BGFireFX;
SoundBuffer MenuClickB, MenuScrollB;
Sound MenuClick, MenuScroll;
Clock timer;

float TS_TandGCnt = 0;
float TS_LCnt = 0;
float TS_LogoCnt = 0;
float TS_PECnt = 0;
float AlphaPE = 255;
int TS_ButtonsCnt = 0;

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
    int gun = PISTOL;
    bool canshoot = 0;
    bool crouch = 0;
    bool is_shooting = 0;
    bool one_sprite_needed = 0;
    int last_key = 0;
    bool live = 1;
    bool isdead = 0;
    
};
Player player;

// shooting <pistol>
struct Pistol
{
   float  damage = 0.8;
   float shoot_timer = 0;
   vector<pair<RectangleShape, int>>rects;

   void shooting()
   {
       pistol.shoot_timer += 0.03;
       if(pistol.shoot_timer > 1 || pistol.shoot_timer == 0.03) {
           Vector2f pl = player.lowerbodySprite.getPosition();
           RectangleShape rect(sf::Vector2f(10, 10));
           rect.setOrigin(-pl.x, -(pl.y + 50));
           pistol.rects.push_back({ rect ,player.last_key });
           pistol.shoot_timer = 0;
       }
   }
} pistol;

// shooting <Rifle>
struct Rifle
{
    float  damage = 0.5;
    float shoot_timer = 0;
    vector<pair<RectangleShape, int>>rects;

    void shooting()
    {
        rifle.shoot_timer += 0.08;
        if (rifle.shoot_timer > 1 || rifle.shoot_timer == 0.03) {
            Vector2f pl = player.lowerbodySprite.getPosition();
            RectangleShape rect(sf::Vector2f(10, 10));
            rect.setOrigin(-pl.x, -(pl.y + 50));
            rifle.rects.push_back({ rect ,player.last_key });
            rifle.shoot_timer = 0;
        }
      
    }
} rifle;


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
    bool start_selected = false, options_selected = false, is_paused = false, ts_escaped = false;

    void move_up()
    {
        selected = (selected == 1 ? 4 : selected - 1);
        TS_ButtonsCnt = selected - 1;
        MenuScroll.play();
    }
    void move_down()
    {
        selected = (selected == 4 ? 1 : selected + 1);
        TS_ButtonsCnt = selected - 1;
        MenuScroll.play();
    }
    void draw()
    {
        window.clear();
        TS_TandGCnt += 0.139;
        if (TS_TandGCnt > 8)
            TS_TandGCnt -= 8;
        TS_TandGSpr.setTextureRect(IntRect((int)TS_TandGCnt * 600, 0, 600, 600));

        TS_LCnt += 0.139;
        if (TS_LCnt > 10)
            TS_LCnt -= 10;
        TS_LSpr.setTextureRect(IntRect((int)TS_LCnt * 473, 0, 473, 261));

        TS_LogoCnt += 0.139;
        if (TS_LogoCnt > 10)
            TS_LogoCnt -= 10;
        TS_LogoSpr.setTextureRect(IntRect((int)TS_LogoCnt * 1382, 0, 1382, 170));

        TS_PECnt += 0.139;
        if (TS_PECnt > 10)
            TS_PECnt -= 10;
        TS_PESpr.setTextureRect(IntRect((int)TS_PECnt * 695, 0, 695, 81));
        window.draw(TS_BGSpr);
        window.draw(TS_TandGSpr);
        window.draw(TS_LSpr);
        window.draw(TS_VSpr);
        window.draw(TS_LogoSpr);
        if (ts_escaped)
        {
            TS_buttonsSpr.setTextureRect(IntRect(TS_ButtonsCnt * 360, 0, 360, 315));
            window.draw(TS_buttonsSpr);
        }
        else
            window.draw(TS_PESpr);
        window.display();

    }
} menu;
//ground & wall
RectangleShape ground[30], wall[30];


//make clock & timer to plmovement
Clock clock_pl;
float dt = 0;
int ntimer[10] = { };

//enemy map 1
void EnemiAnimation(Sprite& s, float maxframe, float x, float y, float delay, float& indecator);
struct Enemy1
{
    Texture texture;
    Sprite sprite;
    RectangleShape rec;
    Vector2f velocity = { 0,0 };
    int damage = 1;
    int health = 10;
    float sprite_indicator[10];
    bool isCarrying_a_weapon = 0;
    bool death_animation_done = 0;
    bool check = 1; //to fix RS Equipping Sprite dimensions 
    int last_key = RIGHT;
    float shoot_timer = 0;
    bool stopped = 0;
    float initial_position;
    bool reversed_direction = 0;
    
    vector <pair <RectangleShape, int>> bullet;//enemy1 pistol
    bool is_alive = 1;
    void setup(Enemy1 enemy1[10])
    {
        for (int i = 0; i < 10; i++)
        {
            enemy1[i].texture.loadFromFile(pathh + "RS Running Sprite Sheet.png");
            enemy1[i].sprite.setTexture(enemy1[i].texture);
            enemy1[i].sprite.setTextureRect(IntRect(0, 0, 312 / 12, 40));
        }
        for (int i = 0; i < 10; i++)
        {
            enemy1[i].rec.setSize(Vector2f(100, 130));
            enemy1[i].rec.setPosition(Vector2f(500 + 700 * i+200, 600));
            enemy1[i].sprite.setPosition(Vector2f(500 + 700 * i+ 200, 600));
            enemy1[i].initial_position = 700 + 700 * i;
            enemy1[i].sprite.setScale(plScale, plScale);
        }
    }
    void movement(Enemy1 enemy1[10])
    {
        for (int i = 0; i < 10; i++)
        {
           // check if enemy is not dead
            if (!enemy1[i].stopped)
            {
                // Player in range of enemy 
                if (abs(player.upperbodySprite.getPosition().x - enemy1[i].rec.getPosition().x) < ENEMY_PLAYER_RANGE)
                {
                    // Player in range of enemy shooting 
                    if (abs(player.upperbodySprite.getPosition().x - enemy1[i].rec.getPosition().x) + 10 * i <= ENEMY_PLAYER_SHOOTING_RANGE && player.health > 0)
                    {
                        if (!enemy1[i].isCarrying_a_weapon)
                        {
                            Carrying_a_weapon(i);
                        }
                        else
                        {
                            EnemyShootingAnimation(i);
                        }
                    }
                    else
                    {
                        EnemyRunningToPl(i);
                    }
                }
                else
                {
                    Aimless_Walking(i); // Player not in range 
                }
            }
        }
        for (int i = 0; i < 10; i++)
        {

            enemy1[i].rec.move(enemy1[i].velocity);
            if (enemy1[i].check)//to fix RS Equipping Sprite dimensions 
            {
                enemy1[i].sprite.setPosition(enemy1[i].rec.getPosition());
            }
            else
            {//to fix RS Equipping Sprite dimensions 
                if (player.upperbodySprite.getPosition().x > enemy1[i].sprite.getPosition().x)
                    enemy1[i].sprite.setPosition(enemy1[i].rec.getPosition().x - 60, enemy1[i].rec.getPosition().y - 22);
                else
                    enemy1[i].sprite.setPosition(enemy1[i].rec.getPosition().x - 58, enemy1[i].rec.getPosition().y - 22);
            }
        }
    }
    void Gravity(Enemy1 enemy1[10])
    {
        for (int j = 0; j < 10; j++)
        {
            if (enemy1[j].rec.getGlobalBounds().intersects(ground[0].getGlobalBounds()))
            {
                enemy1[j].velocity.y = 0;
            }
            else
            {
                enemy1[j].velocity.y += 0.7 * 0.9;
            }

        }
    }
    void Damaged(Enemy1 enemy1[10])
    {
        for (int i = 0; i < 10; i++)
        {
            if (enemy1[i].is_alive)
            {
                if (player.gun == PISTOL)
                {
                    for (int j = 0; j < pistol.rects.size(); j++)
                    {
                        if (enemy1[i].sprite.getGlobalBounds().intersects(pistol.rects[j].first.getGlobalBounds()) && pistol.rects[j].second != 0)
                        {
                            enemy1[i].velocity.x = 0;
                            enemy1[i].health -= pistol.damage;
                            pistol.rects[j].second = 0;
                        }
                    }
                }else if (player.gun == RIFLE)
                {
                    for (int j = 0; j < rifle.rects.size(); j++)
                    {
                        if (enemy1[i].sprite.getGlobalBounds().intersects(rifle.rects[j].first.getGlobalBounds()) && rifle.rects[j].second != 0)
                        {
                            enemy1[i].velocity.x = 0;
                            enemy1[i].health -= rifle.damage;
                            rifle.rects[j].second = 0;
                        }
                    }
                }
                if (enemy1[i].health <= 0)
                {
                    enemy1[i].death_animation(i);
                }
            }
        }
    }
    void shooting(int i, Enemy1 enemy1[10])
    {
        enemy1[i].shoot_timer += 0.05;
        if (enemy1[i].shoot_timer > 1 || enemy1[i].shoot_timer == 0.05) {
            Vector2f pl = enemy1[i].sprite.getPosition();
            RectangleShape rect(sf::Vector2f(20, 10));
            rect.setOrigin(-pl.x, -(pl.y + 50));
            enemy1[i].bullet.push_back({ rect ,enemy1[i].last_key });
            enemy1[i].shoot_timer = 0;
        }
    }
    void Carrying_a_weapon(int i)//i==index of enemy
    {
        enemy1[i].check = 0;
        enemy1[i].velocity.x = 0;
        enemy1[i].texture.loadFromFile(pathh + "RS Equipping Sprite Sheet.png");
        enemy1[i].sprite.setOrigin(322 / 7 / 2, 0);
        //   animation(enemy1[i].sprite, 6.9, 322 / 7, 44, 0.002, 9);
        enemy1[i].sprite_indicator[1] += 0.2;
        if (enemy1[i].sprite_indicator[1] > 6.9)
        {
            enemy1[i].sprite_indicator[1] = 0;
            enemy1[i].isCarrying_a_weapon = 1;
        }
        enemy1[i].sprite.setTextureRect(IntRect(int(enemy1[i].sprite_indicator[1]) * 322 / 7, 0, 322 / 7, 44));
    }
    void EnemyShootingAnimation(int i)
    {
        enemy1[i].shooting(i, enemy1);
        enemy1[i].check = 1;
        enemy1[i].velocity.x = 0;
        enemy1[i].texture.loadFromFile(pathh + "RS Shooting Sprite Sheet.png");
        EnemiAnimation(enemy1[i].sprite, 2.9, 183 / 3, 38, 0.008, enemy1[i].sprite_indicator[0]);
        if (player.upperbodySprite.getPosition().x > enemy1[i].sprite.getPosition().x)
        {
            enemy1[i].sprite.setScale(-plScale, plScale);
            enemy1[i].sprite.setOrigin(0, 0);
            enemy1[i].last_key = RIGHT;
        }
        else
        {
            enemy1[i].sprite.setScale(plScale, plScale);
            enemy1[i].sprite.setOrigin(enemy1[i].sprite.getLocalBounds().width, 0);
            enemy1[i].last_key = LEFT;
        }
    }
    void EnemyRunningToPl(int i)
    {
        enemy1[i].shoot_timer = 1;
        enemy1[i].isCarrying_a_weapon = 0;
        enemy1[i].texture.loadFromFile(pathh + "RS Running Sprite Sheet.png");
        EnemiAnimation(enemy1[i].sprite, 11.9, 312 / 12, 40, 0.017, enemy1[i].sprite_indicator[2]);
        if (player.upperbodySprite.getPosition().x > enemy1[i].sprite.getPosition().x && player.health > 0)
        {
            enemy1[i].velocity.x = 2;
            enemy1[i].sprite.setScale(-plScale, plScale);
            enemy1[i].sprite.setOrigin(0, 0);
            enemy1[i].last_key = RIGHT;
        }
        else if (player.upperbodySprite.getPosition().x < enemy1[i].sprite.getPosition().x && player.health > 0)
        {
            enemy1[i].velocity.x = -2;
            enemy1[i].sprite.setScale(plScale, plScale);
            enemy1[i].sprite.setOrigin(enemy1[i].sprite.getLocalBounds().width, 0);
            enemy1[i].last_key = LEFT;
        }
        else
        {
            enemy1[i].velocity.x = 2;
            enemy1[i].sprite.setScale(-plScale, plScale);
            enemy1[i].sprite.setOrigin(0, 0);
            enemy1[i].last_key = RIGHT;
        }

    }
    void Aimless_Walking(int i)
    {
        enemy1[i].isCarrying_a_weapon = 0;
        if (!enemy1[i].reversed_direction)
        {
            enemy1[i].texture.loadFromFile(pathh + "RS Running Sprite Sheet.png");
            EnemiAnimation(enemy1[i].sprite, 11.9, 312.0 / 12.0, 40, 0.017, enemy1[i].sprite_indicator[5]);
            enemy1[i].velocity.x = 2;
            enemy1[i].sprite.setScale(-plScale, plScale);
            enemy1[i].sprite.setOrigin(0, 0);
            enemy1[i].last_key = RIGHT;
            if (enemy1[i].rec.getPosition().x > enemy1[i].initial_position + 150)
            {
                enemy1[i].reversed_direction = 1;
            }
        }
        else
        {         
            enemy1[i].texture.loadFromFile(pathh + "RS Running Sprite Sheet.png");
            EnemiAnimation(enemy1[i].sprite, 11.9, 312.0 / 12.0, 40, 0.017, enemy1[i].sprite_indicator[5]);
            enemy1[i].velocity.x = -2;
            enemy1[i].sprite.setScale(plScale, plScale);
            enemy1[i].sprite.setOrigin(enemy1[i].sprite.getLocalBounds().width, 0);
            enemy1[i].last_key = LEFT;
            if (enemy1[i].rec.getPosition().x < enemy1[i].initial_position - 150)
            {
                enemy1[i].reversed_direction = 0;
            }
        }

    }
    void death_animation(int i)
    {
        enemy1[i].stopped = 1;
        if (!enemy1[i].death_animation_done)
        {
            enemy1[i].velocity.x = 0;
            enemy1[i].texture.loadFromFile(pathh + "RS Dying Sprite Sheet.png");
            enemy1[i].sprite.setOrigin(341 / 11 / 2, 0);
            enemy1[i].sprite_indicator[1] += 0.1;
            if (enemy1[i].sprite_indicator[1] > 10.9)
            {
                enemy1[i].sprite_indicator[1] = 0.2;
                enemy1[i].death_animation_done = 1;
            }
            enemy1[i].sprite.setTextureRect(IntRect(int(enemy1[i].sprite_indicator[1]) * 341 / 11, 0, 341 / 11, 39));


        }
        else
        {
            enemy1[i].is_alive = 0;
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

float animiindecator[50];

//lvl 1 FG
Texture lvl1FGtex[30];
Sprite Lvl1FG[30];

//lvl 1 A lamps animation
Texture lvl1LampTex[4];
Sprite lvl1lamp[4];

//powerups
RectangleShape powerups(Vector2f(50, 50));
// DECLRATIONS
void TS_Setups();
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
void animation(Sprite& s, float maxframe, float x, float y, float delay, int index);
void playerDamageFromEnemy1();
void playerDeathAnimation();
void drawpistol(RenderWindow& window);
void drawrifle(RenderWindow& window);
int main()
{
    window.setFramerateLimit(60);
    enemy1->setup(enemy1);
    bgSetup();
    TS_Setups();
    Playersetup();
    timer.restart();
    Menu();
    return 0;
}
//DEFINITIONS

void Menu()
{
    while (window.isOpen())
    {
        windowclose();
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
        else {
            if (menu.start_selected)
            {
                TS_PESpr.setColor(Color(255, 255, 255, AlphaPE));
                if (AlphaPE > 0)
                    AlphaPE -= 0.05;
                windowfunction();
            }
            else
            {
                menu.draw();
                if (Keyboard::isKeyPressed(Keyboard::Enter) || menu.ts_escaped)
                {
                    if (!menu.ts_escaped)
                    {
                        MenuClick.play();
                        Clock t;
                        while (true)
                            if (t.getElapsedTime().asMilliseconds() > 300)
                                break;
                    }

                    menu.ts_escaped = true;


                    if ((menu.selected == START && Keyboard::isKeyPressed(Keyboard::Enter)))
                    {
                        MenuClick.play();
                        menu.start_selected = true;
                        windowfunction();
                    }
                    else if (menu.selected == EXIT && Keyboard::isKeyPressed(Keyboard::Enter))
                        window.close();

                    else if (Keyboard::isKeyPressed(Keyboard::Up) && timer.getElapsedTime().asMilliseconds() > 200)
                    {
                        menu.move_up();
                        timer.restart();

                    }
                    else if (Keyboard::isKeyPressed(Keyboard::Down) && timer.getElapsedTime().asMilliseconds() > 200)
                    {

                        menu.move_down();
                        timer.restart();

                    }
                }
            }
        }
    }
}
void bgSetup()
{
    // LEVEL 1 A SET UP
    bgTexture[0].loadFromFile(pathh + "Level 1-A BG.png");
    bgSprite[0].setTexture(bgTexture[0]);
    bgSprite[0].setPosition(-370, -53);
    create(ground, 0, 8830, 30, -370, 800);
    powerups.setPosition(500, 750);
    //lvl 1 ->A FG
    lvl1FGtex[0].loadFromFile(pathh + "Level 1-A FG.png");
    Lvl1FG[0].setTexture(lvl1FGtex[0]);
    Lvl1FG[0].setPosition(-370, -53);
    //lvl 1 A lamps animation
    /*
    Lamp 1 345 * 657 w:1943 h : 149
    Lamp 2 345*657 w:3085 h:149
    Lamp 3 345*657 w:4278 h:149
    Lamp 4 345*657 w:5470 h:149
   */
    lvl1LampTex[0].loadFromFile(pathh + "Level 1-A Lamp 1.png");
    lvl1lamp[0].setTexture(lvl1LampTex[0]);
    lvl1lamp[0].setTextureRect(IntRect(0, 0,345, 657));
    lvl1lamp[0].setPosition(-370+1943, 149-53);


    // LEVEL 1 B->1 SET UP
    bgTexture[1].loadFromFile(pathh + "Level 1-B-1 BG.png");
    bgSprite[1].setTexture(bgTexture[1]);
    bgSprite[1].setPosition(10000, -50);
    create(ground, 1, 5939, 30, 10000, 907);
    create(ground, 2, 300, 18, 15930, 840);
    create(ground, 3, 150, 10, 16245, 625);
    create(ground, 4, 160, 10, 16380, 390);
    create(ground, 5, 325, 18, 16555, 173);
    create(ground, 6, 700, 10, 16850, 40);

    // LEVEL 1 B->2 SET UP
    bgTexture[2].loadFromFile(pathh + "Level 1-B-2 BG.png");
    bgSprite[2].setTexture(bgTexture[2]);
    bgSprite[2].setPosition(14771, -940);

    //animation lv1-B-2(Exit lamp)


    //FG Lv1-B-2
    lvl1FGtex[2].loadFromFile(pathh + "Level 1-B-2 FG.png");
    Lvl1FG[2].setTexture(lvl1FGtex[2]);
    Lvl1FG[2].setPosition(14771, -940);

    create(wall, 0, 20, 80, 15938, 870);
    create(wall, 1, 20, 220, 16235, 640);
    create(wall, 2, 20, 220, 16370, 409);
    create(wall, 3, 10, 200, 16520, 198);
    create(wall, 4, 10, 140, 16840, 60);

    //LEVEL 1 C SET UP
    bgTexture[3].loadFromFile(pathh + "Level 1-C BG.png");
    bgSprite[3].setTexture(bgTexture[3]);
    bgSprite[3].setPosition(18000, 0);

    //fire troches lv1 ->c
    FireTrochestex.loadFromFile(pathh + "Level 1-C Fire Torches.png");
    FireTroches.setTexture(FireTrochestex);
    FireTroches.setTextureRect(IntRect(0, 0, 13120 / 8, 1192));
    FireTroches.setPosition(18000, 0);

    //lvl 1 ->c FG
    lvl1FGtex[3].loadFromFile(pathh + "Level 1-C FG.png");
    Lvl1FG[3].setTexture(lvl1FGtex[3]);
    Lvl1FG[3].setPosition(18000, 0);
    create(ground, 7, 1640, 20, 18000, 970);

    //LEVEL 1 D SET UP
    bgTexture[4].loadFromFile(pathh + "Level 1-D BG.png");
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


    if (Keyboard::isKeyPressed(Keyboard::Q))
    {
        player.gun = RIFLE;
    }
    else if (Keyboard::isKeyPressed(Keyboard::W))
    {
        player.gun = PISTOL;
    }

    //map shortcut
    if (Keyboard::isKeyPressed(sf::Keyboard::T))
        player.upperbodySprite.setPosition(20000, 800);

    cameraView();
    enemy1->movement(enemy1);
    enemy1->Gravity(enemy1);
    enemy1->Damaged(enemy1);
    playerDamageFromEnemy1();

    if (player.health > 0)
        plmovement(player.lowerbodySprite, 11.9, 408 / 12, 41, 0.004, 2);
    player.playerHPSprite.setPosition(view.getCenter().x - 960, view.getCenter().y - 500);
    BGanimation();
    windowclose();
    window.clear();
    for (int i = 0; i < 5; i++)
        window.draw(bgSprite[i]);

    //draw BG animations
    window.draw(FireTroches);
    window.draw(lvl1lamp[0]);
    //window.draw(player.rec);
    for (int i = 0; i < 10; i++) {
        if (enemy1[i].is_alive == 1) {
            //window.draw(enemy1[i].rec);
            window.draw(enemy1[i].sprite);
        }
    }
    window.draw(ground[12]);

    if (player.live)
    {
        window.draw(player.lowerbodySprite);
        if (!player.one_sprite_needed)
            window.draw(player.upperbodySprite);
    }

    for (int i = 0; i < 4; i++)
    {
        if (i == 1)continue;
        window.draw(Lvl1FG[i]);
    }
    if (player.gun == PISTOL)
    {
        drawpistol(window);//draw pistol bullets
    }
    else if (player.gun == RIFLE)
    {
        drawrifle(window); //draw rifle bullets
    }
    //draw enemy1 bullets 
    for (int i = 0; i < 10; i++)
    {
        for (int x = 0; x < enemy1[i].bullet.size(); x++)
        {
            if (enemy1[i].bullet[x].second == LEFT)
            {
                enemy1[i].bullet[x].first.move(-10, 0);
                window.draw(enemy1[i].bullet[x].first);
            }
            else if (enemy1[i].bullet[x].second == RIGHT)
            {
                window.draw(enemy1[i].bullet[x].first);
                enemy1[i].bullet[x].first.move(10, 0);
            }
        }
    }
    // window.draw(ground[0]);
    window.draw(player.playerHPSprite);
    //window.draw(powerups);
    window.setView(view);
    transition_pos_check();
}
void BGanimation()
{
    animation(Exitlamp, 6, 17328 / 6, 2087, 0.01, 0);
    animation(lvl1lamp[0], 8, 345, 657, 0.01, 22);
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
    player.upperbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Upper Body.png");
    player.upperbodySprite.setTexture(player.upperbodyTex);
    player.upperbodySprite.setTextureRect(IntRect(0, 0, 408 / 12, 41));
    player.upperbodySprite.setPosition(100, 600);
    player.upperbodySprite.setScale(plScale, plScale);
    //sprite lowerbody
    player.lowerbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Lower Body.png");
    player.lowerbodySprite.setTexture(player.lowerbodyTex);
    player.lowerbodySprite.setTextureRect(IntRect(0, 0, 408 / 12, 41));
    player.lowerbodySprite.setPosition(100, 600);
    player.lowerbodySprite.setScale(plScale, plScale);
    //rectangle
    player.rec.setPosition(player.upperbodySprite.getPosition().x - 50, player.upperbodySprite.getPosition().y);
    player.rec.setSize(Vector2f(75, 130));
    //HP
    player.playerHPTex.loadFromFile(pathh + "Gradient_Health_Bar.png");
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
        pistol.shoot_timer = 0;
    }
    else
    {
        //functoin -> movement & animation
        move_with_animation(s, maxframe, x, y, delay, index);
        if(player.gun==PISTOL)
            move_with_animation(player.upperbodySprite, maxframe, x, y, delay, index);
        else if(player.gun==RIFLE)
            move_with_animation(player.upperbodySprite, 11.9, 528/11, 29, delay, 32);

    }

    //move player
    if (player.gun == PISTOL)
    {
        s.move(player.Velocity);
        player.upperbodySprite.move(player.Velocity);
    }
    else if (player.gun == RIFLE)//with rifle the speed is slower than pistol
    {
        s.move(player.Velocity.x*0.5,player.Velocity.y);
        player.upperbodySprite.move(player.Velocity.x*0.5,player.Velocity.y);
    }
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
                if (player.gun == PISTOL)
                    pistol.shooting();
                else if (player.gun == RIFLE)
                    rifle.shooting();

                ShootingAnimation();//when pl (move&&shoot)
            }
            //if player move load running sprite sheet
            else
            {
                pistol.shoot_timer = 0;
                if (player.gun == PISTOL)
                {
                    player.upperbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Upper Body.png");
                    player.lowerbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Lower Body.png");
                    animation(s, maxframe, x, y, delay, index);
                }
                else if (player.gun == RIFLE)
                {
                    player.upperbodyTex.loadFromFile(pathh + "Running (Rifle) Sprite Sheet.png");
                    player.lowerbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Lower Body.png");
                    animation(player.upperbodySprite, 11.9, 528 / 12, 29, delay, 32);
                    animation(player.lowerbodySprite, 11.9, 408 / 12, 41, 0.004, 2);
                }


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
                if (player.gun == PISTOL)
                    pistol.shooting();
                else if (player.gun == RIFLE)
                    rifle.shooting();

                ShootingAnimation();//when pl (move&&shoot)
            }
            //if player move load running sprite sheet
            else
            {
                rifle.shoot_timer = 0;
                pistol.shoot_timer = 0;
                if (player.gun == PISTOL)
                {
                    player.upperbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Upper Body.png");
                    player.lowerbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Lower Body.png");
                    animation(s, maxframe, x, y, delay, index);
                }
                else if (player.gun == RIFLE)
                {
                    player.upperbodyTex.loadFromFile(pathh + "Running (Rifle) Sprite Sheet.png");
                    player.lowerbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Lower Body.png");
                    animation(player.upperbodySprite, 11.9, 528 / 12, 29, delay, 32);
                    animation(player.lowerbodySprite, 11.9, 408 / 12, 41, 0.004, 2);
                }
            }
            player.last_key = 2;

            moveToRight(s);

        }

        else
        {
            if (Keyboard::isKeyPressed(Keyboard::J))
            {

                // shooting animation when pl standing
                if (player.gun == PISTOL)
                {
                    pistol.shooting();
                    player.upperbodyTex.loadFromFile(pathh + "Shooting - Standing (Pistol) Sprite Sheet Upper Body.png");
                    animation(player.upperbodySprite, 9.9, 520 / 10, 41, pistolshooting_delay, 10);
                }
                else if (player.gun == RIFLE)
                {
                    rifle.shooting();
                    player.upperbodyTex.loadFromFile(pathh + "Shooting - Standing (Rifle) Sprite Sheet.png");
                    animation(player.upperbodySprite, 3.9, 240 / 4, 27, rifleshooting_delay, 10);
                }

                player.lowerbodyTex.loadFromFile(pathh + "Idle (Pistol) Sprite Sheet Lower Body.png");
                animation(player.lowerbodySprite, 3.9, 128 / 4, 37, 0.04, 3);
            }
            else  if (Keyboard::isKeyPressed(Keyboard::K))
            {
                meeleAnimation();
                pistol.shoot_timer = 0;
            }
            //if player not move then load idle sprite sheet
            else
            {
                IdleAnimation();
                pistol.shoot_timer = 0;
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
    if (player.gun == PISTOL)
    {
        player.lowerbodyTex.loadFromFile(pathh + "Idle (Pistol) Sprite Sheet Lower Body.png");
        animation(player.lowerbodySprite, 3.9, 128 / 4, 37, idle, 3);
        player.is_shooting = 0;
        player.upperbodyTex.loadFromFile(pathh + "Idle (Pistol) Sprite Sheet Upper Body.png");
        animation(player.upperbodySprite, 3.9, 128 / 4, 37, idle, 3);
    }
    else if (player.gun == RIFLE)
    {
        player.lowerbodyTex.loadFromFile(pathh + "Idle (Rifle) Sprite Sheet Lower Body.png");
        animation(player.lowerbodySprite, 3.9, 152 / 4, 37, idle, 3);
        player.is_shooting = 0;
        player.upperbodyTex.loadFromFile(pathh + "Idle (Rifle) Sprite Sheet Upper Body.png");
        animation(player.upperbodySprite, 3.9, 152 / 4, 37, idle, 3);
    }
}
void ShootingAnimation()
{
    player.lowerbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Lower Body.png");
    animation(player.lowerbodySprite, 11.9, 408 / 12, 41, 0.004, 2);
    if (player.gun == PISTOL)
    {
        player.upperbodyTex.loadFromFile(pathh + "Shooting - Standing (Pistol) Sprite Sheet Upper Body.png");
        animation(player.upperbodySprite, 9.9, 520 / 10, 41, pistolshooting_delay, 10);
    }
    else if(player.gun == RIFLE)
    {
        player.upperbodyTex.loadFromFile(pathh + "Shooting - Standing (Rifle) Sprite Sheet.png");
        animation(player.upperbodySprite, 3.9 , 240 / 4, 27, rifleshooting_delay, 10);
    }
}
void crouchingAnimation()
{
    player.crouch = 1;
    player.one_sprite_needed = 1;
    if (Keyboard::isKeyPressed(Keyboard::J))
    {
        if (player.gun == PISTOL)
        {
            pistol.shooting();
            player.lowerbodyTex.loadFromFile(pathh + "Shooting - Crouching (Pistol) Sprite Sheet.png");
            animation(player.lowerbodySprite, 9.9, 520 / 10, 29, pistolshooting_delay, 14);
        }
        else if (player.gun == RIFLE )
        {
            pistol.shooting();
            player.lowerbodyTex.loadFromFile(pathh + "Shooting - Crouching (Rifle) Sprite Sheet.png");
            animation(player.lowerbodySprite, 3.9, 268 / 4, 28, pistolshooting_delay, 14);
        }
        player.lowerbodySprite.setPosition(player.upperbodySprite.getPosition().x, player.upperbodySprite.getPosition().y + player.upperbodyTex.getSize().y - 10);
    }
    else if (Keyboard::isKeyPressed(Keyboard::K))
    {
        player.lowerbodyTex.loadFromFile(pathh + "Melee - Crouching (Pistol) Sprite Sheet.png");
        player.lowerbodySprite.setPosition(player.upperbodySprite.getPosition().x, player.upperbodySprite.getPosition().y + player.upperbodyTex.getSize().y - 30);
        animation(player.lowerbodySprite, 6.9, 294 / 7, 35, pistolshooting_delay, 15);
    }
    else
    {
        player.lowerbodySprite.setPosition(player.upperbodySprite.getPosition().x, player.upperbodySprite.getPosition().y + player.upperbodyTex.getSize().y);
        if (player.gun == PISTOL)
        {
            player.lowerbodyTex.loadFromFile(pathh + "Idle - Crouching (Pistol) Sprite Sheet.png");
            animation(player.lowerbodySprite, 3.9, 136 / 4, 24, idle, 4);
        }
        else if (player.gun == RIFLE)
        {
            player.lowerbodyTex.loadFromFile(pathh + "Idle - Crouching (Rifle) Sprite Sheet.png");
            animation(player.lowerbodySprite, 3.9, 164 / 4, 24, idle, 4);
        }

    }
}
void meeleAnimation()
{
    // melee animation when pl standing
    player.lowerbodyTex.loadFromFile(pathh + "Melee (Pistol) Sprite Sheet Lower Body.png");
    animation(player.lowerbodySprite, 5.9, 288 / 6, 53, melee_delay, 12);
    player.upperbodyTex.loadFromFile(pathh + "Melee (Pistol) Sprite Sheet Upper Body.png");
    animation(player.upperbodySprite, 5.9, 288 / 6, 53, melee_delay, 12);

}
void jumpingAnimation(float delay)
{
    if (player.gun == PISTOL)
    {
        player.upperbodyTex.loadFromFile(pathh + "Jumping (Pistol) Sprite Sheet Upper Body.png");
        animation(player.upperbodySprite, 10.9, 319.0 / 11, 49, delay, 5);
    }
    else if (player.gun == RIFLE)
    {
        player.upperbodyTex.loadFromFile(pathh + "Jumping (Rifle) Sprite Sheet.png");
        animation(player.upperbodySprite, 5.9, 222 / 6, 27, delay, 5);
    }
    player.lowerbodyTex.loadFromFile(pathh + "Jumping (Pistol) Sprite Sheet Lower Body.png");
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
void create(RectangleShape arr[], int index, int width, int hight, int xPosition, int yPostions)
{
    arr[index].setSize(Vector2f(width, hight));
    arr[index].setPosition(xPosition, yPostions);
}
void playerDamageFromEnemy1()
{
    if (player.live)
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < enemy1[i].bullet.size(); j++)
            {
                if ((player.lowerbodySprite.getGlobalBounds().intersects(enemy1[i].bullet[j].first.getGlobalBounds()) || player.upperbodySprite.getGlobalBounds().intersects(enemy1[i].bullet[j].first.getGlobalBounds())) && enemy1[i].bullet[j].second != 0)
                {
                    player.health -= enemy1[i].damage;
                    enemy1[i].bullet[j].second = 0;
                }
            }
        }
        if (player.health <= 0)
        {
            playerDeathAnimation();
        }

    }
}
void playerDeathAnimation()
{
    if (!player.isdead)
    {
        player.Velocity.x = 0;
        //   if (player.gun == PISTOL)
          // {
        player.one_sprite_needed = 1;
        player.lowerbodyTex.loadFromFile(pathh + "Dying Sprite Sheet.png");
        player.lowerbodySprite.setOrigin(480 / 10 / 2, 0);
        animiindecator[30] += 0.1;
        if (animiindecator[30] > 9.9)
        {
            animiindecator[30] = 0.2;
            player.isdead = 1;
        }
        player.lowerbodySprite.setTextureRect(IntRect(int(animiindecator[30]) * 480 / 10, 0, 480 / 10, 44));
        //   }
        //   else if (player.gun == RIFLE)
        //   {

        //   }

    }
    else
    {
        player.live = 0;
    }
}
void drawpistol(RenderWindow& window)
{
    for (int x = 0; x < pistol.rects.size(); x++)
    {
        // Only move the rect here

        if (pistol.rects[x].second == LEFT)
        {
            pistol.rects[x].first.move(-10, 0);
            window.draw(pistol.rects[x].first);
        }
        else if (pistol.rects[x].second == RIGHT)
        {
            window.draw(pistol.rects[x].first);
            pistol.rects[x].first.move(10, 0);
        }
    }
}
void drawrifle(RenderWindow& window)
{
    // Only move the rect here
    for (int x = 0; x < rifle.rects.size(); x++)
    {
        if (rifle.rects[x].second == LEFT)
        {
            rifle.rects[x].first.move(-10, 0);
            window.draw(rifle.rects[x].first);
        }
        else if (rifle.rects[x].second == RIGHT)
        {
            window.draw(rifle.rects[x].first);
            rifle.rects[x].first.move(10, 0);
        }
    }
}
void TS_Setups()
{
    //Sets up the Title Screen & Main Menu Background, Music and Fire Sound Effect.

    TS_BGSpr.setPosition(0.f, 0.f);
    TS_BGTex.loadFromFile(pathh+"TS BG.png");
    TS_BGSpr.setTexture(TS_BGTex);
    TS_BGTheme.openFromFile(pathh+"Title Screen _ Main Menu Theme.wav");
    TS_BGTheme.play();
    TS_BGTheme.setVolume(50.f);
    TS_BGTheme.setLoop(true);
    TS_BGFireFX.openFromFile(pathh+"Title Screen _ Main Menu Fire Sound.wav");
    TS_BGFireFX.play();
    TS_BGFireFX.setVolume(30.f);
    TS_BGFireFX.setLoop(true);


    //Sets up the Title Screen & Main Menu Torches & Gate Animation.
    TS_TandGSpr.setPosition(659.f, 208.f);
    TS_TandGTex.loadFromFile(pathh+"TS Torches _ Gate Sprite Sheet.png");
    TS_TandGSpr.setTexture(TS_TandGTex);



    TS_LSpr.setPosition(722.f, 0.f);
    TS_LTex.loadFromFile(pathh+"TS Lamp Sprite Sheet.png");
    TS_LSpr.setTexture(TS_LTex);


    TS_VSpr.setPosition(0.f, 0.f);
    TS_VTex.loadFromFile(pathh+"Vignette.png");
    TS_VSpr.setTexture(TS_VTex);


    TS_LogoSpr.setPosition(269.f, 465.f);
    TS_LogoTex.loadFromFile(pathh+"TS 1 Sprite Sheet.png");
    TS_LogoSpr.setTexture(TS_LogoTex);

    TS_PESpr.setPosition(612.f, 874.f);
    TS_PETex.loadFromFile(pathh+"TS Press Enter Sprite Sheet.png");
    TS_PESpr.setTexture(TS_PETex);


    TS_buttonsSpr.setPosition(769.f, 650.f);
    TS_buttonsTex.loadFromFile(pathh+"Main Menu Sprite Sheet.png");
    TS_buttonsSpr.setTexture(TS_buttonsTex);
    TS_buttonsSpr.setTextureRect(IntRect(0, 0, 360, 315));

    MenuClickB.loadFromFile(pathh+"Menu Click FX.wav");
    MenuClick.setBuffer(MenuClickB);

    MenuScrollB.loadFromFile(pathh+"Menu Scrolling FX.wav");
    MenuScroll.setBuffer(MenuScrollB);
    MenuScroll.setVolume(100);
}
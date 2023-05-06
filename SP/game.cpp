﻿#include <SFML/Graphics.hpp>
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

//make the variable=
string pathh = "";
const float idle = 0.001,
pistolshooting_delay = 0.007, rifleshooting_delay = 0.005,
plVelocity = 0.2,
plScale = 3.25,
melee_delay = 0.003;
Texture TS_BGTex, TS_TandGTex, TS_LTex, TS_VTex, TS_LogoTex, TS_PETex, TS_buttonsTex, TS_SSTex, TS_OlTex, OptionsTex, MusicControlTex, PMTex;
Sprite TS_BGSpr, TS_TandGSpr, TS_LSpr, TS_VSpr, TS_LogoSpr, TS_PESpr, TS_buttonsSpr, TS_SSSpr, TS_OlSpr, OptionsSpr, MusicControlSpr, PMSpr;
Music TS_BGTheme;
Music TS_BGFireFX;
SoundBuffer MenuClickB, MenuScrollB;
Sound MenuClick, MenuScroll;
Clock timer, timer2, timer4, escTimer;
View view(Vector2f(0, 0), Vector2f(1920, 1080));
float TS_TandGCnt = 0;
float TS_LCnt = 0;
float TS_LogoCnt = 0;
float TS_PECnt = 0;
float AlphaPE = 255;
int TS_ButtonsCnt = 0, TSS_ButtonsCnt = 0, OptionsSprCnt = 0;

// Moved from down to here because 

int bgCounter = 0, leftEnd, rightEnd;//indicator for current map, start of map, end of map
bool  isMoved = false, ismoved2 = 0, ismoved3 = 0;//flags for clearing the previous map
Clock blackscreenTimer;

int full_time_played = 0;

// DECLRATIONS

void window_draw();
void mouse_pos();
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
//player
struct Player
{
    //?
    float plcounter = 0;
    Texture upperbodyTex, lowerbodyTex;
    Sprite upperbodySprite, lowerbodySprite;
    RectangleShape rec;//make rectangle to better collision
    Vector2f Velocity = { 0,0 };
    int health = 100;
    int gun = PISTOL;
    bool canshoot = 0;
    bool crouch = 0;
    bool is_shooting = 0;
    bool one_sprite_needed = 0;
    int last_key = 0;
    bool live = 1, is_getting_damaged = 0;
    Clock damage_timer;
    bool isdead = 0;
    void Playersetup(Player& player)
    {
        //sprite upperbody
        player.upperbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Upper Body.png");
        player.upperbodySprite.setTexture(player.upperbodyTex);
        player.upperbodySprite.setTextureRect(IntRect(0, 0, 408 / 12, 41));
        player.upperbodySprite.setPosition(0, 600);
        player.upperbodySprite.setScale(plScale, plScale);
        //sprite lowerbody
        player.lowerbodyTex.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Lower Body.png");
        player.lowerbodySprite.setTexture(player.lowerbodyTex);
        player.lowerbodySprite.setTextureRect(IntRect(0, 0, 408 / 12, 41));
        player.lowerbodySprite.setPosition(0, 600);
        player.lowerbodySprite.setScale(plScale, plScale);
        //rectangle
        player.rec.setPosition(player.upperbodySprite.getPosition().x - 50, player.upperbodySprite.getPosition().y);
        player.rec.setSize(Vector2f(75, 130));
    }
};
Player player;

// shooting <pistol>
struct Pistol
{
    float  damage = 0.7;
    float shoot_timer = 0;
    vector<pair<RectangleShape, pair<int, double>>>rects;// bullets //checker //position.x
    int range = 900;
    void update_bUllets_distance(Pistol& pistol, int i)
    {
        pistol.rects[i].second.second += 10;
        if (pistol.rects[i].second.second > pistol.range)
            pistol.rects[i].second.first = 0;
    }
    void shooting(Pistol& pistol)
    {
        pistol.shoot_timer += 0.03;
        if (pistol.shoot_timer > 1 || pistol.shoot_timer == 0.03) {
            Vector2f pl = player.lowerbodySprite.getPosition();
            RectangleShape rect(sf::Vector2f(10, 10));
            rect.setOrigin(-pl.x, -(pl.y + 50));
            pistol.rects.push_back({ rect ,{player.last_key,0} });
            pistol.shoot_timer = 0;
        }
    }
    void drawpistol(RenderWindow& window, Pistol& pistol)
    {
        for (int x = 0; x < pistol.rects.size(); x++)
        {
            // Only move the rect here
            if (pistol.rects[x].second.first == LEFT)
            {
                pistol.rects[x].first.move(-10, 0);
                window.draw(pistol.rects[x].first);
            }
            else if (pistol.rects[x].second.first == RIGHT)
            {
                window.draw(pistol.rects[x].first);
                pistol.rects[x].first.move(10, 0);
            }
            pistol.update_bUllets_distance(pistol, x);
        }
    }
} pistol;

// shooting <Rifle>
struct Rifle
{
    float  damage = 0.2;
    float shoot_timer = 0;
    vector<pair<RectangleShape, pair< int, double>>>rects;
    int range = 700,ammo=200;
    void update_bUllets_distance(Rifle& rifle, int i)
    {
        rifle.rects[i].second.second += 14;
        if (rifle.rects[i].second.second > rifle.range)
            rifle.rects[i].second.first = 0;
    }
    void shooting(Rifle& rifle)
    {
        rifle.shoot_timer += 0.08;
        if (rifle.shoot_timer > 1 || rifle.shoot_timer == 0.03) {
            Vector2f pl = player.lowerbodySprite.getPosition();
            RectangleShape rect(sf::Vector2f(10, 10));
            rect.setOrigin(-pl.x, -(pl.y + 50));
            rifle.rects.push_back({ rect ,{player.last_key,0} });
            rifle.shoot_timer = 0;
            rifle.ammo--;
        }

    }
    void drawrifle(Rifle& rifle)
    {
        // Only move the rect here
        for (int x = 0; x < rifle.rects.size(); x++)
        {
            if (rifle.rects[x].second.first == LEFT)
            {
                rifle.rects[x].first.move(-14, 0);
                window.draw(rifle.rects[x].first);
            }
            else if (rifle.rects[x].second.first == RIGHT)
            {
                window.draw(rifle.rects[x].first);
                rifle.rects[x].first.move(14, 0);
            }
            update_bUllets_distance(rifle, x);
        }
    }

} rifle;

//liser
struct Liser
{
    float damage = 0.001;
    vector<pair<RectangleShape, int>>rects;

    void shooting(Liser& liser)
    {
        Vector2f pl = player.lowerbodySprite.getPosition();
        RectangleShape rect(sf::Vector2f(100, 5));
        rect.setOrigin(-pl.x, -(pl.y + 50));
        rect.setFillColor(Color::Red);
        liser.rects.push_back({ rect ,player.last_key });
    }

    void drawliser(Liser& liser)
    {
        for (int x = 0; x < liser.rects.size(); x++)
        {
            if (liser.rects[x].second == LEFT)
            {
                liser.rects[x].first.move(-100, 0);
                window.draw(liser.rects[x].first);
            }
            else if (liser.rects[x].second == RIGHT)
            {
                window.draw(liser.rects[x].first);
                liser.rects[x].first.move(100, 0);
            }
        }
    }
}liser;

struct Blood_Spatter
{
    Texture tex;
    Sprite sprite;
    float indicator;//sprite indicator
    bool animationdone;
    void setup(Blood_Spatter& blood)
    {
        blood.tex.loadFromFile(pathh + "Blood Spatter.png");
        blood.sprite.setTexture(blood.tex);
        blood.sprite.setTextureRect(IntRect(0, 0, 500, 500));
        blood.sprite.setScale(0.5, 0.5);
    }
    void update(Blood_Spatter& blood)
    {
        blood.indicator += 0.229;
        if (blood.indicator > 2.9)
        {
            blood.indicator = 0;
            player.damage_timer.restart();
            player.is_getting_damaged = 0;
        }
        blood.sprite.setTextureRect(IntRect(int(blood.indicator) * 500, 0, 500, 500));
    }
    void move(Blood_Spatter& blood)
    {
        Vector2f pl = player.lowerbodySprite.getPosition();
        blood.sprite.setPosition(pl.x - 120, pl.y - 50);
    }
    void call(Blood_Spatter& blood)
    {
        if (player.is_getting_damaged == 1 && player.damage_timer.getElapsedTime().asMilliseconds() > 300)  // this adds blood spatter to player when damaged
        {
            blood.update(blood);
            window.draw(blood.sprite);
            player.upperbodySprite.setColor(Color::Red);
            player.lowerbodySprite.setColor(Color::Red);
        }
        else
        {
            player.upperbodySprite.setColor(Color::White);
            player.lowerbodySprite.setColor(Color::White);
        }
    }
}blood;

struct MusicSoundControl
{
    int selected = 1, soundlevel = 100;
    bool isOpen = false;

    void move_right()
    {
        selected = (selected + 1 <= 7 ? selected + 1 : 7);
        soundlevel = (100 - (selected - 1) * 15 < 11 ? 0 : 100 - (selected - 1) * 15);
        TS_BGTheme.setVolume(soundlevel);
    }

    void move_left()
    {
        selected = (selected - 1 == 0 ? 1 : selected - 1);
        soundlevel = (100 - (selected - 1) * 15 < 11 ? 0 : 100 - (selected - 1) * 15);
        TS_BGTheme.setVolume(soundlevel);
    }
    void draw(string section = "")
    {
        window.clear();

        if (section == "main")
        {
            TS_TandGCnt += 0.139;
            if (TS_TandGCnt > 8)
                TS_TandGCnt -= 8;
            TS_TandGSpr.setTextureRect(IntRect((int)TS_TandGCnt * 600, 0, 600, 600));

            TS_LCnt += 0.139;
            if (TS_LCnt > 10)
                TS_LCnt -= 10;
            TS_LSpr.setTextureRect(IntRect((int)TS_LCnt * 473, 0, 473, 261));
            OptionsSpr.setTextureRect(IntRect(OptionsSprCnt * 843, 0, 843, 441));
            MusicControlSpr.setTextureRect(IntRect((selected - 1) * 755, 0, 755, 240));
            window.draw(TS_BGSpr);
            window.draw(TS_TandGSpr);
            window.draw(TS_LSpr);
            window.draw(TS_OlSpr);
            window.draw(MusicControlSpr);
            window.display();
        }

        else
        {
            TS_OlSpr.setScale(3 * view.getSize().x / 1920.0, 3 * view.getSize().y / 1080.0);
            TS_OlSpr.setPosition(view.getCenter().x - (1000 * view.getSize().x / 1920), view.getCenter().y - (600 * view.getSize().y / 1080.0));
            MusicControlSpr.setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
            MusicControlSpr.setPosition(view.getCenter().x - (400 * view.getSize().x / 1920.0), view.getCenter().y - (150 * view.getSize().y / 1080.0));

            MusicControlSpr.setTextureRect(IntRect((selected - 1) * 755, 0, 755, 240));
            window.clear();
            window_draw();
            window.draw(TS_OlSpr);
            window.draw(MusicControlSpr);
            window.display();

        }

    }

};
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
    void draw()
    {
        TS_OlSpr.setScale(3 * view.getSize().x / 1920.0, 3 * view.getSize().y / 1080.0);
        TS_OlSpr.setPosition(view.getCenter().x - (1000 * view.getSize().x / 1920), view.getCenter().y - (600 * view.getSize().y / 1080.0));
        PMSpr.setScale(0.7 * view.getSize().x / 1920.0, 0.7 * view.getSize().y / 1080.0);
        PMSpr.setPosition(view.getCenter().x - (195 * view.getSize().x / 1920.0), view.getCenter().y - 60 * (view.getSize().y / 1080.0));
        PMSpr.setTextureRect(IntRect((selected - 1) * 588, 0, 588, 352));
        window.draw(TS_OlSpr);
        window.draw(PMSpr);
        window.display();

    }
}pause_menu;

struct Start
{
    int selected = 1;

    void move_up()
    {
        selected = (selected == 1 ? 3 : selected - 1);
        MenuScroll.play();
        TSS_ButtonsCnt = selected - 1;
    }

    void move_down()
    {
        selected = (selected == 3 ? 1 : selected + 1);
        MenuScroll.play();
        TSS_ButtonsCnt = selected - 1;
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
        TS_SSSpr.setTextureRect(IntRect(TSS_ButtonsCnt * 400, 0, 400, 315));
        window.draw(TS_BGSpr);
        window.draw(TS_TandGSpr);
        window.draw(TS_LSpr);
        window.draw(TS_VSpr);
        window.draw(TS_LogoSpr);
        window.draw(TS_SSSpr);
        window.display();
    }
};

struct OptionsScreen
{
    int selected = 1;
    bool isOpen = false;
    MusicSoundControl music_slider;
    void move_up()
    {
        selected = (selected == 1 ? 2 : 1);
        OptionsSprCnt = selected - 1;
    }

    void move_down()
    {
        selected = (selected == 1 ? 2 : 1);
        OptionsSprCnt = selected - 1;
    }

    void exit()
    {
        isOpen = false;
    }

    void draw(string section = "")
    {
        if (section == "menu")
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
            OptionsSpr.setTextureRect(IntRect(OptionsSprCnt * 843, 0, 843, 441));
            OptionsSpr.setPosition(539.f, 400.f);

            window.draw(TS_BGSpr);
            window.draw(TS_TandGSpr);
            window.draw(TS_LSpr);
            window.draw(TS_OlSpr);
            window.draw(OptionsSpr);
            window.display();
        }
        else
        {
            OptionsSpr.setPosition(view.getCenter().x - (420 * view.getSize().x / 1920.0), view.getCenter().y - (100 * view.getSize().y / 1080.0));
            OptionsSpr.setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
            OptionsSpr.setTextureRect(IntRect(OptionsSprCnt * 843, 0, 843, 441));
            window.clear();
            window_draw();
            window.draw(TS_OlSpr);
            window.draw(OptionsSpr);
            window.display();
        }
    }
};

//menu
struct Menu
{
    int selected = 1;
    Start start_screen;
    OptionsScreen options_screen;

    bool game_started = false, start_selected = false,
        is_paused = false, ts_escaped = false;

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
    void main_screen_draw()
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

//HUD
struct HUD {
    // health bar
    Texture HP_bar_tex;
    Sprite HP_bar_sprite;
    int hp_index = 0;
    //Mug shot
    Texture Mug_shot_tex;
    Sprite Mug_shot_sprite;
    //Lives
    Texture lives_tex;
    Sprite lives_sprite;
    // lives number
    Texture lives_number_tex;
    Sprite lives_number_1_sprite, lives_number_2_sprite;
    int lives_num1_index = 9, lives_num2_index = 6;
    //Ammo
    // Ammo numbers for rifle
    Texture ammo_num_tex;
    Sprite ammo_num_sprite[3];
    int ammo_num_index[3]{ 0,0,0 };
    // infinity for pistol
    Texture infinity_tex;
    Sprite infinity_sprite;
    // weapons holder
    Texture weapons_holder_tex;
    Sprite weapons_holder_sprite;
    //Time
    Texture time_num_tex;
    Sprite time_num1_sprite, time_num2_sprite;
    int time_num1_index = 9, time_num2_index = 9;
    // 9-time index= actual time
    Clock hud_time;
    int hud_int_time = hud_time.getElapsedTime().asSeconds();
    //score
    Texture score_num_tex;
    Sprite score_num_sprite[6];
    int score_num_index[6]{ 9,9,9,9,9,9 };

    void setup(HUD& hud) {
        hud.HP_bar_tex.loadFromFile("Health Bar.png");
        hud.HP_bar_sprite.setTexture(hud.HP_bar_tex);
        hud.HP_bar_sprite.setScale(3.455, 3.455);
        hud.HP_bar_sprite.setTextureRect(IntRect(hp_index * (390 / 6), 0, 390 / 6, 11));

        hud.Mug_shot_tex.loadFromFile("Mugshot.png");
        hud.Mug_shot_sprite.setTexture(hud.Mug_shot_tex);
        hud.Mug_shot_sprite.setScale(3.5, 3.5);

        hud.lives_tex.loadFromFile("Lives.png");
        hud.lives_sprite.setTexture(hud.lives_tex);

        hud.infinity_tex.loadFromFile("Infinity.png");
        hud.infinity_sprite.setTexture(hud.infinity_tex);

        hud.weapons_holder_tex.loadFromFile("Weapons Holder.png");
        hud.weapons_holder_sprite.setTexture(hud.weapons_holder_tex);
        hud.weapons_holder_sprite.setScale(2.7, 2.7);

        hud.lives_number_tex.loadFromFile("Numbers Lives.png");
        hud.lives_number_1_sprite.setTexture(hud.lives_number_tex);
        hud.lives_number_1_sprite.setTextureRect(IntRect(lives_num1_index * (290 / 10), 0, 290 / 10, 29));
        hud.lives_number_2_sprite.setTexture(hud.lives_number_tex);
        hud.lives_number_2_sprite.setTextureRect(IntRect(lives_num2_index * (290 / 10), 0, 290 / 10, 29));

        hud.time_num_tex.loadFromFile("Numbers.png");
        hud.time_num1_sprite.setTexture(hud.time_num_tex);
        hud.time_num1_sprite.setTextureRect(IntRect(time_num1_index * (550 / 10), 0, 550 / 10, 55));
        hud.time_num2_sprite.setTexture(hud.time_num_tex);
        hud.time_num2_sprite.setTextureRect(IntRect(time_num2_index * (550 / 10), 0, 550 / 10, 55));

        hud.score_num_tex.loadFromFile("Numbers Score.png");
        for (int i = 0; i < 6; i++)
        {
            hud.score_num_sprite[i].setTexture(hud.score_num_tex);
            hud.score_num_sprite[i].setTextureRect(IntRect(score_num_index[i] * (360 / 10), 0, 360 / 10, 36));
        }

        hud.ammo_num_tex.loadFromFile("Numbers Ammo.png");
        for (int i = 0; i < 3; i++)
        {
            hud.ammo_num_sprite[i].setTexture(hud.ammo_num_tex);
            hud.ammo_num_sprite[i].setTextureRect(IntRect(ammo_num_index[i] * (390 / 10), 0, 390 / 10, 39));
        }

    }
    void positions(HUD& hud) {
        hud.HP_bar_sprite.setScale(3.455 * view.getSize().x / 1920.0, 3.455 * view.getSize().y / 1080.0);
        hud.HP_bar_sprite.setPosition(view.getCenter().x - (850 * view.getSize().x / 1920.0), view.getCenter().y - (474 * view.getSize().y / 1080.0));
        
        hud.Mug_shot_sprite.setScale(3.5 * view.getSize().x / 1920.0, 3.5 * view.getSize().y / 1080.0);
        hud.Mug_shot_sprite.setPosition(view.getCenter().x - (940 * view.getSize().x / 1920.0), view.getCenter().y - (520 * view.getSize().y / 1080.0));

        hud.lives_sprite.setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
        hud.lives_sprite.setPosition(view.getCenter().x + (783 * view.getSize().x / 1920.0), view.getCenter().y - (520 * view.getSize().y / 1080.0));

        hud.infinity_sprite.setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
        hud.infinity_sprite.setPosition(view.getCenter().x - (771 * view.getSize().x / 1920.0), view.getCenter().y + (453 * view.getSize().y / 1080.0));

        hud.weapons_holder_sprite.setScale(2.7 * view.getSize().x / 1920.0, 2.7 * view.getSize().y / 1080.0);
        hud.weapons_holder_sprite.setPosition(view.getCenter().x - (940 * view.getSize().x / 1920.0), view.getCenter().y + (439 * view.getSize().y / 1080.0));

        hud.lives_number_1_sprite.setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
        hud.lives_number_1_sprite.setPosition(view.getCenter().x + (882 * view.getSize().x / 1920.0), view.getCenter().y - (510 * view.getSize().y / 1080.0));

        hud.lives_number_2_sprite.setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
        hud.lives_number_2_sprite.setPosition(view.getCenter().x + (911 * view.getSize().x / 1920.0), view.getCenter().y - (510 * view.getSize().y / 1080.0));

        hud.time_num1_sprite.setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
        hud.time_num1_sprite.setPosition(view.getCenter().x - (57 * view.getSize().x / 1920.0), view.getCenter().y - (520 * view.getSize().y / 1080.0));

        hud.time_num2_sprite.setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
        hud.time_num2_sprite.setPosition(view.getCenter().x + (2 * view.getSize().x / 1920.0), view.getCenter().y - (520 * view.getSize().y /1080.0));

        for (int i = 0; i < 6; i++)
        {
            hud.score_num_sprite[i].setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
            hud.score_num_sprite[i].setPosition(view.getCenter().x - (850 * view.getSize().x / 1920.0) + i * 38, view.getCenter().y - (520 * view.getSize().y / 1080.0));
        }

        for (int i = 0; i < 3; i++)
        {
            hud.ammo_num_sprite[i].setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
            hud.ammo_num_sprite[i].setPosition(view.getCenter().x - (771 * view.getSize().x / 1920.0) + i * 41, view.getCenter().y + (460 * view.getSize().y * 1080.0));
        }

    }
    void draw(HUD hud) {
        hud.HP_bar_sprite.setTextureRect(IntRect(hp_index * (390 / 6), 0, 390 / 6, 11));
        window.draw(hud.HP_bar_sprite);

        window.draw(hud.Mug_shot_sprite);
        window.draw(hud.lives_sprite);
        window.draw(hud.weapons_holder_sprite);
        window.draw(hud.lives_number_1_sprite);
        window.draw(hud.lives_number_2_sprite);
        hud.time_num1_sprite.setTextureRect(IntRect(time_num1_index * (550 / 10), 0, 550 / 10, 55));
        window.draw(hud.time_num1_sprite);
        hud.time_num2_sprite.setTextureRect(IntRect(time_num2_index * (550 / 10), 0, 550 / 10, 55));
        window.draw(hud.time_num2_sprite);

        for (int i = 0; i < 6; i++)
            window.draw(hud.score_num_sprite[i]);
		for (int i = 0; i < 3; i++)
		{
			hud.ammo_num_sprite[i].setTextureRect(IntRect(ammo_num_index[i] * (390 / 10), 0, 390 / 10, 39));
		}
        if (player.gun == RIFLE)
            for (int i = 0; i < 3; i++)
                window.draw(hud.ammo_num_sprite[i]);

        else if (player.gun == PISTOL)
            window.draw(hud.infinity_sprite);
    }
    void time_calculation() {
        hud_int_time = hud_time.getElapsedTime().asSeconds();
        if (!(time_num1_index == 0 && time_num2_index == 0)) {
            time_num2_index = 9 - (hud_int_time % 10);
            time_num1_index = 9 - (hud_int_time / 10);
        }
        else {
            full_time_played += hud_time.getElapsedTime().asSeconds();
            hud_time.restart();
        }
    }
    void ammo_display(){
        ammo_num_index[2] = 9 - (rifle.ammo % 10);//123
        ammo_num_index[1] = 9-((rifle.ammo/10)-(rifle.ammo/100)*10);
        ammo_num_index[0] = 9 - (rifle.ammo / 100);

    }
}hud;
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
    bool death_animation_done = 0, is_getting_damaged = 0;
    Clock damage_timer;
    bool check = 1; //to fix RS Equipping Sprite dimensions
    int last_key = RIGHT;
    float shoot_timer = 0;
    bool stopped = 0;
    float initial_position;
    bool reversed_direction = 0;

    vector <pair <RectangleShape, int>> bullet;//enemy1 pistol
    bool is_alive = 1;
    void setup(Enemy1 enemy1[30])
    {
        for (int i = 0; i < 30; i++)
        {
            enemy1[i].texture.loadFromFile(pathh + "RS Running Sprite Sheet.png");
            enemy1[i].sprite.setTexture(enemy1[i].texture);
            enemy1[i].sprite.setTextureRect(IntRect(0, 0, 312 / 12, 40));
        }
        for (int i = 0; i < 30; i++)
        {
            enemy1[i].rec.setSize(Vector2f(100, 130));
            enemy1[i].rec.setPosition(Vector2f(1000 + 200 * i, 600));
            enemy1[i].sprite.setPosition(Vector2f(1000 + 200 * i, 600));
            enemy1[i].initial_position = 1000 + 200 * i;
            enemy1[i].sprite.setScale(plScale, plScale);
        }
    }
    void movement(Enemy1 enemy1[30])
    {
        for (int i = 0; i < 30; i++)
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
                            Carrying_a_weapon(i, enemy1);
                        }
                        else
                        {
                            EnemyShootingAnimation(i, enemy1);
                        }
                    }
                    else
                    {
                        EnemyRunningToPl(i, enemy1);
                    }
                }
                else
                { // Player not in range
                    if (i % 2 == 0)
                        Aimless_Walking(i, enemy1);
                    else if (i % 3 == 0)
                    {
                        Idle_animation_1(i, enemy1);
                        enemy1[i].velocity.x = 0;
                    }
                    else
                    {
                        Idle_animation_2(i, enemy1);
                        enemy1[i].velocity.x = 0;
                    }
                }
            }
        }
        for (int i = 0; i < 30; i++)
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
    void Gravity(Enemy1 enemy1[30])
    {
        for (int j = 0; j < 30; j++)
        {

            float x = enemy1[j].rec.getPosition().x;
            enemy1[j].rec.setPosition(x, 670);
        }
    }
    void Damaged(Enemy1 enemy1[30])
    {
        for (int i = 0; i < 30; i++)
        {
            if (enemy1[i].is_alive)
            {

                for (int j = 0; j < pistol.rects.size(); j++)
                {
                    if (enemy1[i].sprite.getGlobalBounds().intersects(pistol.rects[j].first.getGlobalBounds()) && pistol.rects[j].second.first != 0)
                    {
                        enemy1[i].velocity.x = 0;
                        enemy1[i].health -= pistol.damage;
                        enemy1[i].is_getting_damaged = 1;
                        pistol.rects[j].second.first = 0;
                    }
                }
                for (int j = 0; j < rifle.rects.size(); j++)
                {
                    if (enemy1[i].sprite.getGlobalBounds().intersects(rifle.rects[j].first.getGlobalBounds()) && rifle.rects[j].second.first != 0)
                    {
                        enemy1[i].velocity.x = 0;
                        enemy1[i].health -= rifle.damage;
                        enemy1[i].is_getting_damaged = 1;
                        rifle.rects[j].second.first = 0;
                    }
                }
                for (int j = 0; j < liser.rects.size(); j++)
                {
                    if (enemy1[i].sprite.getGlobalBounds().intersects(liser.rects[j].first.getGlobalBounds()) && liser.rects[j].second != 0)
                    {
                        enemy1[i].velocity.x = 0;
                        enemy1[i].health -= liser.damage;
                        enemy1[i].is_getting_damaged = 1;
                        liser.rects[j].second = 0;
                    }
                }
                if (enemy1[i].is_getting_damaged == 1)  // this adds red color to enemies when damaged
                {
                    if (enemy1[i].damage_timer.getElapsedTime().asMilliseconds() <= 300) {
                        enemy1[i].sprite.setColor(Color::Red);
                    }
                    else {
                        enemy1[i].is_getting_damaged = 0;
                    }
                }
                else
                {
                    enemy1[i].sprite.setColor(Color::White);
                    enemy1[i].damage_timer.restart();
                }
                if (enemy1[i].health <= 0)
                {
                    enemy1[i].death_animation(i, enemy1);
                }
            }
        }
    }
    void shooting(int i, Enemy1 enemy1[30])
    {
        enemy1[i].shoot_timer += 0.08;
        if (enemy1[i].shoot_timer > 1 || enemy1[i].shoot_timer == 0.05) {
            Vector2f pl = enemy1[i].sprite.getPosition();
            RectangleShape rect(sf::Vector2f(20, 10));
            rect.setOrigin(-pl.x, -(pl.y + 50));
            enemy1[i].bullet.push_back({ rect ,enemy1[i].last_key });
            enemy1[i].shoot_timer = 0;
        }
    }
    void Carrying_a_weapon(int i, Enemy1 enemy1[30])//i==index of enemy
    {
        enemy1[i].check = 0;
        enemy1[i].velocity.x = 0;
        enemy1[i].texture.loadFromFile(pathh + "RS Equipping Sprite Sheet.png");
        enemy1[i].sprite.setOrigin(322 / 7 / 2, 0);
        //   animation(enemy1[i].sprite, 6.9, 322 / 7, 44, 0.002, 9);
        enemy1[i].sprite_indicator[1] += 0.28;
        if (enemy1[i].sprite_indicator[1] > 6.9)
        {
            enemy1[i].sprite_indicator[1] = 0;
            enemy1[i].isCarrying_a_weapon = 1;
        }
        enemy1[i].sprite.setTextureRect(IntRect(int(enemy1[i].sprite_indicator[1]) * 322 / 7, 0, 322 / 7, 44));
    }
    void EnemyShootingAnimation(int i, Enemy1 enemy1[30])
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
    void EnemyRunningToPl(int i, Enemy1 enemy1[30])
    {
        enemy1[i].shoot_timer = 1;
        enemy1[i].isCarrying_a_weapon = 0;
        enemy1[i].texture.loadFromFile(pathh + "RS Running Sprite Sheet.png");
        EnemiAnimation(enemy1[i].sprite, 11.9, 312 / 12, 40, 0.017, enemy1[i].sprite_indicator[2]);
        if (player.upperbodySprite.getPosition().x > enemy1[i].sprite.getPosition().x && player.health > 0)
        {
            if (i % 2 == 0)
                enemy1[i].velocity.x = 3;
            else if (i % 3 == 0)
                enemy1[i].velocity.x = 4.5;
            else
                enemy1[i].velocity.x = 3;
            enemy1[i].sprite.setScale(-plScale, plScale);
            enemy1[i].sprite.setOrigin(0, 0);
            enemy1[i].last_key = RIGHT;
        }
        else if (player.upperbodySprite.getPosition().x < enemy1[i].sprite.getPosition().x && player.health > 0)
        {
            if (i % 2 == 0)
                enemy1[i].velocity.x = -3;
            else if (i % 3 == 0)
                enemy1[i].velocity.x = -4.5;
            else
                enemy1[i].velocity.x = -3;
            enemy1[i].sprite.setScale(plScale, plScale);
            enemy1[i].sprite.setOrigin(enemy1[i].sprite.getLocalBounds().width, 0);
            enemy1[i].last_key = LEFT;
        }
        else
        {
            if (i % 2 == 0)
                enemy1[i].velocity.x = 3;
            else if (i % 3 == 0)
                enemy1[i].velocity.x = 4.5;
            else
                enemy1[i].velocity.x = 3;
            enemy1[i].sprite.setScale(-plScale, plScale);
            enemy1[i].sprite.setOrigin(0, 0);
            enemy1[i].last_key = RIGHT;
        }

    }
    void Aimless_Walking(int i, Enemy1 enemy1[30])
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
            if (enemy1[i].rec.getPosition().x > enemy1[i].initial_position + 100)
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
            if (enemy1[i].rec.getPosition().x < enemy1[i].initial_position - 100)
            {
                enemy1[i].reversed_direction = 0;
            }
        }

    }
    void death_animation(int i, Enemy1 enemy1[30])
    {
        enemy1[i].stopped = 1;
        if (!enemy1[i].death_animation_done)
        {
            enemy1[i].velocity.x = 0;
            enemy1[i].texture.loadFromFile(pathh + "RS Dying Sprite Sheet.png");
            enemy1[i].sprite.setOrigin(341 / 11 / 2, 0);
            enemy1[i].sprite_indicator[1] += 0.2;
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
    void Idle_animation_2(int i, Enemy1 enemy1[30])
    {
        enemy1[i].texture.loadFromFile(pathh + "RS Idle 2 Sprite Sheet.png");
        EnemiAnimation(enemy1[i].sprite, 7.9, 232 / 8, 37, 0.005, enemy1[i].sprite_indicator[4]);
    }
    void Idle_animation_1(int i, Enemy1 enemy1[30])
    {
        enemy1[i].texture.loadFromFile(pathh + "RS Idle 1 Sprite Sheet.png");
        EnemiAnimation(enemy1[i].sprite, 5.9, 174 / 6, 38, 0.005, enemy1[i].sprite_indicator[5]);
    }
    void drawbullets(Enemy1 RS[30])
    {
        for (int i = 0; i < 30; i++)
        {
            for (int x = 0; x < RS[i].bullet.size(); x++)
            {
                if (RS[i].bullet[x].second == LEFT)
                {
                    RS[i].bullet[x].first.move(-10, 0);
                    window.draw(RS[i].bullet[x].first);
                }
                else if (RS[i].bullet[x].second == RIGHT)
                {
                    window.draw(RS[i].bullet[x].first);
                    RS[i].bullet[x].first.move(10, 0);
                }
            }
        }
    }
    void draw(Enemy1 RS[30])
    {
        for (int i = 0; i < 30; i++) {
            if (RS[i].is_alive == 1) {
                //window.draw(enemy1[i].rec);
                window.draw(RS[i].sprite);
            }
        }
        RS->drawbullets(RS);
    }
}RS[30];//Rebel Soldier

struct Enemy2
{
    Texture RWTex;
    Sprite RWSpr;
    int RwInitialPos = 12500;

    void setup(Enemy2 enemy2[5])
    {
        for (int i = 0; i < 5; i++)
        {
            enemy2[i].RWTex.loadFromFile(pathh + "RW Idle Sprite Sheet.png");
            enemy2[i].RWSpr.setTexture(enemy2[i].RWTex);
            enemy2[i].RWSpr.setScale(plScale, plScale);
            enemy2[i].RWSpr.setTextureRect(IntRect(0, 0, 36, 48));
            enemy2[i].RWSpr.setPosition(RwInitialPos + i * 200, 765);
        }
    }
    void movement();
    void fighting();
    void death();
    void draw(Enemy2 enemy2[5])
    {
        for (int i = 0; i < 5; i++)
            window.draw(enemy2[i].RWSpr);
    }
}enemy2[5];

struct Tank
{
    // Tank shooting
    struct Tank_shooting
    {
        float  damage = 30;
        vector<pair<RectangleShape, int>>rects;// bullets //checker 
        Clock shooting_timer;
        void shooting(Tank_shooting& tank_shooting, Tank& tank)
        {
            Vector2f tankpos = tank.tankSprite.getPosition();
            RectangleShape rect(sf::Vector2f(40, 10));
            rect.setOrigin(-tankpos.x, -(tankpos.y - 150));
            tank_shooting.rects.push_back({ rect ,tank.last_key });
        }
        void draw_tank_bullets(Tank_shooting& tank_shooting)
        {
            for (int x = 0; x < tank_shooting.rects.size(); x++)
            {
                // Only move the rect here
                if (tank_shooting.rects[x].second == LEFT)
                {
                    tank_shooting.rects[x].first.move(-15, 0);
                    window.draw(tank_shooting.rects[x].first);
                }
                else if (tank_shooting.rects[x].second == RIGHT)
                {
                    window.draw(tank_shooting.rects[x].first);
                    tank_shooting.rects[x].first.move(15, 0);
                }
            }
        }
    }shooting;


    Texture tanktex;
    Sprite tankSprite;
    int tankInitialPos = 400;
    float tankAnimationInd[4];
    bool isdead = false;
    int dmg = 100;
    int health = 1000;
    int last_key = RIGHT;
    //127 × 48 pixels
    void setup(Tank& tank)
    {
        tank.tanktex.loadFromFile(pathh + "Tank Idle Sprite Sheet.png");
        tank.tankSprite.setTexture(tank.tanktex);
        tank.tankSprite.setTextureRect(IntRect(0, 0, 64, 48));
        tank.tankSprite.setOrigin(32, 48);
        tank.tankSprite.setScale(4, 4);
        tank.tankSprite.setPosition(11500, 950);
    }
    void draw(Tank& tank)
    {
        window.draw(tank.tankSprite);
        tank.shooting.draw_tank_bullets(tank.shooting);
    }
    void tankIdleAnimation(Tank& tank)
    {//127 × 48
        tank.tanktex.loadFromFile(pathh + "Tank Idle Sprite Sheet.png");
        EnemiAnimation(tank.tankSprite, 2, 127 / 2, 48, 0.004, tank.tankAnimationInd[0]);
    }
    void tankMovingAnimation(Tank& tank)
    {//512 × 48
        tank.tanktex.loadFromFile(pathh + "Tank Moving Forward Sprite Sheet.png");
        EnemiAnimation(tank.tankSprite, 8, 512 / 8, 48, 0.01, tank.tankAnimationInd[1]);
    }
    void tankShootingAnimation(Tank& tank)
    {//260 × 48
        tank.tanktex.loadFromFile(pathh + "Tank Shooting Sprite Sheet.png");
        tank.tankAnimationInd[2] += 0.004 * dt;
        if (tank.tankAnimationInd[2] > 3.9)
        {
            tank.tankAnimationInd[2] = 0;
            tank.shooting.shooting(tank.shooting, tank);
            tank.shooting.shooting_timer.restart();
        }
        tank.tankSprite.setTextureRect(IntRect(int(tank.tankAnimationInd[2]) * 260 / 4, 0, 260 / 4, 48));

    }
    void tankDeathAnimation(Tank& tank)
    {//2133 × 200
        tank.tanktex.loadFromFile(pathh + "Tank Destroyed Sprite Sheet");
        EnemiAnimation(tank.tankSprite, 27, 2133 / 27, 200, 0.005, tank.tankAnimationInd[3]);
    }

    void tankState(Tank& tank)
    {
        int distance = tank.tankSprite.getPosition().x - player.upperbodySprite.getPosition().x;
        if (abs(distance) > 1000)
        {
            tankIdleAnimation(tank);
        }
        else if (abs(distance) < 600)
        {

            if (tank.shooting.shooting_timer.getElapsedTime().asMilliseconds() > 1000)
            {
                tankShootingAnimation(tank);
            }
        }
        else
        {
            tankMovingAnimation(tank);

            if (distance > 0)
            {
                tank.tankSprite.setScale(4, 4);
                tank.tankSprite.move(-3, 0);
                tank.last_key = LEFT;
            }
            else
            {
                tank.tankSprite.setScale(-4, 4);
                tank.tankSprite.move(3, 0);
                tank.last_key = RIGHT;
            }
        }
    }
}tank;


Clock push_timer;
float delay = 0.1;
struct Enemy3
{
    Texture texture;
    Sprite  sprite;
    RectangleShape rec;
    Vector2f velocity = { 0,0 };
    bool isCarrying_a_weapon = 0;
    float shoot_timer = 0;
    bool stopped = 0;
    int last_key = RIGHT;
    bool death_animation_done = 0, is_getting_damaged = 0;
    float sprite_indicator[10] = { };
    bool is_alive = 1;
    Clock damage_timer;
    int health = 10;
    int damage = 1;
    bool check = 1;
    vector <pair <RectangleShape, int>> bullet; //enemy3 pistol

    void Damaged(vector<Enemy3>& enemy3)
    {
        for (int i = 0; i < enemy3.size(); i++)
        {
            if (enemy3[i].is_alive)
            {

                for (int j = 0; j < pistol.rects.size(); j++)
                {
                    if (enemy3[i].sprite.getGlobalBounds().intersects(pistol.rects[j].first.getGlobalBounds()) && pistol.rects[j].second.first != 0)
                    {
                        enemy3[i].velocity.x = 0;
                        enemy3[i].health -= pistol.damage;
                        enemy3[i].is_getting_damaged = 1;
                        pistol.rects[j].second.first = 0;
                    }
                }
                for (int j = 0; j < rifle.rects.size(); j++)
                {
                    if (enemy3[i].sprite.getGlobalBounds().intersects(rifle.rects[j].first.getGlobalBounds()) && rifle.rects[j].second.first != 0)
                    {
                        enemy3[i].velocity.x = 0;
                        enemy3[i].health -= rifle.damage;
                        enemy3[i].is_getting_damaged = 1;
                        rifle.rects[j].second.first = 0;
                    }
                }
                for (int j = 0; j < liser.rects.size(); j++)
                {
                    if (enemy3[i].sprite.getGlobalBounds().intersects(liser.rects[j].first.getGlobalBounds()) && liser.rects[j].second != 0)
                    {
                        enemy3[i].velocity.x = 0;
                        enemy3[i].health -= liser.damage;
                        enemy3[i].is_getting_damaged = 1;
                        liser.rects[j].second = 0;
                    }
                }
                if (enemy3[i].is_getting_damaged == 1)  // this adds red color to enemies when damaged
                {
                    if (enemy3[i].damage_timer.getElapsedTime().asMilliseconds() <= 300) {
                        enemy3[i].sprite.setColor(Color::Red);
                    }
                    else {
                        enemy3[i].is_getting_damaged = 0;
                    }
                }
                else
                {
                    enemy3[i].sprite.setColor(Color::White);
                    enemy3[i].damage_timer.restart();
                }
                    enemy3[i].death_animation(enemy3);
                
            }
        }
    }
    void death_animation(vector<Enemy3>& enemy3)
    {
        for (int i = 0; i < enemy3.size(); i++)
        {
            if (enemy3[i].health <= 0)
            {
                enemy3[i].stopped = 1;
                if (!enemy3[i].death_animation_done)
                {
                    enemy3[i].velocity.x = 0;
                    enemy3[i].texture.loadFromFile(pathh + "RS Dying Sprite Sheet.png");
                    enemy3[i].sprite.setOrigin(341 / 11 / 2, 0);
                    enemy3[i].sprite_indicator[1] += 0.2;
                    if (enemy3[i].sprite_indicator[1] > 10.9)
                    {
                        enemy3[i].sprite_indicator[1] = 0.2;
                        enemy3[i].death_animation_done = 1;
                    }
                    enemy3[i].sprite.setTextureRect(IntRect(int(enemy3[i].sprite_indicator[1]) * 341 / 11, 0, 341 / 11, 39));


                }
                else
                {
                    enemy3[i].is_alive = 0;
                }
            }
        }
    }
    void draw(vector<Enemy3>& enemy3)
    {
        for (int i = 0; i < enemy3.size(); i++)
        {
            if (enemy3[i].is_alive == 1)
            {
                window.draw(enemy3[i].sprite);
            }
            enemy3[i].drawbullets(enemy3);
        }
    }
    void drawbullets(vector<Enemy3>& enemy3)
    {
        for (int i = 0; i < enemy3.size(); i++)
        {
            for (int x = 0; x < enemy3[i].bullet.size(); x++)
            {
                if (enemy3[i].bullet[x].second == LEFT)
                {
                    enemy3[i].bullet[x].first.move(-10, 0);
                    window.draw(enemy3[i].bullet[x].first);
                }
                else if (enemy3[i].bullet[x].second == RIGHT)
                {
                    window.draw(enemy3[i].bullet[x].first);
                    enemy3[i].bullet[x].first.move(10, 0);
                }
            }
        }
    }
    void Carrying_a_weapon(vector<Enemy3>& enemy3)
    {
        for (int i = 0; i < enemy3.size(); i++)
        {
            float distance = abs(enemy3[i].sprite.getPosition().x - player.lowerbodySprite.getPosition().x);
            if (distance <= 300 && !enemy3[i].stopped && !enemy3[i].isCarrying_a_weapon)
            {
                enemy3[i].check = 0;
                enemy3[i].velocity.x = 0;
                enemy3[i].texture.loadFromFile(pathh + "RS Equipping Sprite Sheet.png");
                enemy3[i].sprite.setOrigin(322 / 7 / 2, 0);
                //   animation(enemy1[i].sprite, 6.9, 322 / 7, 44, 0.002, 9);
                enemy3[i].sprite_indicator[1] += 0.28;
                if (enemy3[i].sprite_indicator[1] > 6.9)
                {
                    enemy3[i].sprite_indicator[1] = 0;
                    enemy3[i].isCarrying_a_weapon = 1;
                }
                enemy3[i].sprite.setTextureRect(IntRect(int(enemy3[i].sprite_indicator[1]) * 322 / 7, 0, 322 / 7, 44));
            }
        }
    }
    void shooting(vector<Enemy3>& enemy3)
    {
        for (int i = 0; i < enemy3.size(); i++)
        {
            enemy3[i].shoot_timer += 0.08;
            if (enemy3[i].shoot_timer > 1 || enemy3[i].shoot_timer == 0.05) 
            {
                Vector2f pl = enemy3[i].sprite.getPosition();
                RectangleShape rect(sf::Vector2f(20, 10));
                rect.setOrigin(-pl.x, -(pl.y + 50));
                enemy3[i].bullet.push_back({ rect ,enemy3[i].last_key });
                enemy3[i].shoot_timer = 0;
            }
        }
    }
    void movement(vector<Enemy3>& enemy3)
    {
            for (int i = 0; i < enemy3.size(); i++)
            {
                float distance = abs(enemy3[i].sprite.getPosition().x - player.lowerbodySprite.getPosition().x);
                if (distance > 300&&!enemy3[i].stopped)
                {
                    enemy3[i].shoot_timer = 1;
                    enemy3[i].isCarrying_a_weapon = 0;
                    enemy3[i].texture.loadFromFile(pathh + "RS Running Sprite Sheet.png");
                    EnemiAnimation(enemy3[i].sprite, 11.9, 312 / 12, 40, 0.017, enemy3[i].sprite_indicator[2]);
                    if (player.upperbodySprite.getPosition().x > enemy3[i].sprite.getPosition().x && player.health > 0)
                    {
                        enemy3[i].velocity.x = 3;
                        enemy3[i].sprite.setScale(-plScale, plScale);
                        enemy3[i].sprite.setOrigin(0, 0);
                        enemy3[i].last_key = RIGHT;
                    }
                    else if (player.upperbodySprite.getPosition().x < enemy3[i].sprite.getPosition().x && player.health > 0)
                    {
                        enemy3[i].velocity.x = -3;
                        enemy3[i].sprite.setScale(plScale, plScale);
                        enemy3[i].sprite.setOrigin(enemy3[i].sprite.getLocalBounds().width, 0);
                        enemy3[i].last_key = LEFT;
                    }
                    else
                    {
                        enemy3[i].velocity.x = 3;
                        enemy3[i].sprite.setScale(-plScale, plScale);
                        enemy3[i].sprite.setOrigin(0, 0);
                        enemy3[i].last_key = RIGHT;
                    }
                }
            }
    }
    void EnemyShootingAnimation(vector<Enemy3>& enemy3)
    {
        for (int i = 0; i < enemy3.size(); i++)
        {
            float distance = abs(enemy3[i].sprite.getPosition().x - player.lowerbodySprite.getPosition().x);
            if(distance <= 300 && !enemy3[i].stopped&& enemy3[i].isCarrying_a_weapon)
            {
                enemy3[i].shooting(enemy3);
                enemy3[i].check = 1;
                enemy3[i].velocity.x = 0;
                enemy3[i].texture.loadFromFile(pathh + "RS Shooting Sprite Sheet.png");
                EnemiAnimation(enemy3[i].sprite, 2.9, 183 / 3, 38, 0.008, enemy3[i].sprite_indicator[0]);
                if (player.upperbodySprite.getPosition().x > enemy3[i].sprite.getPosition().x)
                {
                    enemy3[i].sprite.setScale(-plScale, plScale);
                    enemy3[i].sprite.setOrigin(0, 0);
                    enemy3[i].last_key = RIGHT;
                }
                else
                {
                    enemy3[i].sprite.setScale(plScale, plScale);
                    enemy3[i].sprite.setOrigin(enemy3[i].sprite.getLocalBounds().width, 0);
                    enemy3[i].last_key = LEFT;
                }
            }
        }
    }
    void Gravity(vector<Enemy3>& enemy3)//update y position
    { 
        for (int i = 0; i < enemy3.size(); i++)
        {
            float x = enemy3[i].rec.getPosition().x;
            enemy3[i].rec.setPosition(x, 820);
        }
    }
}new_enemy;
vector<Enemy3>enemy3;
void new_enemy_setup()
{
    new_enemy.rec.setSize(Vector2f(100, 130));
    new_enemy.rec.setPosition(Vector2f(19650, 820));
    new_enemy.texture.loadFromFile(pathh + "RS Running Sprite Sheet.png");
    new_enemy.sprite.setTexture(new_enemy.texture);
    new_enemy.sprite.setScale(plScale, plScale);
    new_enemy.sprite.setTextureRect(IntRect(0, 0, 312 / 12, 40));
    new_enemy.sprite.setPosition(Vector2f(19650, 820));
}
void call()
{
    if (bgCounter == LEVEL_1_C_BG)
    {
        if (push_timer.getElapsedTime().asSeconds()*delay > 3)
        {
            delay += 0.1;
            enemy3.push_back(new_enemy);
            push_timer.restart();
        }
      /*  for (int i = 0; i < enemy3.size(); i++)
        {
            float distance = abs(enemy3[i].sprite.getPosition().x - player.lowerbodySprite.getPosition().x);
            if (distance > 300 && !enemy3[i].stopped)
            {
                enemy3[i].texture.loadFromFile(pathh + "RS Running Sprite Sheet.png");
            }
            else if (distance <= 300 && !enemy3[i].stopped && enemy3[i].isCarrying_a_weapon)
            {
                enemy3[i].texture.loadFromFile(pathh + "RS Shooting Sprite Sheet.png");
            }
            else if (distance <= 300 && !enemy3[i].stopped && !enemy3[i].isCarrying_a_weapon)
            {
                enemy3[i].texture.loadFromFile(pathh + "RS Equipping Sprite Sheet.png");
            }
      }*/
        new_enemy.movement(enemy3);
        new_enemy.Carrying_a_weapon(enemy3);
        new_enemy.EnemyShootingAnimation(enemy3);
        new_enemy.Damaged(enemy3);
        
        for (int i = 0; i < enemy3.size(); i++)
        {
            enemy3[i].rec.move(enemy3[i].velocity);
            if (enemy3[i].check)//to fix RS Equipping Sprite dimensions
            {
                enemy3[i].sprite.setPosition(enemy3[i].rec.getPosition());
            }
            else
            {
                //to fix RS Equipping Sprite dimensions
                if (player.upperbodySprite.getPosition().x > enemy3[i].sprite.getPosition().x)
                    enemy3[i].sprite.setPosition(enemy3[i].rec.getPosition().x - 60, enemy3[i].rec.getPosition().y - 22);
                else
                    enemy3[i].sprite.setPosition(enemy3[i].rec.getPosition().x - 58, enemy3[i].rec.getPosition().y - 22);
            }
        }
    }
}

//gravity
float gravity = 0.7;
bool canDoubleJump;

//background Textures&Sprites
Texture bgTexture[30];
Sprite bgSprite[30];

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

//lvl 1 A lamps animation && lvl 1 B && lvl 1 C torches
Texture lvl1LampTex[4], lvl1torchTex[4], lvl1_D_torchTex[2], Exitlamp1_D_Tex;
Sprite lvl1lamp[4], lvl1torch[4], lvl1_D_torch[2], Exitlamp1_D;

//powerups
RectangleShape powerups(Vector2f(50, 50));

int main()
{
    blood.setup(blood);
    window.setFramerateLimit(60);
    RS->setup(RS);
    enemy2->setup(enemy2);
    tank.setup(tank);
    hud.setup(hud);
    bgSetup();
    TS_Setups();
    player.Playersetup(player);
    timer.restart();
    Menu();
    return 0;
}
//DEFINITIONS

void Menu()
{
    while (window.isOpen())
    {
        mouse_pos();
        windowclose();
        if (pause_menu.is_paused)
        {
            if (!menu.options_screen.isOpen)
            {
                window_draw();
                pause_menu.draw();
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200 && !menu.options_screen.isOpen)
            {
                pause_menu.is_paused = false;
                escTimer.restart();
            }
            else if (pause_menu.selected == 1 && Keyboard::isKeyPressed(Keyboard::Enter))
                pause_menu.exit();
            else if (pause_menu.selected == 2 && Keyboard::isKeyPressed(Keyboard::Enter) && timer2.getElapsedTime().asMilliseconds() > 200 || menu.options_screen.isOpen)
            {
                timer2.restart();
                if (!menu.options_screen.isOpen)
                {
                    timer4.restart();
                    menu.options_screen.isOpen = true;
                }
                if (!menu.options_screen.music_slider.isOpen)
                    menu.options_screen.draw();
                if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
                {
                    menu.options_screen.isOpen = false;
                    escTimer.restart();
                }
                if (Keyboard::isKeyPressed(Keyboard::Up) && timer.getElapsedTime().asMilliseconds() > 200)
                {
                    timer.restart();
                    menu.options_screen.move_up();
                }
                if (Keyboard::isKeyPressed(Keyboard::Down) && timer.getElapsedTime().asMilliseconds() > 200)
                {
                    timer.restart();
                    menu.options_screen.move_down();
                }
                if (Keyboard::isKeyPressed(Keyboard::Enter) && menu.options_screen.selected == 1 && timer4.getElapsedTime().asMilliseconds() > 200 || menu.options_screen.music_slider.isOpen)
                {
                    timer4.restart();
                    menu.options_screen.music_slider.draw();
                    menu.options_screen.music_slider.isOpen = true;
                    if (Keyboard::isKeyPressed(Keyboard::Right) && timer.getElapsedTime().asMilliseconds() > 200)
                    {
                        timer.restart();
                        menu.options_screen.music_slider.move_left();
                    }
                    if (Keyboard::isKeyPressed(Keyboard::Left) && timer.getElapsedTime().asMilliseconds() > 200)
                    {
                        timer.restart();
                        menu.options_screen.music_slider.move_right();
                    }
                    if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
                    {
                        escTimer.restart();
                        menu.options_screen.music_slider.isOpen = false;
                    }
                }
            }
            else if (pause_menu.selected == 3 && Keyboard::isKeyPressed(Keyboard::Enter))
                window.close();
            else if (Keyboard::isKeyPressed(Keyboard::Up) && timer.getElapsedTime().asMilliseconds() > 200)
            {
                timer.restart();
                pause_menu.move_up();
            }
            else if (Keyboard::isKeyPressed(Keyboard::Down) && timer.getElapsedTime().asMilliseconds() > 200)
            {
                pause_menu.move_down();
                timer.restart();
            }
        }
        else {
            if (menu.game_started)
            {
                TS_PESpr.setColor(Color(255, 255, 255, AlphaPE));
                if (AlphaPE > 0)
                    AlphaPE -= 0.05;
                windowfunction();
            }
            else
            {
                if (!menu.start_selected && !menu.options_screen.isOpen)
                    menu.main_screen_draw();
                if (Keyboard::isKeyPressed(Keyboard::Enter) || menu.ts_escaped)
                {

                    if (!menu.ts_escaped)
                    {
                        timer2.restart();
                        MenuClick.play();
                        menu.ts_escaped = true;

                    }
                    else
                    {

                        if (((menu.selected == START && Keyboard::isKeyPressed(Keyboard::Enter)) && timer2.getElapsedTime().asMilliseconds() > 200) || menu.start_selected)
                        {
                            hud.hud_time.restart();
                            timer2.restart();
                            menu.start_screen.draw();
                            if (!menu.start_selected)
                            {
                                MenuClick.play();
                                menu.start_selected = true;
                                timer.restart();

                            }
                            if (menu.start_screen.selected == 1 && Keyboard::isKeyPressed(Keyboard::Enter) && timer.getElapsedTime().asMilliseconds() > 200)
                            {
                                menu.game_started = true;
                                timer.restart();
                                windowfunction();

                            }
                            if (Keyboard::isKeyPressed(Keyboard::Up) && timer.getElapsedTime().asMilliseconds() > 200)
                            {
                                menu.start_screen.move_up();
                                timer.restart();
                            }
                            if (Keyboard::isKeyPressed(Keyboard::Down) && timer.getElapsedTime().asMilliseconds() > 200)
                            {
                                menu.start_screen.move_down();
                                timer.restart();
                            }
                            if (Keyboard::isKeyPressed(Keyboard::Escape) && timer.getElapsedTime().asMilliseconds() > 200)
                            {
                                menu.start_selected = false;
                                timer.restart();
                            }

                        }
                        else if (menu.selected == OPTIONS && Keyboard::isKeyPressed(Keyboard::Enter) && timer2.getElapsedTime().asMilliseconds() > 200 || menu.options_screen.isOpen)
                        {
                            timer2.restart();
                            if (!menu.options_screen.music_slider.isOpen)
                                menu.options_screen.draw("menu");
                            if (!menu.options_screen.isOpen)
                            {
                                menu.options_screen.isOpen = true;
                                timer4.restart();
                            }
                            if (Keyboard::isKeyPressed(Keyboard::Enter) && timer4.getElapsedTime().asMilliseconds() > 200 || menu.options_screen.music_slider.isOpen)
                            {
                                timer4.restart();
                                menu.options_screen.music_slider.draw("main");
                                menu.options_screen.music_slider.isOpen = true;

                                if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
                                {
                                    escTimer.restart();
                                    menu.options_screen.music_slider.isOpen = false;
                                }

                                if (Keyboard::isKeyPressed(Keyboard::Right) && timer.getElapsedTime().asMilliseconds() > 200)
                                {
                                    timer.restart();
                                    menu.options_screen.music_slider.move_left();
                                }
                                if (Keyboard::isKeyPressed(Keyboard::Left) && timer.getElapsedTime().asMilliseconds() > 200)
                                {
                                    timer.restart();
                                    menu.options_screen.music_slider.move_right();
                                }


                            }
                            if (Keyboard::isKeyPressed(Keyboard::Down) && timer.getElapsedTime().asMilliseconds() > 200)
                            {
                                timer.restart();
                                menu.options_screen.move_down();
                            }
                            if (Keyboard::isKeyPressed(Keyboard::Up) && timer.getElapsedTime().asMilliseconds() > 200)
                            {
                                timer.restart();
                                menu.options_screen.move_up();
                            }
                            if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
                            {
                                escTimer.restart();
                                menu.options_screen.exit();
                            }
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
}
void bgSetup()
{

    new_enemy_setup();

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
    lvl1LampTex[0].loadFromFile(pathh + "Level 1-A Lamp 1.png");
    lvl1lamp[0].setTexture(lvl1LampTex[0]);
    lvl1lamp[0].setTextureRect(IntRect(0, 0, 345, 657));
    lvl1lamp[0].setPosition(-370 + 1943, 149 - 53);
    lvl1LampTex[1].loadFromFile(pathh + "Level 1-A Lamp 2.png");
    lvl1lamp[1].setTexture(lvl1LampTex[1]);
    lvl1lamp[1].setTextureRect(IntRect(0, 0, 2760 / 8, 657));
    lvl1lamp[1].setPosition(-370 + 3085, 149 - 53);
    lvl1LampTex[2].loadFromFile(pathh + "Level 1-A Lamp 3.png");
    lvl1lamp[2].setTexture(lvl1LampTex[2]);
    lvl1lamp[2].setTextureRect(IntRect(0, 0, 2760 / 8, 657));
    lvl1lamp[2].setPosition(-370 + 4278, 149 - 53);
    lvl1LampTex[3].loadFromFile(pathh + "Level 1-A Lamp 4.png");
    lvl1lamp[3].setTexture(lvl1LampTex[3]);
    lvl1lamp[3].setTextureRect(IntRect(0, 0, 2760 / 8, 657));
    lvl1lamp[3].setPosition(-370 + 5470, 149 - 53);

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

    // Lvl 1 - B Torches

    lvl1torchTex[0].loadFromFile(pathh + "Level 1-B Torches 1.png");
    lvl1torch[0].setTexture(lvl1torchTex[0]);
    lvl1torch[0].setPosition(10000 + 345, -50 + 559);
    lvl1torchTex[1].loadFromFile(pathh + "Level 1-B Torches 2.png");
    lvl1torch[1].setTexture(lvl1torchTex[1]);
    lvl1torch[1].setPosition(10000 + 1090, -50 + 559);
    lvl1torchTex[2].loadFromFile(pathh + "Level 1-B Torches 3.png");
    lvl1torch[2].setTexture(lvl1torchTex[2]);
    lvl1torch[2].setPosition(10000 + 3028, -50 + 559);
    lvl1torchTex[3].loadFromFile(pathh + "Level 1-B Torches 4.png");
    lvl1torch[3].setTexture(lvl1torchTex[3]);
    lvl1torch[3].setPosition(10000 + 3774, -50 + 559);

    // LEVEL 1 B --> 2 SET UP

    bgTexture[2].loadFromFile(pathh + "Level 1-B-2 BG.png");
    bgSprite[2].setTexture(bgTexture[2]);
    bgSprite[2].setPosition(14771, -940);

    // Exit-lamp 1 - B

    ExitlampTex.loadFromFile(pathh + "Level 1-B Exit Lamps.png");
    Exitlamp.setTexture(ExitlampTex);
    Exitlamp.setPosition(14771 + 2176, -940 + 471);


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

    //fire troches lv1 --> C

    FireTrochestex.loadFromFile(pathh + "Level 1-C Fire Torches.png");
    FireTroches.setTexture(FireTrochestex);
    FireTroches.setTextureRect(IntRect(0, 0, 13120 / 8, 1192));
    FireTroches.setPosition(18000, 0);

    //lvl 1 --> C FG

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

    // Lvl 1 - D Fire Torches

    lvl1_D_torchTex[0].loadFromFile(pathh + "Level 1-D Torch 1_2.png");
    lvl1_D_torch[0].setTexture(lvl1_D_torchTex[0]);
    lvl1_D_torch[0].setPosition(20000 + 1020, 563);
    lvl1_D_torchTex[1].loadFromFile(pathh + "Level 1-D Torch 3_4_5.png");
    lvl1_D_torch[1].setTexture(lvl1_D_torchTex[1]);
    lvl1_D_torch[1].setPosition(22073, 391);

    // Lvl 1 - D Exit Lamp

    Exitlamp1_D_Tex.loadFromFile(pathh + "Level 1-D Exit Lamps.png");
    Exitlamp1_D.setTexture(Exitlamp1_D_Tex);
    Exitlamp1_D.setPosition(24212, 568);

}
void windowfunction()
{
    //delta time
    dt = clock_pl.getElapsedTime().asMicroseconds();
    dt /= 750;
    clock_pl.restart();

    if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
    {
        pause_menu.is_paused = true;
        escTimer.restart();
    }

    if (Keyboard::isKeyPressed(Keyboard::Q))
    {
        player.gun = RIFLE;
    }
    else if (Keyboard::isKeyPressed(Keyboard::W))
    {
        player.gun = PISTOL;
    }

    blood.move(blood);
    //map shortcut
    if (Keyboard::isKeyPressed(sf::Keyboard::T))
        player.upperbodySprite.setPosition(20000, 800);

    cameraView();
    RS->movement(RS);
    RS->Gravity(RS);
    if(enemy3.size()>0)
        enemy3[0].Gravity(enemy3);
    RS->Damaged(RS);
    playerDamageFromEnemy1();

    if (enemy3.size() < 30)
    {
        call();
    }
    if (player.health > 0)
        plmovement(player.lowerbodySprite, 11.9, 408 / 12, 41, 0.004, 2);

    tank.tankState(tank);


    BGanimation();
    windowclose();
    window.clear();
    window_draw();
    window.setView(view);
    hud.positions(hud);
    hud.time_calculation();
	hud.ammo_display();
    transition_pos_check();
}
void BGanimation()
{
    animation(lvl1lamp[0], 8, 345, 657, 0.01, 22);
    animation(lvl1lamp[1], 8, 345, 657, 0.01, 23);
    animation(lvl1lamp[2], 8, 345, 657, 0.01, 24);
    animation(lvl1lamp[3], 8, 345, 657, 0.01, 25);
    animation(lvl1torch[0], 8, 1304.0 / 8, 303, 0.01, 26);
    animation(lvl1torch[1], 8, 1304.0 / 8, 303, 0.01, 27);
    animation(lvl1torch[2], 8, 1304.0 / 8, 303, 0.01, 28);
    animation(lvl1torch[3], 8, 1304.0 / 8, 303, 0.01, 29);
    animation(Exitlamp, 6, 1092.0 / 6, 99, 0.002, 30);
    animation(FireTroches, 8, 13120.0 / 8, 1192, 0.01, 31);
    animation(lvl1_D_torch[0], 8, 3096.0 / 8, 168, 0.01, 32);
    animation(lvl1_D_torch[1], 8, 653, 401, 0.01, 33);
    animation(Exitlamp1_D, 6, 1092 / 6.0, 98, 0.002, 34);

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
            full_time_played += hud.hud_time.getElapsedTime().asSeconds();
            hud.hud_time.restart();
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
            full_time_played += hud.hud_time.getElapsedTime().asSeconds();
            hud.hud_time.restart();
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
            full_time_played += hud.hud_time.getElapsedTime().asSeconds();
            hud.hud_time.restart();
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
        if (player.gun == PISTOL)
            move_with_animation(player.upperbodySprite, maxframe, x, y, delay, index);
        else if (player.gun == RIFLE)
            move_with_animation(player.upperbodySprite, 11.9, 528 / 11, 29, delay, 32);

    }

    //move player
    if (player.gun == PISTOL)
    {
        s.move(player.Velocity);
        player.upperbodySprite.move(player.Velocity);
    }
    else if (player.gun == RIFLE)//with rifle the speed is slower than pistol
    {
        s.move(player.Velocity.x * 0.5, player.Velocity.y);
        player.upperbodySprite.move(player.Velocity.x * 0.5, player.Velocity.y);
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
                    //liser.shooting(liser);
                    pistol.shooting(pistol);
                else if (player.gun == RIFLE)
                    rifle.shooting(rifle);

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
                    animation(player.upperbodySprite, 11.9, 528 / 12, 29, 0.0001, 32);
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
                    //   liser.shooting(liser);
                    pistol.shooting(pistol);
                else if (player.gun == RIFLE)
                    rifle.shooting(rifle);

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
                    animation(player.upperbodySprite, 11.9, 528 / 12, 29, 0.0001, 32);
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
                    //  liser.shooting(liser);
                    pistol.shooting(pistol);
                    player.upperbodyTex.loadFromFile(pathh + "Shooting - Standing (Pistol) Sprite Sheet Upper Body.png");
                    animation(player.upperbodySprite, 9.9, 520 / 10, 41, pistolshooting_delay, 10);
                }
                else if (player.gun == RIFLE && rifle.ammo!=0)
                {
                    cout << rifle.ammo << "   ";
                    rifle.shooting(rifle);
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

void window_draw()
{
    for (int i = 0; i < 5; i++)
        window.draw(bgSprite[i]);

    //draw BG animations
    for (int i = 0; i < 4; i++)
    {
        window.draw(lvl1lamp[i]);
        window.draw(lvl1torch[i]);
    }
    window.draw(Exitlamp);
    window.draw(FireTroches);
    window.draw(lvl1_D_torch[0]);
    window.draw(lvl1_D_torch[1]);
    window.draw(Exitlamp1_D);

    //window.draw(player.rec);
    RS->draw(RS);
   if(enemy3.size())
        enemy3[0].draw(enemy3);
    
    // enemy3[0].draw(enemy3[0]);
    window.draw(ground[12]);
    if (player.live || player.isdead)
    {
        window.draw(player.lowerbodySprite);
        if (!player.one_sprite_needed)
            window.draw(player.upperbodySprite);
    }
    blood.call(blood);
    pistol.drawpistol(window, pistol);//draw pistol bullets
    rifle.drawrifle(rifle); //draw rifle bullets
    liser.drawliser(liser);
    for (int i = 0; i < 4; i++)
    {
        if (i == 1)continue;
        window.draw(Lvl1FG[i]);
    }
    // window.draw(ground[0]);
    hud.draw(hud);
    //window.draw(powerups);
    //enemy2->draw(enemy2);
    tank.draw(tank);
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
    else if (player.gun == RIFLE)
    {
        player.upperbodyTex.loadFromFile(pathh + "Shooting - Standing (Rifle) Sprite Sheet.png");
        animation(player.upperbodySprite, 3.9, 240 / 4, 27, rifleshooting_delay, 10);
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
            liser.shooting(liser);
            player.lowerbodyTex.loadFromFile(pathh + "Shooting - Crouching (Pistol) Sprite Sheet.png");
            animation(player.lowerbodySprite, 9.9, 520 / 10, 29, pistolshooting_delay, 14);
        }
        else if (player.gun == RIFLE)
        {
            pistol.shooting(pistol);
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
        else if (player.gun == RIFLE && rifle.ammo !=0)
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
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j < RS[i].bullet.size(); j++)
            {
                if ((player.lowerbodySprite.getGlobalBounds().intersects(RS[i].bullet[j].first.getGlobalBounds()) || player.upperbodySprite.getGlobalBounds().intersects(RS[i].bullet[j].first.getGlobalBounds())) && RS[i].bullet[j].second != 0)
                {
                    player.health -= RS[i].damage;

                    if (player.health % 20 == 0)
                        hud.hp_index++;

                    player.is_getting_damaged = 1;
                    RS[i].bullet[j].second = 0;
                }
            }
        }
        for (int i = 0; i < enemy3.size(); i++)
        {
            for (int j = 0; j < enemy3[i].bullet.size(); j++)
            {
                if ((player.lowerbodySprite.getGlobalBounds().intersects(enemy3[i].bullet[j].first.getGlobalBounds()) || player.upperbodySprite.getGlobalBounds().intersects(enemy3[i].bullet[j].first.getGlobalBounds())) && enemy3[i].bullet[j].second != 0)
                {
                    player.health -= enemy3[i].damage;

                    if (player.health % 20 == 0)
                        hud.hp_index++;

                    player.is_getting_damaged = 1;
                    enemy3[i].bullet[j].second = 0;
                }
            }
        }
    }
    if (player.health <= 0)
    {
        playerDeathAnimation();
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
void TS_Setups()
{
    //Sets up the Title Screen & Main Menu Background, Music and Fire Sound Effect.

    TS_BGSpr.setPosition(0.f, 0.f);
    TS_BGTex.loadFromFile(pathh + "TS BG.png");
    TS_BGSpr.setTexture(TS_BGTex);
    TS_BGTheme.openFromFile(pathh + "Title Screen _ Main Menu Theme.wav");
    TS_BGTheme.play();
    TS_BGTheme.setVolume(100.f);
    TS_BGTheme.setLoop(true);
    TS_BGFireFX.openFromFile(pathh + "Title Screen _ Main Menu Fire Sound.wav");
    TS_BGFireFX.play();
    TS_BGFireFX.setVolume(30.f);//30;
    TS_BGFireFX.setLoop(true);


    //Sets up the Title Screen & Main Menu Torches & Gate Animation.
    TS_TandGSpr.setPosition(659.f, 208.f);
    TS_TandGTex.loadFromFile(pathh + "TS Torches _ Gate Sprite Sheet.png");
    TS_TandGSpr.setTexture(TS_TandGTex);



    TS_LSpr.setPosition(722.f, 0.f);
    TS_LTex.loadFromFile(pathh + "TS Lamp Sprite Sheet.png");
    TS_LSpr.setTexture(TS_LTex);


    TS_VSpr.setPosition(0.f, 0.f);
    TS_VTex.loadFromFile(pathh + "Vignette.png");
    TS_VSpr.setTexture(TS_VTex);


    TS_LogoSpr.setPosition(269.f, 465.f);
    TS_LogoTex.loadFromFile(pathh + "TS 1 Sprite Sheet.png");
    TS_LogoSpr.setTexture(TS_LogoTex);

    TS_PESpr.setPosition(612.f, 874.f);
    TS_PETex.loadFromFile(pathh + "TS Press Enter Sprite Sheet.png");
    TS_PESpr.setTexture(TS_PETex);


    TS_buttonsSpr.setPosition(769.f, 650.f);
    TS_buttonsTex.loadFromFile(pathh + "Main Menu Sprite Sheet.png");
    TS_buttonsSpr.setTexture(TS_buttonsTex);
    TS_buttonsSpr.setTextureRect(IntRect(0, 0, 360, 315));

    TS_SSSpr.setPosition(769.f, 650.f);
    TS_SSTex.loadFromFile(pathh + "Main Menu 2 Sprite Sheet.png");
    TS_SSSpr.setTexture(TS_SSTex);
    TS_SSSpr.setTextureRect(IntRect(0, 0, 400, 315));

    TS_OlSpr.setPosition(0.f, 0.f);
    TS_OlTex.loadFromFile(pathh + "Dark Overlay.png");
    TS_OlSpr.setTexture(TS_OlTex);
    TS_OlSpr.setScale(3, 3);

    PMSpr.setPosition(666, 364);
    PMTex.loadFromFile(pathh + "Pause Screen Sprite Sheet.png");
    PMSpr.setTexture(PMTex);
    PMSpr.setScale(0.7, 0.7);
    PMSpr.setTextureRect(IntRect(0, 0, 588, 352));

    OptionsSpr.setPosition(539.f, 400.f);
    OptionsTex.loadFromFile(pathh + "Sound Sprite Sheet.png");
    OptionsSpr.setTexture(OptionsTex);
    OptionsSpr.setTextureRect(IntRect(0, 0, 843, 441));

    MusicControlSpr.setPosition(580, 289);
    MusicControlTex.loadFromFile(pathh + "Sound Slider Sprite Sheet.png");
    MusicControlSpr.setTexture(MusicControlTex);
    MusicControlSpr.setTextureRect(IntRect(0, 0, 755, 240));

    MenuClickB.loadFromFile(pathh + "Menu Click FX.wav");
    MenuClick.setBuffer(MenuClickB);

    MenuScrollB.loadFromFile(pathh + "Menu Scrolling FX.wav");
    MenuScroll.setBuffer(MenuScrollB);
    MenuScroll.setVolume(100);//100;
}
void mouse_pos()
{
    if (Mouse::isButtonPressed(Mouse::Left))
    {
        Vector2i mousePos = Mouse::getPosition(window);
        cout << mousePos.x << ' ' << mousePos.y << '\n';
    }
}
///
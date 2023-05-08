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

//player.gun 
#define PISTOL 1
#define RIFLE 2
#define LISER 3
#define FLAME 4
#define BIOCHEMICAL 5

//power ups
#define ENEMY_PLAYER_RANGE 700
#define ENEMY_PLAYER_SHOOTING_RANGE 400
#define HEALTH_KIT 1
#define HEALTH_POTION 2
#define SPEED_POTION 3
#define DAMAGE_POTION 4
#define RIFLE_AMMO 5
#define LISER_AMMO 6
#define FLAME_AMMO 7
#define BIOCHEMICAL_AMMO 8

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
Texture TS_BGTex, TS_TandGTex, TS_LTex, TS_VTex, TS_LogoTex, TS_PETex,
TS_buttonsTex, TS_SSTex, TS_OlTex, OptionsTex, MusicControlTex, PMTex,
RWTexRun, RWTexAttack, RWTexDeath, PTexSCPL, PTexIPL, PTexSCRL, PTexICPL, PTexICRL, PTexJRU, PTexJPL, PTexDL,
PTexMU, PTexML, PTexRPU, PTexRPL, PTexRRU, PTexRRL, PTexSSPU, PTexSSRU, PTexIPSL, PTexIPSU, PTexIRL, PTexIRU, PTexJPU,
health_kit_tex, health_potion_tex, speed_potion_tex, damage_potion_tex, rifle_ammo_tex, deathScreenBGTex,
deathScreenFGTex, deathScreenFGTex2, NewGameTex;


Sprite TS_BGSpr, TS_TandGSpr, TS_LSpr, TS_VSpr, TS_LogoSpr, TS_PESpr, TS_buttonsSpr, TS_SSSpr, NewGameSpr,
TS_OlSpr, OptionsSpr, MusicControlSpr, PMSpr, deathScreenBGSpr, deathScreenFGSpr, deathScreenFGSpr2;


Music TS_BGTheme;
Music TS_BGFireFX;


SoundBuffer MenuClickB, MenuScrollB, DeathScreenFXB, GamePlayB, MenuReturnB;
Sound MenuClick, MenuScroll, DeathScreenFX, GamePlayTheme, MenuReturn;


Clock timer, timer2, timer4, escTimer;


View view(Vector2f(0, 0), Vector2f(1920, 1080));


float TS_TandGCnt = 0, TS_LCnt = 0, TS_LogoCnt = 0, TS_PECnt = 0, AlphaPE = 255;
int TS_ButtonsCnt = 0, TSS_ButtonsCnt = 0, OptionsSprCnt = 0, deathScreenFG2cnt = 0;

Font nameFont;
Text nameDis;
// Moved from down to here because

int bgCounter = 0, leftEnd, rightEnd;//indicator for current map, start of map, end of map
bool  isMoved = false, ismoved2 = 0, ismoved3 = 0;//flags for clearing the previous map
Clock blackscreenTimer;

int full_time_played = 0;

// DECLRATIONS
char key_code(sf::Keyboard::Key);
void EnemiAnimation(Sprite& s, float maxframe, float x, float y, float delay, float& indecator);
void texture_setup();
void window_draw();
void mouse_pos();
void TS_Setups();
void jumpingAnimation(float);
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
    float plcounter = 0, pl_upper_melee_ctr = 0, pl_lower_melee_ctr = 0, pl_death_ctr = 0;
    Texture upperbodyTex, lowerbodyTex;
    Sprite upperbodySprite, lowerbodySprite;

    RectangleShape rec;//make rectangle to better collision
    Vector2f Velocity = { 0,0 }, last_enemy_pos;
    float health = 100;

    SoundBuffer playerdeath_B;
    Sound playergeath;

    int score = 0, speed_boost = 1;
    int gun = PISTOL, kill_count = 0;
    bool canshoot = 0;
    bool crouch = 0;
    bool is_shooting = 0;
    bool one_sprite_needed = 0;
    int last_key = 0;
    bool live = 1, is_getting_damaged = 0;
    Clock damage_timer;
    bool isdead = 0;
    bool holding_knife = 0;
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

        player.playerdeath_B.loadFromFile("Player Death.wav");
        player.playergeath.setBuffer(player.playerdeath_B);
        player.playergeath.setVolume(40);
    }
    void melee_animation(Player& player)
    {
        player.upperbodySprite.setOrigin(48 / 2.0, 15);
        player.upperbodySprite.setTexture(PTexMU);
        EnemiAnimation(player.upperbodySprite, 5, 48, 53, 0.02 / 3, player.pl_upper_melee_ctr);

        player.lowerbodySprite.setOrigin(48 / 2.0, 15);
        player.lowerbodySprite.setTexture(PTexML);
        EnemiAnimation(player.lowerbodySprite, 5, 48, 53, 0.02 / 3, player.pl_lower_melee_ctr);
    }
}player;



// shooting <Rifle>
struct Rifle
{
    float  damage = 0.9;
    float shoot_timer = 0;
    vector<pair<RectangleShape, pair< int, double>>>rects;
    vector<Sprite>sprite;
    int range = 1200, ammo = 200;
    Texture tex;
    SoundBuffer sound_B, gunvoice_B;
    Sound sound, gunvoice;
    void setup(Rifle& rifle)
    {
        rifle.tex.loadFromFile(pathh + "Rifle Ammo.png");
        rifle.sound_B.loadFromFile("Rifle Bullet.wav");
        rifle.sound.setBuffer(rifle.sound_B);
        rifle.sound.setVolume(40);
        rifle.gunvoice_B.loadFromFile("Rifle Voice.wav");
        rifle.gunvoice.setBuffer(rifle.gunvoice_B);
        rifle.gunvoice.setVolume(40);
    }
    void update_bUllets_distance(Rifle& rifle, int i)
    {
        rifle.rects[i].second.second += 14;
        if (rifle.rects[i].second.second > rifle.range)
            rifle.rects[i].second.first = 0;
    }
    void shooting(Rifle& rifle)
    {
        if (rifle.ammo > 0)
        {
            rifle.shoot_timer += 0.08;
            if (rifle.shoot_timer > 1 || rifle.shoot_timer == 0.03)
            {
                rifle.sound.play();
                Vector2f pl = player.lowerbodySprite.getPosition();
                RectangleShape rect(sf::Vector2f(10, 10));
                rect.setOrigin(-pl.x, -(pl.y + 50));
                rifle.rects.push_back({ rect ,{player.last_key,0} });
                Sprite spr;
                spr.setTexture(rifle.tex);
                spr.setOrigin(-pl.x, -(pl.y + 50));
                rifle.sprite.push_back(spr);
                rifle.shoot_timer = 0;
                rifle.ammo--;
            }

        }
    }
    void drawrifle(Rifle& rifle)
    {
        // Only move the rect here
        for (int x = 0; x < rifle.rects.size(); x++)
        {
            if (rifle.rects[x].second.first == LEFT)
            {
                rifle.rects[x].first.move(-10, 0);
                rifle.sprite[x].move(-10, 0);
                window.draw(rifle.sprite[x]);
            }
            else if (rifle.rects[x].second.first == RIGHT)
            {
                rifle.rects[x].first.move(10, 0);
                rifle.sprite[x].move(10, 0);
                window.draw(rifle.sprite[x]);
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
    Clock shooting_timer;
    SoundBuffer soundb;
    Sound sound;
    void setup(Liser& liser)
    {
        //liser setup
        liser.soundb.loadFromFile("Laser.wav");
        liser.sound.setBuffer(liser.soundb);
        liser.sound.setVolume(40);
    }
    void shooting(Liser& liser)
    {
        if (liser.shooting_timer.getElapsedTime().asSeconds() > 6)
        {
            liser.shooting_timer.restart();
        }
        liser.sound.play();
        liser.damage = 0.001 * liser.shooting_timer.getElapsedTime().asMilliseconds();
        Vector2f pl = player.lowerbodySprite.getPosition();
        RectangleShape rect(sf::Vector2f(100, 2 * liser.shooting_timer.getElapsedTime().asSeconds() + 5));
        rect.setOrigin(-pl.x, -(pl.y + 50));
        rect.setFillColor(Color::Red);
        liser.rects.push_back({ rect ,player.last_key });
        if (liser.shooting_timer.getElapsedTime().asSeconds() > 7)
        {
            liser.shooting_timer.restart();
        }
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

// shooting <pistol>
struct Pistol
{
    float  damage = 1.1;
    float shoot_timer = 0;
    SoundBuffer soundb;
    Sound sound;

    Texture tex;
    vector<Sprite>sprite;
    vector<pair<RectangleShape, pair<int, double>>>rects;// bullets //checker //position.x

    int range = 1200;
    void setup(Pistol& pistol)
    {
        pistol.tex.loadFromFile(pathh + "Handgun Ammo.png");
        pistol.soundb.loadFromFile("Revolver Bullet.wav");
        pistol.sound.setBuffer(soundb);
        pistol.sound.setVolume(40);

    }
    void update_bUllets_distance(Pistol& pistol, int i)
    {
        pistol.rects[i].second.second += 10;
        if (pistol.rects[i].second.second > pistol.range)
            pistol.rects[i].second.first = 0;
    }
    void shooting(Pistol& pistol)
    {
        pistol.shoot_timer += 0.03;
        if (pistol.shoot_timer > 1 || pistol.shoot_timer == 0.03)
        {
            pistol.sound.play();
            Vector2f pl = player.lowerbodySprite.getPosition();
            RectangleShape rect(sf::Vector2f(10, 10));
            rect.setOrigin(-pl.x, -(pl.y + 50));
            Sprite spr;
            spr.setTexture(pistol.tex);
            spr.setOrigin(-pl.x, -(pl.y + 50));
            pistol.rects.push_back({ rect ,{player.last_key,0} });
            pistol.sprite.push_back(spr);
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
                pistol.sprite[x].move(-10, 0);
                window.draw(pistol.sprite[x]);

            }
            else if (pistol.rects[x].second.first == RIGHT)
            {
                pistol.rects[x].first.move(10, 0);
                pistol.sprite[x].move(10, 0);
                window.draw(pistol.sprite[x]);
            }
            pistol.update_bUllets_distance(pistol, x);
        }
    }
} pistol;

//FLAME
struct Flame
{
    float damage = 0.001;
    float animation_indeator = 0;
    Texture tex;
    Sprite sprite;
    //825 *37
    void shooting(Flame& flame)
    {

    }

    void setup(Flame& flame)
    {
        flame.tex.loadFromFile("Flame Ammo Sprite Sheet.png");
        flame.sprite.setTexture(flame.tex);
    }
}flame;


//biochemical
struct Biochemical
{

}Biochemical;


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

struct NewGameScreen
{
    bool is_open = false;
    string name;


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
        nameDis.setString(name);

        window.draw(TS_BGSpr);
        window.draw(TS_TandGSpr);
        window.draw(TS_LSpr);
        window.draw(TS_OlSpr);
        window.draw(NewGameSpr);
        window.draw(nameDis);
        window.display();
    }


}NewGameScreen;
struct DeathScreen //Contains the Death Screen Menu Elements.
{
    int selected = 1;
    bool is_active = 0;


    void deathScreenSetup() //Sets up the death screen
    {
        deathScreenBGTex.loadFromFile("BG.png");
        deathScreenBGSpr.setTexture(deathScreenBGTex);
        deathScreenBGSpr.setPosition(0.f, 0.f);
        deathScreenFGTex.loadFromFile("You Died 1.png");
        deathScreenFGSpr.setTexture(deathScreenFGTex);
        deathScreenFGSpr.setPosition(0.f, 0.f);
        deathScreenFGTex2.loadFromFile("You Died Sprite Sheet 2.png");
        deathScreenFGSpr2.setTexture(deathScreenFGTex2);
        deathScreenFGSpr2.setPosition(0.f, 0.f);
        DeathScreenFXB.loadFromFile("Dark Souls Death FX.wav");
        DeathScreenFX.setBuffer(DeathScreenFXB);
    }
    void move_up()
    {
        selected = (selected == 1 ? 2 : 1);
        deathScreenFG2cnt = selected - 1;
        MenuScroll.play();
    }
    void move_down()
    {
        selected = (selected == 1 ? 2 : 1);
        deathScreenFG2cnt = selected - 1;
        MenuScroll.play();
    }
    void main_screen_draw()
    {
        window.clear();
        window_draw();
        deathScreenFGSpr2.setPosition(view.getCenter().x - (980 * view.getSize().x / 1920.0), view.getCenter().y - (650 * view.getSize().y / 1080.0));
        deathScreenFGSpr2.setTextureRect(IntRect(deathScreenFG2cnt * 1920, 0, 1920, 1080));
        TS_OlSpr.setScale(3 * view.getSize().x / 1920.0, 3 * view.getSize().y / 1080.0);
        TS_OlSpr.setPosition(view.getCenter().x - (1000 * view.getSize().x / 1920), view.getCenter().y - (600 * view.getSize().y / 1080.0));

        window.draw(TS_OlSpr);
        window.draw(deathScreenFGSpr2);
        window.display();
    }
} DeathScreen;

struct MusicSoundControl
{
    int selected = 1, soundlevel = 100;
    bool isOpen = false;


    void move_right()
    {
        selected = (selected + 1 <= 7 ? selected + 1 : 7);
        soundlevel = (100 - (selected - 1) * 15 < 11 ? 0 : 100 - (selected - 1) * 15);
        TS_BGTheme.setVolume(soundlevel);
        GamePlayTheme.setVolume(soundlevel);
        MenuScroll.play();
    }

    void move_left()
    {
        selected = (selected - 1 == 0 ? 1 : selected - 1);
        soundlevel = (100 - (selected - 1) * 15 < 11 ? 0 : 100 - (selected - 1) * 15);
        TS_BGTheme.setVolume(soundlevel);
        GamePlayTheme.setVolume(soundlevel);
        MenuScroll.play();
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
        MenuScroll.play();
    }
    void move_up()
    {
        selected = (selected == 1 ? 3 : selected - 1);
        MenuScroll.play();
    }
    void move_down()
    {
        selected = (selected == 3 ? 1 : selected + 1);
        MenuScroll.play();
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
        MenuScroll.play();
    }

    void move_down()
    {
        selected = (selected == 1 ? 2 : 1);
        OptionsSprCnt = selected - 1;
        MenuScroll.play();
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
    Clock score_increase_timer;
    void setup(HUD& hud) {
        hud.HP_bar_tex.loadFromFile(pathh + "Health Bar.png");
        hud.HP_bar_sprite.setTexture(hud.HP_bar_tex);
        hud.HP_bar_sprite.setScale(3.455, 3.455);
        hud.HP_bar_sprite.setTextureRect(IntRect(hp_index * (390 / 6), 0, 390 / 6, 11));

        hud.Mug_shot_tex.loadFromFile(pathh + "Mugshot.png");
        hud.Mug_shot_sprite.setTexture(hud.Mug_shot_tex);
        hud.Mug_shot_sprite.setScale(3.5, 3.5);

        hud.lives_tex.loadFromFile(pathh + "Lives.png");
        hud.lives_sprite.setTexture(hud.lives_tex);

        hud.infinity_tex.loadFromFile(pathh + "Infinity.png");
        hud.infinity_sprite.setTexture(hud.infinity_tex);

        hud.weapons_holder_tex.loadFromFile(pathh + "Weapons Holder.png");
        hud.weapons_holder_sprite.setTexture(hud.weapons_holder_tex);
        hud.weapons_holder_sprite.setScale(2.7, 2.7);

        hud.lives_number_tex.loadFromFile(pathh + "Numbers Lives.png");
        hud.lives_number_1_sprite.setTexture(hud.lives_number_tex);
        hud.lives_number_1_sprite.setTextureRect(IntRect(lives_num1_index * (290 / 10), 0, 290 / 10, 29));
        hud.lives_number_2_sprite.setTexture(hud.lives_number_tex);
        hud.lives_number_2_sprite.setTextureRect(IntRect(lives_num2_index * (290 / 10), 0, 290 / 10, 29));

        hud.time_num_tex.loadFromFile(pathh + "Numbers.png");
        hud.time_num1_sprite.setTexture(hud.time_num_tex);
        hud.time_num1_sprite.setTextureRect(IntRect(time_num1_index * (550 / 10), 0, 550 / 10, 55));
        hud.time_num2_sprite.setTexture(hud.time_num_tex);
        hud.time_num2_sprite.setTextureRect(IntRect(time_num2_index * (550 / 10), 0, 550 / 10, 55));

        hud.score_num_tex.loadFromFile(pathh + "Numbers Score.png");
        for (int i = 0; i < 6; i++)
        {
            hud.score_num_sprite[i].setTexture(hud.score_num_tex);
            hud.score_num_sprite[i].setTextureRect(IntRect(score_num_index[i] * (360 / 10), 0, 360 / 10, 36));
        }

        hud.ammo_num_tex.loadFromFile(pathh + "Numbers Ammo.png");
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
        hud.time_num2_sprite.setPosition(view.getCenter().x + (2 * view.getSize().x / 1920.0), view.getCenter().y - (520 * view.getSize().y / 1080.0));

        for (int i = 0; i < 6; i++)
        {
            hud.score_num_sprite[i].setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
            hud.score_num_sprite[i].setPosition(view.getCenter().x - (850 * view.getSize().x / 1920.0) + i * 38 * view.getSize().x / 1920.0, view.getCenter().y - (520 * view.getSize().y / 1080.0));
        }

        for (int i = 0; i < 3; i++)
        {
            hud.ammo_num_sprite[i].setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
            hud.ammo_num_sprite[i].setPosition(view.getCenter().x - (771 * view.getSize().x / 1920.0) + i * 41 * view.getSize().x / 1920.0, view.getCenter().y + (460 * view.getSize().y / 1080.0));
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
            hud.score_num_sprite[i].setTextureRect(IntRect(score_num_index[i] * (360 / 10), 0, 360 / 10, 36));
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
    void HUD_mechanics_call(HUD& hud) {
        hud.positions(hud);
        hud.time_calculation();
        hud.ammo_display();
        hud.score_display();
        hud.hp_display();
    }
    void time_calculation() {
        hud_int_time = hud_time.getElapsedTime().asSeconds();
        if (hud_time.getElapsedTime().asSeconds() <= 99) {
            time_num2_index = 9 - (hud_int_time % 10);
            time_num1_index = 9 - (hud_int_time / 10);
        }
        else {
            full_time_played += hud_time.getElapsedTime().asSeconds();
            hud_time.restart();
        }
    }
    void ammo_display() {
        ammo_num_index[2] = 9 - (rifle.ammo % 10);//123
        ammo_num_index[1] = 9 - ((rifle.ammo / 10) - (rifle.ammo / 100) * 10);
        ammo_num_index[0] = 9 - (rifle.ammo / 100);

    }
    void score_display() {
        //123456
        score_num_index[5] = 9 - (player.score % 10);// 6
        score_num_index[4] = 9 - ((player.score / 10) - (player.score / 100) * 10);//5
        score_num_index[3] = 9 - ((player.score / 100) - (player.score / 1000) * 10);//4
        score_num_index[2] = 9 - ((player.score / 1000) - (player.score / 10000) * 10);//3
        score_num_index[1] = 9 - ((player.score / 10000) - (player.score / 100000) * 10);//2
        score_num_index[0] = 9 - (player.score / 100000);//1
    }
    void hp_display() {
        if (player.health >= 20)
            hp_index = 5 - ((int)player.health / 20);
        else if (player.health <= 0)
            hp_index = 5;
    }
}hud;

//PowerUps4
struct PowerUps
{
    int pick_power_up = 1 + rand() % 5;
    Sprite powerup_sprite;
    Clock powerup_timer;
    void drop_power_up(vector<PowerUps>& powerups)
    {
        powerup.pick_power_up = 1 + rand() % 5;
        if (player.kill_count % 3 == 0)
        {
            if (pick_power_up == HEALTH_KIT)
            {
                powerup_sprite.setTexture(health_kit_tex);

            }
            else if (pick_power_up == HEALTH_POTION)
            {
                powerup_sprite.setTexture(health_potion_tex);

            }
            else if (pick_power_up == SPEED_POTION)
            {
                powerup_sprite.setTexture(speed_potion_tex);

            }
            else if (pick_power_up == DAMAGE_POTION)
            {
                powerup_sprite.setTexture(damage_potion_tex);

            }
            else if (pick_power_up == RIFLE_AMMO)
            {
                powerup_sprite.setTexture(rifle_ammo_tex);
            }
            else if (pick_power_up == LISER_AMMO)
            {

            }
            else if (pick_power_up == FLAME_AMMO)
            {

            }
            else if (pick_power_up == BIOCHEMICAL_AMMO)
            {

            }
            /*
              #define LISER_AMMO 6
              #define FLAME_AMMO 7
              #define BIOCHEMICAL_AMMO 8
              */
            powerup_sprite.setPosition(player.last_enemy_pos.x, player.last_enemy_pos.y + 70);
            powerup_sprite.setScale(2.5, 2.5);
            powerups.push_back(powerup);
        }
    }
    void apply_effects(vector<PowerUps>& powerups) {
        for (int i = 0; i < powerups.size(); i++) {
            if (player.rec.getGlobalBounds().intersects(powerups[i].powerup_sprite.getGlobalBounds())) {
                powerups[i].powerup_timer.restart();
                if (pick_power_up == HEALTH_KIT)
                {
                    player.health = 100;
                }
                else if (pick_power_up == HEALTH_POTION)
                {
                    if (player.health <= 60)
                        player.health += 40;
                    else player.health = 100;
                }
                else if (pick_power_up == SPEED_POTION)
                {
                    if (powerups[i].powerup_timer.getElapsedTime().asSeconds() <= 15) {
                        player.speed_boost = 2;
                    }

                }
                else if (pick_power_up == DAMAGE_POTION)
                {
                    if (powerups[i].powerup_timer.getElapsedTime().asSeconds() <= 15) {
                        rifle.damage *= 2;
                        pistol.damage *= 2;
                        liser.damage *= 2;
                    }
                }
                else if (pick_power_up == RIFLE_AMMO)
                {
                    rifle.ammo += 100;
                }
                else if (pick_power_up == LISER_AMMO)
                {

                }
                else if (pick_power_up == FLAME_AMMO)
                {

                }
                else if (pick_power_up == BIOCHEMICAL_AMMO)
                {

                }
                powerups.pop_back();
            }

        }

    }
}powerup;
vector <PowerUps> powerups;
//ground & wall
RectangleShape ground[30], wall[30];


//make clock & timer to plmovement
Clock clock_pl;
float dt = 0;
int ntimer[10] = { };

//enemy map 1

struct Enemy1
{
    Texture shootingtex, Dyingtex, Equippingtex, idle1tex, idle2tex, runningtex;
    Sprite sprite;
    RectangleShape rec;
    Vector2f velocity = { 0,0 };

    //shooting sound
    SoundBuffer shootingSoundB;
    Sound shootingSound;
    //death sound
    SoundBuffer deathSoundB;
    Sound deathSound;

    SoundBuffer deathSoundB2;
    Sound deathSound2;

    int damage = 1;
    float health = 10;
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
    vector<Sprite>bulletssprites;
    bool is_alive = 1;
    void setup(Enemy1 enemy1[30])
    {
        for (int i = 0; i < 30; i++)
        {
            //shooting sound
            enemy1[i].shootingSoundB.loadFromFile("Rebel Gun Bullet.wav");
            enemy1[i].shootingSound.setBuffer(enemy1[i].shootingSoundB);
            enemy1[i].shootingSound.setVolume(40);
            //death sound
            enemy1[i].deathSoundB.loadFromFile("Enemy Death 1.wav");
            enemy1[i].deathSound.setBuffer(enemy1[i].deathSoundB);
            enemy1[i].deathSound.setVolume(40);
            //death sound
            enemy1[i].deathSoundB2.loadFromFile("Enemy Death 3.wav");
            enemy1[i].deathSound2.setBuffer(enemy1[i].deathSoundB2);
            enemy1[i].deathSound2.setVolume(40);

            enemy1[i].runningtex.loadFromFile(pathh + "RS Running Sprite Sheet.png");
            enemy1[i].idle1tex.loadFromFile(pathh + "RS Idle 1 Sprite Sheet.png");
            enemy1[i].idle2tex.loadFromFile(pathh + "RS Idle 2 Sprite Sheet.png");
            enemy1[i].Equippingtex.loadFromFile(pathh + "RS Equipping Sprite Sheet.png");
            enemy1[i].Dyingtex.loadFromFile(pathh + "RS Dying Sprite Sheet.png");
            enemy1[i].shootingtex.loadFromFile(pathh + "RS Shooting Sprite Sheet.png");
            enemy1[i].sprite.setTexture(enemy1[i].runningtex);
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
    void Status(Enemy1 enemy1[30])
    {
        if (bgCounter == LEVEL_1_A_BG)
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
        }
        else
        {
            for (int i = 0; i < 30; i++)
                enemy1[i].velocity.x = 0;
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
            if (player.holding_knife && player.rec.getGlobalBounds().intersects(enemy1[i].sprite.getGlobalBounds()))
            {
                enemy1[i].health -= 0.3 / 2;
                enemy1[i].is_getting_damaged = true;
            }
        }
    }
    void shooting(int i, Enemy1 enemy1[30])
    {
        enemy1[i].shoot_timer += 0.08;
        if (enemy1[i].shoot_timer > 1 || enemy1[i].shoot_timer == 0.05)
        {
            enemy1[i].shootingSound.play();
            Vector2f pl = enemy1[i].sprite.getPosition();
            RectangleShape rect(sf::Vector2f(20, 10));
            rect.setOrigin(-pl.x + 30, -(pl.y + 40));
            Sprite spr;
            spr.setTexture(rifle.tex);
            spr.setOrigin(-pl.x + 30, -(pl.y + 40));
            enemy1[i].bulletssprites.push_back(spr);
            enemy1[i].bullet.push_back({ rect ,enemy1[i].last_key });
            enemy1[i].shoot_timer = 0;
        }
    }
    void Carrying_a_weapon(int i, Enemy1 enemy1[30])//i==index of enemy
    {
        enemy1[i].check = 0;
        enemy1[i].velocity.x = 0;
        enemy1[i].sprite.setTexture(enemy1[i].Equippingtex);
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
        enemy1[i].sprite.setTexture(enemy1[i].shootingtex);
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
        enemy1[i].sprite.setTexture(enemy1[i].runningtex);
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
            enemy1[i].sprite.setTexture(enemy1[i].runningtex);
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
            enemy1[i].sprite.setTexture(enemy1[i].runningtex);
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
            enemy1[i].sprite.setTexture(enemy1[i].Dyingtex);
            enemy1[i].sprite.setOrigin(341 / 11 / 2, 0);
            enemy1[i].sprite_indicator[1] += 0.2;
            if (enemy1[i].sprite_indicator[1] > 10.9)
            {
                player.score += 125;
                player.kill_count++;
                if (player.kill_count % 3 == 0) {
                    player.last_enemy_pos.x = enemy1[i].sprite.getPosition().x;
                    player.last_enemy_pos.y = enemy1[i].sprite.getPosition().y;
                    powerup.drop_power_up(powerups);
                }
                enemy1[i].sprite_indicator[1] = 0.2;
                enemy1[i].death_animation_done = 1;
            }
            if (enemy1[i].sprite_indicator[1] >= 1 && enemy1[i].sprite_indicator[1] < 1.3)
                enemy1[i].deathSound.play();
            enemy1[i].sprite.setTextureRect(IntRect(int(enemy1[i].sprite_indicator[1]) * 341 / 11, 0, 341 / 11, 39));
        }
        else
        {
            enemy1[i].is_alive = 0;

        }
    }
    void Idle_animation_2(int i, Enemy1 enemy1[30])
    {
        enemy1[i].sprite.setTexture(enemy1[i].idle2tex);
        EnemiAnimation(enemy1[i].sprite, 7.9, 232 / 8, 37, 0.005, enemy1[i].sprite_indicator[4]);
    }
    void Idle_animation_1(int i, Enemy1 enemy1[30])
    {
        enemy1[i].sprite.setTexture(enemy1[i].idle1tex);
        EnemiAnimation(enemy1[i].sprite, 5.9, 174 / 6, 38, 0.005, enemy1[i].sprite_indicator[5]);
    }
    void draw(Enemy1 RS[30])
    {
        for (int i = 0; i < 30; i++) {
            for (int x = 0; x < RS[i].bullet.size(); x++)
            {
                if (RS[i].bullet[x].second == LEFT)
                {
                    RS[i].bullet[x].first.move(-10, 0);
                    RS[i].bulletssprites[x].move(-10, 0);
                    window.draw(RS[i].bulletssprites[x]);

                }
                else if (RS[i].bullet[x].second == RIGHT)
                {
                    RS[i].bullet[x].first.move(10, 0);
                    RS[i].bulletssprites[x].move(10, 0);
                    window.draw(RS[i].bulletssprites[x]);
                }
            }
            if (RS[i].is_alive == 1) {
                //window.draw(enemy1[i].rec);
                window.draw(RS[i].sprite);
            }
        }
    }
}RS[30];//Rebel Soldier


struct Enemy2
{
    Sprite RWSpr;
    int RwInitialPos = 12500;
    float health = 10, damage = 0.02;
    float run_indicator = 0, attack_indicator = 0, death_indicator = 0;
    bool is_getting_damaged = false, is_alive = true;
    string current_status = "";
    Clock damage_timer;

    SoundBuffer shootingSoundB;
    Sound shootingSound;
    //death sound
    SoundBuffer deathSoundB;
    Sound deathSound;

    void setup(Enemy2 enemy2[5])
    {
        for (int i = 0; i < 5; i++)
        {
            //shooting sound
            enemy2[i].shootingSoundB.loadFromFile("Rebel Gun Bullet.wav");
            enemy2[i].shootingSound.setBuffer(enemy2[i].shootingSoundB);
            enemy2[i].shootingSound.setVolume(40);
            //death sound
            enemy2[i].deathSoundB.loadFromFile("Enemy Death 2.wav");
            enemy2[i].deathSound.setBuffer(enemy2[i].deathSoundB);
            enemy2[i].deathSound.setVolume(40);

            enemy2[i].RWSpr.setTexture(RWTexRun);
            enemy2[i].RWSpr.setScale(plScale, plScale);
            enemy2[i].RWSpr.setTextureRect(IntRect(0, 0, 36, 48));
            enemy2[i].RWSpr.setPosition(RwInitialPos + i * 1500, 750);
        }
    }
    void movement(Enemy2& s)
    {
        s.RWSpr.setTexture(RWTexRun);
        s.RWSpr.setOrigin(0, 0);
        if (player.rec.getPosition().x < s.RWSpr.getPosition().x)
        {

            s.RWSpr.setScale(plScale, plScale);
            EnemiAnimation(s.RWSpr, 11, 49, 52, 0.02, s.run_indicator);
            s.RWSpr.move(-5, 0);
        }
        else
        {
            s.RWSpr.setScale(-plScale, plScale);
            EnemiAnimation(s.RWSpr, 11, 49, 52, 0.02, s.run_indicator);
            s.RWSpr.move(5, 0);
        }

    }
    void Damaged(Enemy2 enemy2[5])
    {
        for (int i = 0; i < 5; i++)
        {
            if (enemy2[i].is_alive)
            {

                for (int j = 0; j < pistol.rects.size(); j++)
                {
                    if (enemy2[i].RWSpr.getGlobalBounds().intersects(pistol.rects[j].first.getGlobalBounds()) && pistol.rects[j].second.first != 0)
                    {
                        enemy2[i].health -= pistol.damage;
                        if (enemy2[i].health > 0)
                            enemy2[i].is_getting_damaged = 1;
                        pistol.rects[j].second.first = 0;
                    }
                }
                for (int j = 0; j < rifle.rects.size(); j++)
                {
                    if (enemy2[i].RWSpr.getGlobalBounds().intersects(rifle.rects[j].first.getGlobalBounds()) && rifle.rects[j].second.first != 0)
                    {
                        enemy2[i].health -= rifle.damage;
                        if (enemy2[i].health > 0)
                            enemy2[i].is_getting_damaged = 1;
                        rifle.rects[j].second.first = 0;
                    }
                }
                for (int j = 0; j < liser.rects.size(); j++)
                {
                    if (enemy2[i].RWSpr.getGlobalBounds().intersects(liser.rects[j].first.getGlobalBounds()) && liser.rects[j].second != 0)
                    {
                        enemy2[i].health -= liser.damage;
                        if (enemy2[i].health > 0)
                            enemy2[i].is_getting_damaged = 1;
                        liser.rects[j].second = 0;
                    }
                }
                if (enemy2[i].is_getting_damaged == 1)  // this adds red color to enemies when damaged
                {
                    if (enemy2[i].damage_timer.getElapsedTime().asMilliseconds() <= 300)
                    {
                        enemy2[i].RWSpr.setColor(Color::Red);
                    }
                    else {
                        enemy2[i].is_getting_damaged = 0;
                    }
                }
                else
                {
                    enemy2[i].RWSpr.setColor(Color::White);
                    enemy2[i].damage_timer.restart();
                }
                if (enemy2[i].health <= 0)
                {
                    death(enemy2[i]);

                }
            }
            if (player.holding_knife && player.rec.getGlobalBounds().intersects(enemy2[i].RWSpr.getGlobalBounds()))
            {
                enemy2[i].health -= 0.3 / 2;
                enemy2[i].is_getting_damaged = true;
            }
        }

    }
    void attack(Enemy2& s)
    {
        s.RWSpr.setTexture(RWTexAttack);
        s.RWSpr.setOrigin(92 / 2.0, 5);
        player.is_getting_damaged = true;
        player.health -= s.damage;
        if (player.rec.getPosition().x < s.RWSpr.getPosition().x)
        {
            s.RWSpr.setScale(plScale, plScale);
            EnemiAnimation(s.RWSpr, 8, 92, 54, 0.02 / 2, s.attack_indicator);
        }
        else
        {
            s.RWSpr.setScale(-plScale, plScale);
            EnemiAnimation(s.RWSpr, 8, 92, 54, 0.02 / 2, s.attack_indicator);
        }
    }
    void death(Enemy2& s)
    {
        s.RWSpr.setTexture(RWTexDeath);
        s.RWSpr.setOrigin(55 / 2.0, 0);
        s.RWSpr.setTextureRect(IntRect(int(s.death_indicator) * 55, 0, 55, 45));
        s.death_indicator += 0.02 * 10;
        if (s.death_indicator > 10)
        {
            player.score += 300;
            player.kill_count++;
            s.death_indicator = 0;
            s.is_alive = false;
        }
        if (s.death_indicator > 0.02 * 10 && s.death_indicator < 0.02 * 10 * 3)
        {
            enemy2[1].deathSound.play();
        }

    }

    void move_away(Enemy2& s)
    {
        s.RWSpr.setTexture(RWTexRun);
        s.RWSpr.setOrigin(0, 0);
        s.RWSpr.setScale(-plScale, plScale);
        EnemiAnimation(s.RWSpr, 11, 49, 52, 0.02, s.run_indicator);
        s.RWSpr.move(6, 0);
    }
    void status(Enemy2 enemy2[5])
    {
        if (bgCounter == LEVEL_1_B_BG)
        {
            for (int i = 0; i < 5; i++)
            {
                if (enemy2[i].is_alive)
                {
                    if (player.isdead && !player.live)
                        enemy2[i].current_status = "move_away";
                    else if (player.rec.getGlobalBounds().intersects(enemy2[i].RWSpr.getGlobalBounds()))
                        enemy2[i].current_status = "attack";
                    else
                        enemy2[i].current_status = "move";
                }
                else
                    enemy2[i].RWSpr.setScale(0, 0);
            }

            for (int i = 0; i < 5; i++)
            {
                if (enemy2[i].is_alive && enemy2[i].health > 0)
                {
                    if (enemy2[i].current_status == "move")
                        movement(enemy2[i]);
                    else if (enemy2[i].current_status == "attack")
                        attack(enemy2[i]);
                    else if (enemy2[i].current_status == "move_away")
                        move_away(enemy2[i]);
                }

            }
        }
    }

    void draw(Enemy2 enemy2[5])
    {
        for (int i = 0; i < 5; i++)
            window.draw(enemy2[i].RWSpr);
    }
}enemy2[5];

/*struct Tank
//{
//    // Tank shooting
//    struct Tank_shooting
//    {
//        float  damage = 30;
//        vector<pair<RectangleShape, int>>rects;// bullets //checker
//        vector<Sprite>sprite;
//        Clock shooting_timer;
//        void shooting(Tank_shooting& tank_shooting, Tank& tank)
//        {
//            Vector2f tankpos = tank.tankSprite.getPosition();
//            RectangleShape rect(sf::Vector2f(40, 10));
//            rect.setOrigin(-tankpos.x, -(tankpos.y - 150));
//            //Sprite spr;
//            //spr.setTexture();
//            //spr.setOrigin(-tankpos.x, -(tankpos.y - 150));
//            //sprite.push_back(spr)
//            tank_shooting.rects.push_back({ rect ,tank.last_key });
//        }
//        void draw_tank_bullets(Tank_shooting& tank_shooting)
//        {
//            for (int x = 0; x < tank_shooting.rects.size(); x++)
//            {
//                // Only move the rect here
//                if (tank_shooting.rects[x].second == LEFT)
//                {
//                    tank_shooting.rects[x].first.move(-15, 0);
//                    window.draw(tank_shooting.rects[x].first);
//                }
//                else if (tank_shooting.rects[x].second == RIGHT)
//                {
//                    window.draw(tank_shooting.rects[x].first);
//                    tank_shooting.rects[x].first.move(15, 0);
//                }
//            }
//        }
//    }shooting;


//    Texture tanktex, tankidle, tankmove, tankshoot, tankdie;
//    Sprite tankSprite;
//    bool stopped = 0;
//    float shoot_timer = 0;
//    Clock damage_timer;
//    bool is_getting_damaged = 0;
//    int tankInitialPos = 400;
//    float tankAnimationInd[4];
//    bool isdead = false;
//    int dmg = 100;
//    int health = 10;
//    int last_key = RIGHT;
//    bool is_alive = 1;

//    //127 × 48 pixels
//    void setup(Tank& tank)
//    {
//        tank.tanktex.loadFromFile(pathh + "Tank Idle Sprite Sheet.png");
//        tank.tankidle.loadFromFile(pathh + "Tank Idle Sprite Sheet.png");
//        tank.tankmove.loadFromFile(pathh + "Tank Moving Forward Sprite Sheet.png");
//        tank.tankshoot.loadFromFile(pathh + "Tank Shooting Sprite Sheet.png");
//        tank.tankdie.loadFromFile(pathh + "Tank Destroyed Sprite Sheet.png");

//        tank.tankSprite.setTexture(tank.tanktex);
//        tank.tankSprite.setTextureRect(IntRect(0, 0, 64, 48));
//        tank.tankSprite.setOrigin(32, 48);
//        tank.tankSprite.setScale(4, 4);
//        tank.tankSprite.setPosition(11500, 950);
//    }
//    void draw(Tank& tank)
//    {
//        if (tank.is_alive)
//        {
//            window.draw(tank.tankSprite);
//        }
//        tank.shooting.draw_tank_bullets(tank.shooting);
//    }
//    void tankIdleAnimation(Tank& tank)
//    {//127 × 48
//        tank.tankSprite.setTexture(tankidle);
//        EnemiAnimation(tank.tankSprite, 2, 127 / 2, 48, 0.004, tank.tankAnimationInd[0]);
//    }
//    void tankMovingAnimation(Tank& tank)
//    {//512 × 48
//        tank.tankSprite.setTexture(tankmove);
//        EnemiAnimation(tank.tankSprite, 8, 512 / 8, 48, 0.01, tank.tankAnimationInd[1]);
//    }
//    void tankShootingAnimation(Tank& tank)
//    {//260 × 48
//        tank.tankSprite.setTexture(tankshoot);
//        tank.tankAnimationInd[2] += 0.004 * dt;
//        if (tank.tankAnimationInd[2] > 3.9)
//        {
//            tank.tankAnimationInd[2] = 0;
//            tank.shooting.shooting(tank.shooting, tank);
//            tank.shooting.shooting_timer.restart();
//        }
//        tank.tankSprite.setTextureRect(IntRect(int(tank.tankAnimationInd[2]) * 260 / 4, 0, 260 / 4, 48));

//    }
//    void tankDeathAnimation(Tank& tank)
//    {//2133 × 200

//        tank.tankSprite.setTexture(tankdie);
//        tank.stopped = 1;
//        tank.tankAnimationInd[3] += 0.02;
//        if (tank.tankAnimationInd[3] > 26.9)
//        {
//            tank.tankAnimationInd[3] = 0;
//            player.score += 1000;
//            player.kill_count++;
//            tank.is_alive = 0;
//        }
//        tank.tankSprite.setTextureRect(IntRect(int(tank.tankAnimationInd[3]) * 2133 / 27, 0, 2133 / 27, 200));
//    }
//    void tankState(Tank& tank)
//    {
//        if (!tank.stopped)
//        {
//            int distance = tank.tankSprite.getPosition().x - player.upperbodySprite.getPosition().x;
//            if (abs(distance) > 1000)
//            {
//                tankIdleAnimation(tank);
//            }
//            else if (abs(distance) < 600)
//            {

//                if (tank.shooting.shooting_timer.getElapsedTime().asMilliseconds() > 1000)
//                {
//                    tankShootingAnimation(tank);
//                }

//            }
//            else
//            {
//                tankMovingAnimation(tank);

//                if (distance > 0)
//                {
//                    tank.tankSprite.setScale(4, 4);
//                    tank.tankSprite.move(-3, 0);
//                    tank.last_key = LEFT;
//                }
//                else
//                {
//                    tank.tankSprite.setScale(-4, 4);
//                    tank.tankSprite.move(3, 0);
//                    tank.last_key = RIGHT;
//                }
//            }
//        }

//    }
//    void Damaged(Tank& tank)
//    {
//        //if (tank.is_alive)
//        //{

//        //	for (int j = 0; j < pistol.rects.size(); j++)
//        //	{
//        //		if (tank.RWSpr.getGlobalBounds().intersects(pistol.rects[j].first.getGlobalBounds()) && pistol.rects[j].second.first != 0)
//        //		{
//        //			enemy2[i].health -= pistol.damage;
//        //			if (enemy2[i].health > 0)
//        //				enemy2[i].is_getting_damaged = 1;
//        //			pistol.rects[j].second.first = 0;
//        //		}
//        //	}
//        //	for (int j = 0; j < rifle.rects.size(); j++)
//        //	{
//        //		if (enemy2[i].RWSpr.getGlobalBounds().intersects(rifle.rects[j].first.getGlobalBounds()) && rifle.rects[j].second.first != 0)
//        //		{
//        //			enemy2[i].health -= rifle.damage;
//        //			if (enemy2[i].health > 0)
//        //				enemy2[i].is_getting_damaged = 1;
//        //			rifle.rects[j].second.first = 0;
//        //		}
//        //	}
//        //	for (int j = 0; j < liser.rects.size(); j++)
//        //	{
//        //		if (enemy2[i].RWSpr.getGlobalBounds().intersects(liser.rects[j].first.getGlobalBounds()) && liser.rects[j].second != 0)
//        //		{
//        //			enemy2[i].health -= liser.damage;
//        //			if (enemy2[i].health > 0)
//        //				enemy2[i].is_getting_damaged = 1;
//        //			liser.rects[j].second = 0;
//        //		}
//        //	}
//        //	if (enemy2[i].is_getting_damaged == 1)  // this adds red color to enemies when damaged
//        //	{
//        //		if (enemy2[i].damage_timer.getElapsedTime().asMilliseconds() <= 300)
//        //		{
//        //			enemy2[i].RWSpr.setColor(Color::Red);
//        //		}
//        //		else {
//        //			enemy2[i].is_getting_damaged = 0;
//        //		}
//        //	}
//        //	else
//        //	{
//        //		enemy2[i].RWSpr.setColor(Color::White);
//        //		enemy2[i].damage_timer.restart();
//        //	}
//        //	if (enemy2[i].health <= 0)
//        //	{
//        //		death(enemy2[i]);

//        //	}
//        //}
//        //if (player.holding_knife && player.rec.getGlobalBounds().intersects(enemy2[i].RWSpr.getGlobalBounds()))
//        //{
//        //	enemy2[i].health -= 0.3 / 2;
//        //	enemy2[i].is_getting_damaged = true;
//        //}
//    }

}tank;
*/

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
    vector<Sprite>bulletssprites;
    void shooting(int i, vector<Enemy3>& enemy1)
    {
        enemy1[i].shoot_timer += 0.08;
        if (enemy1[i].shoot_timer > 1 || enemy1[i].shoot_timer == 0.05)
        {
            RS[i].shootingSound.play();
            Vector2f pl = enemy1[i].sprite.getPosition();
            RectangleShape rect(sf::Vector2f(20, 10));
            rect.setOrigin(-pl.x + 30, -(pl.y + 40));
            Sprite spr;
            spr.setTexture(rifle.tex);
            spr.setOrigin(-pl.x + 30, -(pl.y + 40));
            enemy1[i].bulletssprites.push_back(spr);
            enemy1[i].bullet.push_back({ rect ,enemy1[i].last_key });
            enemy1[i].shoot_timer = 0;
        }
    }
    void drawbullets(int i, vector<Enemy3>& RS)
    {
        for (int x = 0; x < RS[i].bullet.size(); x++)
        {
            if (RS[i].bullet[x].second == LEFT)
            {
                RS[i].bullet[x].first.move(-10, 0);
                RS[i].bulletssprites[x].move(-10, 0);
                window.draw(RS[i].bulletssprites[x]);

            }
            else if (RS[i].bullet[x].second == RIGHT)
            {
                RS[i].bullet[x].first.move(10, 0);
                RS[i].bulletssprites[x].move(10, 0);
                window.draw(RS[i].bulletssprites[x]);
            }
        }
        if (RS[i].is_alive == 1) {
            //window.draw(enemy1[i].rec);
            window.draw(RS[i].sprite);
        }
    }
    void death_animation(int i, vector<Enemy3>& enemy1)
    {
        enemy1[i].stopped = 1;
        if (!enemy1[i].death_animation_done)
        {
            enemy1[i].velocity.x = 0;
            enemy1[i].sprite.setTexture(RS[i].Dyingtex);
            enemy1[i].sprite.setOrigin(341 / 11 / 2, 0);
            enemy1[i].sprite_indicator[1] += 0.2;
            if (enemy1[i].sprite_indicator[1] > 10.9)
            {
                player.score += 125;
                player.kill_count++;
                if (player.kill_count % 3 == 0) {
                    player.last_enemy_pos.x = enemy1[i].sprite.getPosition().x;
                    player.last_enemy_pos.y = enemy1[i].sprite.getPosition().y;
                    powerup.drop_power_up(powerups);
                }
                enemy1[i].sprite_indicator[1] = 0.2;
                enemy1[i].death_animation_done = 1;
            }
            enemy1[i].sprite.setTextureRect(IntRect(int(enemy1[i].sprite_indicator[1]) * 341 / 11, 0, 341 / 11, 39));
        }
        else
        {
            enemy1[i].is_alive = 0;
            RS[i].deathSound2.play();
        }
    }
    void Damaged(int i, vector<Enemy3>& enemy1)
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
                enemy1[i].velocity.x = 0;
                enemy1[i].death_animation(i, enemy1);
            }
        }
        if (player.holding_knife && player.rec.getGlobalBounds().intersects(enemy1[i].sprite.getGlobalBounds()))
        {
            enemy1[i].health -= 0.3 / 2;
            enemy1[i].is_getting_damaged = true;
        }
    }
    void Carrying_a_weapon(int i, vector<Enemy3>& enemy3)
    {
        enemy3[i].check = 0;
        enemy3[i].velocity.x = 0;
        enemy3[i].sprite.setTexture(RS[0].Equippingtex);
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
    void EnemyShootingAnimation(int i, vector<Enemy3>& enemy3)
    {
        enemy3[i].check = 1;
        enemy3[i].velocity.x = 0;
        enemy3[i].sprite.setTexture(RS[i].shootingtex);
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
    void EnemyRunningToPl(int i, vector<Enemy3>& enemy3)
    {
        enemy3[i].shoot_timer = 1;
        enemy3[i].isCarrying_a_weapon = 0;
        enemy3[i].sprite.setTexture(RS[0].runningtex);
        EnemiAnimation(enemy3[i].sprite, 11.9, 312 / 12, 40, 0.017, enemy3[i].sprite_indicator[2]);
        if (player.upperbodySprite.getPosition().x > enemy3[i].sprite.getPosition().x && player.health > 0)
        {
            if (i % 2 == 0)
                enemy3[i].velocity.x = 3;
            else if (i % 3 == 0)
                enemy3[i].velocity.x = 4.5;
            else
                enemy3[i].velocity.x = 3;
            enemy3[i].sprite.setScale(-plScale, plScale);
            enemy3[i].sprite.setOrigin(0, 0);
            enemy3[i].last_key = RIGHT;
        }
        else if (player.upperbodySprite.getPosition().x < enemy3[i].sprite.getPosition().x && player.health > 0)
        {
            if (i % 2 == 0)
                enemy3[i].velocity.x = -3;
            else if (i % 3 == 0)
                enemy3[i].velocity.x = -4.5;
            else
                enemy3[i].velocity.x = -3;
            enemy3[i].sprite.setScale(plScale, plScale);
            enemy3[i].sprite.setOrigin(enemy3[i].sprite.getLocalBounds().width, 0);
            enemy3[i].last_key = LEFT;
        }
        else
        {
            if (i % 2 == 0)
                enemy3[i].velocity.x = 3;
            else if (i % 3 == 0)
                enemy3[i].velocity.x = 4.5;
            else
                enemy3[i].velocity.x = 3;
            enemy3[i].sprite.setScale(-plScale, plScale);
            enemy3[i].sprite.setOrigin(0, 0);
            enemy3[i].last_key = RIGHT;
        }

    }
}new_enemy;
vector<Enemy3>enemy3;
void new_enemy_setup()
{
    new_enemy.rec.setSize(Vector2f(100, 130));
    new_enemy.rec.setPosition(Vector2f(19650, 820));
    new_enemy.sprite.setTexture(RS[0].runningtex);
    new_enemy.sprite.setScale(plScale, plScale);
    new_enemy.sprite.setTextureRect(IntRect(0, 0, 312 / 12, 40));
    new_enemy.sprite.setPosition(Vector2f(19650, 820));
}
void call()
{
    if (bgCounter == LEVEL_1_C_BG)
    {
        if (push_timer.getElapsedTime().asSeconds() * delay > 3)
        {
            delay += 0.1;
            enemy3.push_back(new_enemy);
            push_timer.restart();
        }
        for (int i = 0; i < enemy3.size(); i++)
        {
            if (!enemy3[i].stopped)
            {
                if (abs(player.upperbodySprite.getPosition().x - enemy3[i].sprite.getPosition().x) + 10 * i <= ENEMY_PLAYER_SHOOTING_RANGE && player.health > 0)
                {
                    if (!enemy3[i].isCarrying_a_weapon)
                    {
                        // Carrying_a_weapon
                        enemy3[i].Carrying_a_weapon(i, enemy3);
                    }
                    else
                    {
                        //EnemyShootingAnimation
                        enemy3[i].shooting(i, enemy3);
                        enemy3[i].EnemyShootingAnimation(i, enemy3);
                    }
                }
                else
                {
                    // EnemyRunningToPl
                    enemy3[i].EnemyRunningToPl(i, enemy3);
                }
            }
        }

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
            enemy3[i].Damaged(i, enemy3);
        }
    }
}

//gravity
float gravity = 0.7;
bool canDoubleJump;

//background Textures&Sprites
Texture bgTexture[30];
Sprite bgSprite[30];

////Mission start voice
SoundBuffer startvoice_B;
Sound startvoice;
bool once = 0;


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
//RectangleShape powerups(Vector2f(50, 50));

int main()
{
    pistol.setup(pistol);
    liser.setup(liser);
    rifle.setup(rifle);
    texture_setup();
    blood.setup(blood);
    window.setFramerateLimit(60);
    RS->setup(RS);
    enemy2->setup(enemy2);
    // tank.setup(tank);
    hud.setup(hud);
    bgSetup();
    TS_Setups();
    DeathScreen.deathScreenSetup();
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
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed && NewGameScreen.is_open)
            {
                if (event.key.code == sf::Keyboard::Backspace)
                    NewGameScreen.name.pop_back();
                else
                    NewGameScreen.name += key_code(event.key.code);
            }

            if (event.type == Event::Closed)
                window.close();
        }
        mouse_pos();

        if (player.isdead)
        {
            DeathScreen.main_screen_draw();
            if (Keyboard::isKeyPressed(Keyboard::Up) && timer.getElapsedTime().asMilliseconds() > 200)
            {
                timer.restart();
                DeathScreen.move_up();
            }
            if (Keyboard::isKeyPressed(Keyboard::Down) && timer.getElapsedTime().asMilliseconds() > 200)
            {
                timer.restart();
                DeathScreen.move_down();
            }
            if (DeathScreen.selected == 1 && Keyboard::isKeyPressed(Keyboard::Enter))
            {
                MenuClick.play();
            }
            if (DeathScreen.selected == 2 && Keyboard::isKeyPressed(Keyboard::Enter))
            {
                MenuClick.play();
                window.close();
            }
        }
        else

        {
            if (pause_menu.is_paused)
            {
                if (!menu.options_screen.isOpen)
                {
                    window_draw();
                    pause_menu.draw();
                }
                if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200 && !menu.options_screen.isOpen)
                {
                    MenuReturn.play();
                    pause_menu.is_paused = false;
                    escTimer.restart();
                }
                else if (pause_menu.selected == 1 && Keyboard::isKeyPressed(Keyboard::Enter))
                {
                    MenuClick.play();
                    pause_menu.exit();
                }
                else if (pause_menu.selected == 2 && Keyboard::isKeyPressed(Keyboard::Enter) && timer2.getElapsedTime().asMilliseconds() > 200 || menu.options_screen.isOpen)
                {
                    timer2.restart();
                    if (!menu.options_screen.isOpen)
                    {
                        MenuClick.play();
                        timer4.restart();
                        menu.options_screen.isOpen = true;
                    }
                    if (!menu.options_screen.music_slider.isOpen)
                        menu.options_screen.draw();
                    if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
                    {
                        MenuReturn.play();
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
                        if (!menu.options_screen.music_slider.isOpen)
                        {
                            menu.options_screen.music_slider.isOpen = true;
                            MenuClick.play();
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
                        if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
                        {
                            MenuReturn.play();
                            escTimer.restart();
                            menu.options_screen.music_slider.isOpen = false;
                        }
                    }
                }
                else if (pause_menu.selected == 3 && Keyboard::isKeyPressed(Keyboard::Enter))
                {
                    MenuClick.play();
                    window.close();
                }
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
                    if (!menu.start_selected && !menu.options_screen.isOpen && !NewGameScreen.is_open)
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
                                if (!NewGameScreen.is_open)
                                    menu.start_screen.draw();
                                if (!menu.start_selected)
                                {
                                    MenuClick.play();
                                    menu.start_selected = true;
                                    timer.restart();

                                }
                                if (menu.start_screen.selected == 1 && Keyboard::isKeyPressed(Keyboard::Enter) && timer.getElapsedTime().asMilliseconds() > 200)
                                {
                                    MenuClick.play();
                                    TS_BGTheme.stop();
                                    TS_BGFireFX.stop();
                                    GamePlayTheme.play();
                                    menu.game_started = true;
                                    timer.restart();
                                    windowfunction();

                                }
                                if (menu.start_screen.selected == 2 && Keyboard::isKeyPressed(Keyboard::Enter) && timer.getElapsedTime().asMilliseconds() > 300 || NewGameScreen.is_open)
                                {
                                    if (!NewGameScreen.is_open)
                                        MenuClick.play();
                                    timer.restart();
                                    NewGameScreen.is_open = true;
                                    NewGameScreen.draw();

                                    if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 300)
                                    {
                                        escTimer.restart();
                                        NewGameScreen.is_open = false;
                                        NewGameScreen.name = "";
                                    }
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
                                    MenuReturn.play();
                                    menu.start_selected = false;
                                    timer.restart();
                                }


                            }
                            else if (menu.selected == OPTIONS && Keyboard::isKeyPressed(Keyboard::Enter) && timer2.getElapsedTime().asMilliseconds() > 200 || menu.options_screen.isOpen)
                            {
                                timer2.restart();
                                if (!menu.options_screen.music_slider.isOpen)
                                {
                                    menu.options_screen.draw("menu");
                                }
                                if (!menu.options_screen.isOpen)
                                {
                                    MenuClick.play();
                                    menu.options_screen.isOpen = true;
                                    timer4.restart();
                                }
                                if (Keyboard::isKeyPressed(Keyboard::Enter) && menu.options_screen.selected == 1 && timer4.getElapsedTime().asMilliseconds() > 200 || menu.options_screen.music_slider.isOpen)
                                {
                                    timer4.restart();
                                    menu.options_screen.music_slider.draw("main");
                                    if (!menu.options_screen.music_slider.isOpen)
                                    {
                                        menu.options_screen.music_slider.isOpen = true;
                                        MenuClick.play();
                                    }

                                    if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
                                    {
                                        MenuReturn.play();
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
                                    MenuReturn.play();
                                    escTimer.restart();
                                    menu.options_screen.exit();
                                }
                            }
                            else if (menu.selected == EXIT && Keyboard::isKeyPressed(Keyboard::Enter))
                            {
                                MenuClick.play();
                                window.close();
                            }

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
}
void bgSetup()
{
    pistol.setup(pistol);
    new_enemy_setup();

    // LEVEL 1 A SET UP

    bgTexture[0].loadFromFile(pathh + "Level 1-A BG.png");
    bgSprite[0].setTexture(bgTexture[0]);
    bgSprite[0].setPosition(-370, -53);
    create(ground, 0, 8830, 30, -370, 800);
    // powerups.setPosition(500, 750);

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

    if (Keyboard::isKeyPressed(Keyboard::X))
        player.isdead = true;
    if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
    {
        MenuReturn.play();
        pause_menu.is_paused = true;
        escTimer.restart();
    }

    if (Keyboard::isKeyPressed(Keyboard::Q) && rifle.ammo > 0 && player.gun == PISTOL)
    {
        player.gun = RIFLE;
        rifle.gunvoice.play();
    }
    else if (Keyboard::isKeyPressed(Keyboard::W) && player.gun == RIFLE)
    {
        player.gun = PISTOL;
    }
    else if (Keyboard::isKeyPressed(Keyboard::E) && player.gun == PISTOL)
    {
        player.gun = LISER;
    }
    if (rifle.ammo <= 0)
        player.gun = PISTOL;

    if (!Keyboard::isKeyPressed(Keyboard::K))
        player.holding_knife = false;
    blood.move(blood);
    //map shortcut
    if (Keyboard::isKeyPressed(sf::Keyboard::T))
        player.upperbodySprite.setPosition(20000, 800);

    cameraView();
    RS->Status(RS);
    RS->Gravity(RS);

    RS->Damaged(RS);
    enemy2->status(enemy2);
    enemy2->Damaged(enemy2);
    playerDamageFromEnemy1();
    powerup.apply_effects(powerups);
    if (enemy3.size() < 30)
    {
        call();
    }
    if (player.health > 0)
        plmovement(player.lowerbodySprite, 11.9, 408 / 12, 41, 0.004, 2);

    //tank.tankState(tank);


    BGanimation();
    windowclose();
    window.clear();
    window_draw();
    window.setView(view);
    hud.HUD_mechanics_call(hud);
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
        if (once == 0)
        {
            startvoice.play();
            once = 1;
        }

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
        if (player.gun == PISTOL || player.gun == BIOCHEMICAL || player.gun == FLAME)
            move_with_animation(player.upperbodySprite, maxframe, x, y, delay, index);
        else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
            move_with_animation(player.upperbodySprite, 11.9, 528 / 11, 29, delay, 32);

    }

    //move player
    if (player.gun == PISTOL || player.gun == BIOCHEMICAL || player.gun == FLAME)
    {
        s.move(player.Velocity.x * (player.speed_boost), player.Velocity.y * player.speed_boost);
        player.upperbodySprite.move(player.Velocity.x * (player.speed_boost), player.Velocity.y * (player.speed_boost));
    }
    else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
    {
        s.move(player.Velocity.x * (player.speed_boost) * 0.7, player.Velocity.y * player.speed_boost);
        player.upperbodySprite.move(player.Velocity.x * (player.speed_boost) * 0.7, player.Velocity.y * (player.speed_boost));
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
                    pistol.shooting(pistol);
                else if (player.gun == RIFLE && rifle.ammo > 0)
                    rifle.shooting(rifle);
                else if (player.gun == FLAME) {
                    //flame.shooting
                }
                else if (player.gun == BIOCHEMICAL_AMMO)
                {  //biohemial shooting

                }
                else if (player.gun == LISER)
                {//liser shooting
                    liser.shooting(liser);
                }
                ShootingAnimation();//when pl (move&&shoot)
            }
            //if player move load running sprite sheet
            else
            {
                pistol.shoot_timer = 0;
                if (player.gun == PISTOL || player.gun == BIOCHEMICAL || player.gun == FLAME)
                {
                    player.upperbodySprite.setTexture(PTexRPU);
                    player.lowerbodySprite.setTexture(PTexRPL);
                    animation(s, maxframe, x, y, delay, index);
                }
                else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
                {
                    player.upperbodySprite.setTexture(PTexRRU);
                    player.lowerbodySprite.setTexture(PTexRRL);
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
                    pistol.shooting(pistol);
                else if (player.gun == RIFLE && rifle.ammo > 0)
                    rifle.shooting(rifle);
                else if (player.gun == FLAME) {
                    //flame.shooting
                }
                else if (player.gun == BIOCHEMICAL_AMMO)
                {  //biohemial shooting

                }
                else if (player.gun == LISER)
                {//liser shooting
                    liser.shooting(liser);
                }
                ShootingAnimation();//when pl (move&&shoot)
            }
            //if player move load running sprite sheet
            else
            {
                rifle.shoot_timer = 0;
                pistol.shoot_timer = 0;
                if (player.gun == PISTOL || player.gun == BIOCHEMICAL || player.gun == FLAME)
                {
                    player.upperbodySprite.setTexture(PTexRPU);
                    player.lowerbodySprite.setTexture(PTexRPL);
                    animation(s, maxframe, x, y, delay, index);
                }
                else if (player.gun == RIFLE || player.gun == LISER)
                {
                    player.upperbodySprite.setTexture(PTexRRU);
                    player.lowerbodySprite.setTexture(PTexRPL);
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
                if (player.gun == PISTOL || player.gun == BIOCHEMICAL || player.gun == FLAME)
                {
                    player.upperbodySprite.setTexture(PTexSSPU);
                    animation(player.upperbodySprite, 9.9, 520 / 10, 41, pistolshooting_delay, 10);
                }
                else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
                {
                    player.upperbodySprite.setTexture(PTexSSRU);
                    animation(player.upperbodySprite, 3.9, 240 / 4, 27, rifleshooting_delay, 10);
                }
                if (player.gun == PISTOL)
                    pistol.shooting(pistol);
                else if (player.gun == RIFLE && rifle.ammo > 0)
                    rifle.shooting(rifle);
                else if (player.gun == FLAME) {
                    //flame.shooting
                }
                else if (player.gun == BIOCHEMICAL_AMMO)
                {  //biohemial shooting

                }
                else if (player.gun == LISER)
                {//liser shooting
                    liser.shooting(liser);
                }
                player.lowerbodySprite.setTexture(PTexIPL);
                animation(player.lowerbodySprite, 3.9, 128 / 4, 37, 0.04, 3);
            }
            else  if (Keyboard::isKeyPressed(Keyboard::K))
            {
                player.melee_animation(player);
                player.holding_knife = true;
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
    for (int i = 0; i < enemy3.size(); i++)
    {
        if (enemy3[i].is_alive)
        {
            enemy3[i].drawbullets(i, enemy3);

        }
    }
    // enemy3[0].draw(enemy3[0]);
    window.draw(ground[12]);


    //guns
    pistol.drawpistol(window, pistol);//draw pistol bullets
    rifle.drawrifle(rifle); //draw rifle bullets
    liser.drawliser(liser);


    if (player.live && !player.isdead)
    {
        window.draw(player.lowerbodySprite);
        if (!player.one_sprite_needed)
            window.draw(player.upperbodySprite);
    }
    blood.call(blood);
    for (int i = 0; i < 4; i++)
    {
        if (i == 1)continue;
        window.draw(Lvl1FG[i]);
    }
    for (int i = 0; i < powerups.size(); i++)
    {
        window.draw(powerups[i].powerup_sprite);
    }

    // window.draw(ground[0]);
    hud.draw(hud);
    enemy2->draw(enemy2);
    //  tank.draw(tank);
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
    if (player.gun == PISTOL || player.gun == BIOCHEMICAL || player.gun == FLAME)
    {
        player.lowerbodySprite.setTexture(PTexIPSL);
        animation(player.lowerbodySprite, 3.9, 128 / 4, 37, idle, 3);
        player.is_shooting = 0;
        player.upperbodySprite.setTexture(PTexIPSU);
        animation(player.upperbodySprite, 3.9, 128 / 4, 37, idle, 3);
    }
    else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
    {
        player.lowerbodySprite.setTexture(PTexIRL);
        animation(player.lowerbodySprite, 3.9, 152 / 4, 37, idle, 3);
        player.is_shooting = 0;
        player.upperbodySprite.setTexture(PTexIRU);
        animation(player.upperbodySprite, 3.9, 152 / 4, 37, idle, 3);
    }
}
void ShootingAnimation()
{
    player.lowerbodySprite.setTexture(PTexRPL);
    animation(player.lowerbodySprite, 11.9, 408 / 12.0, 41, 0.004, 2);
    if (player.gun == PISTOL || player.gun == BIOCHEMICAL || player.gun == FLAME)
    {
        player.upperbodySprite.setTexture(PTexSSPU);
        animation(player.upperbodySprite, 9.9, 520 / 10.0, 41, pistolshooting_delay, 10);
    }
    else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
    {
        player.upperbodySprite.setTexture(PTexSSRU);
        animation(player.upperbodySprite, 3.9, 240 / 4.0, 27, rifleshooting_delay, 10);
    }
}
void crouchingAnimation()
{
    player.crouch = 1;
    player.one_sprite_needed = 1;
    if (Keyboard::isKeyPressed(Keyboard::J))
    {
        if (player.gun == PISTOL || player.gun == BIOCHEMICAL || player.gun == FLAME)
        {
            player.lowerbodySprite.setTexture(PTexSCPL);
            animation(player.lowerbodySprite, 9.9, 520 / 10, 29, pistolshooting_delay, 14);
        }
        else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
        {
            player.lowerbodySprite.setTexture(PTexSCRL);
            animation(player.lowerbodySprite, 3.9, 268 / 4, 28, pistolshooting_delay, 14);
        }
        if (player.gun == PISTOL)
        {
            pistol.shooting(pistol);
        }
        else if (player.gun == RIFLE && rifle.ammo > 0)
        {
            rifle.shooting(rifle);
        }
        else if (player.gun == FLAME)
        {
            //flame.shooting
        }
        else if (player.gun == BIOCHEMICAL_AMMO)
        {  //biohemial shooting

        }
        else if (player.gun == LISER)
        {//liser shooting
            liser.shooting(liser);
        }
        player.lowerbodySprite.setPosition(player.upperbodySprite.getPosition().x, player.upperbodySprite.getPosition().y + player.upperbodyTex.getSize().y - 10);
    }
    else if (Keyboard::isKeyPressed(Keyboard::K))
    {
        player.melee_animation(player);
        player.holding_knife = true;
    }
    else
    {
        player.lowerbodySprite.setPosition(player.upperbodySprite.getPosition().x, player.upperbodySprite.getPosition().y + player.upperbodyTex.getSize().y);
        if (player.gun == PISTOL || player.gun == BIOCHEMICAL || player.gun == FLAME)
        {
            player.lowerbodySprite.setTexture(PTexICPL);
            animation(player.lowerbodySprite, 3.9, 136 / 4.0, 24, idle, 4);
        }
        else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
        {
            player.lowerbodySprite.setTexture(PTexICRL);
            animation(player.lowerbodySprite, 3.9, 164 / 4.0, 24, idle, 4);
        }
    }
}
void jumpingAnimation(float delay)
{
    if (player.gun == PISTOL || player.gun == BIOCHEMICAL || player.gun == FLAME)
    {
        player.upperbodySprite.setTexture(PTexJPU);
        animation(player.upperbodySprite, 10.9, 319.0 / 11, 49, delay, 5);
    }
    else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
    {
        player.upperbodySprite.setTexture(PTexJRU);
        animation(player.upperbodySprite, 5.9, 222 / 6, 27, delay, 5);
    }
    player.lowerbodySprite.setTexture(PTexJPL);
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
                    player.is_getting_damaged = 1;
                    RS[i].bullet[j].second = 0;

                }
                if (RS[i].bullet[j].first.getPosition().x > 9000 || RS[i].bullet[j].first.getPosition().x < -500)
                {
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
        player.one_sprite_needed = 1;
        player.lowerbodySprite.setTexture(PTexDL);
        player.lowerbodySprite.setOrigin(480 / 10 / 2, 0);
        player.pl_death_ctr += 0.2;
        if (player.pl_death_ctr > 9.9)
        {
            cout << "is dead";
            player.isdead = 1;
        }
        if (player.pl_death_ctr >= 0.2 && player.pl_death_ctr < 0.5)
        {
            //sound
            player.playergeath.play();
        }
        if (!player.isdead)
            player.lowerbodySprite.setTextureRect(IntRect(int(player.pl_death_ctr) * 480 / 10, 0, 480 / 10, 44));
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

    GamePlayB.loadFromFile("Game BG Music.wav");
    GamePlayTheme.setBuffer(GamePlayB);
    GamePlayTheme.setLoop(true);

    MenuReturnB.loadFromFile("Menu Return FX.wav");
    MenuReturn.setBuffer(MenuReturnB);

    NewGameSpr.setPosition(15, -200);
    NewGameTex.loadFromFile("Enter Your Name.png");
    NewGameSpr.setTexture(NewGameTex);

    nameFont.loadFromFile("Inversionz Unboxed.ttf");
    nameDis.setFont(nameFont);
    nameDis.setPosition(780, 740);
    nameDis.setCharacterSize(80);

}
void texture_setup()
{
    RWTexRun.loadFromFile(pathh + "RW Running Sprite Sheet.png");
    RWTexAttack.loadFromFile(pathh + "RW Fighting Sprite Sheet.png");
    RWTexDeath.loadFromFile(pathh + "RW Dying Sprite Sheet.png");
    PTexMU.loadFromFile(pathh + "Melee (Pistol) Sprite Sheet Upper Body.png");
    PTexML.loadFromFile(pathh + "Melee (Pistol) Sprite Sheet Lower Body.png");
    PTexRPU.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Upper Body.png");
    PTexRPL.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Lower Body.png");
    PTexRRU.loadFromFile(pathh + "Running (Rifle) Sprite Sheet.png");
    PTexRRL.loadFromFile(pathh + "Running (Pistol) Sprite Sheet Lower Body.png");
    PTexSSPU.loadFromFile(pathh + "Shooting - Standing (Pistol) Sprite Sheet Upper Body.png");
    PTexSSRU.loadFromFile(pathh + "Shooting - Standing (Rifle) Sprite Sheet.png");
    PTexIPSL.loadFromFile(pathh + "Idle (Pistol) Sprite Sheet Lower Body.png");
    PTexIPSU.loadFromFile(pathh + "Idle (Pistol) Sprite Sheet Upper Body.png");
    PTexIRL.loadFromFile(pathh + "Idle (Rifle) Sprite Sheet Lower Body.png");
    PTexIRU.loadFromFile(pathh + "Idle (Rifle) Sprite Sheet Upper Body.png");
    PTexSCPL.loadFromFile(pathh + "Shooting - Crouching (Pistol) Sprite Sheet.png");
    PTexIPL.loadFromFile(pathh + "Idle (Pistol) Sprite Sheet Lower Body.png");
    PTexSCRL.loadFromFile(pathh + "Shooting - Crouching (Rifle) Sprite Sheet.png");
    PTexICPL.loadFromFile(pathh + "Idle - Crouching (Pistol) Sprite Sheet.png");
    PTexICRL.loadFromFile(pathh + "Idle - Crouching (Rifle) Sprite Sheet.png");
    PTexJPU.loadFromFile(pathh + "Jumping (Pistol) Sprite Sheet Upper Body.png");
    PTexJRU.loadFromFile(pathh + "Jumping (Rifle) Sprite Sheet.png");
    PTexJPL.loadFromFile(pathh + "Jumping (Pistol) Sprite Sheet Lower Body.png");
    PTexDL.loadFromFile(pathh + "Dying Sprite Sheet.png");
    health_kit_tex.loadFromFile(pathh + "Health Kit.png");
    health_potion_tex.loadFromFile(pathh + "Health Potion.png");
    speed_potion_tex.loadFromFile(pathh + "Speed Potion.png");
    damage_potion_tex.loadFromFile(pathh + "Damage Potion.png");
    rifle_ammo_tex.loadFromFile(pathh + "Rifle Icon.png");
    startvoice_B.loadFromFile("Mission 1 Start.wav");
    startvoice.setBuffer(startvoice_B);
    startvoice.setVolume(60);
}
void mouse_pos()
{
    if (Mouse::isButtonPressed(Mouse::Left))
    {
        Vector2i mousePos = Mouse::getPosition(window);
        cout << mousePos.x << ' ' << mousePos.y << '\n';
    }
}

char key_code(sf::Keyboard::Key key)
{
    char keyChar = '\0';
    switch (key)
    {
    case sf::Keyboard::A:
        keyChar = 'A';
        break;
    case sf::Keyboard::B:
        keyChar = 'B';
        break;
    case sf::Keyboard::C:
        keyChar = 'C';
        break;
    case sf::Keyboard::D:
        keyChar = 'D';
        break;
    case sf::Keyboard::E:
        keyChar = 'E';
        break;
    case sf::Keyboard::F:
        keyChar = 'F';
        break;
    case sf::Keyboard::G:
        keyChar = 'G';
        break;
    case sf::Keyboard::H:
        keyChar = 'H';
        break;
    case sf::Keyboard::I:
        keyChar = 'I';
        break;
    case sf::Keyboard::J:
        keyChar = 'J';
        break;
    case sf::Keyboard::K:
        keyChar = 'K';
        break;
    case sf::Keyboard::L:
        keyChar = 'L';
        break;
    case sf::Keyboard::M:
        keyChar = 'M';
        break;
    case sf::Keyboard::N:
        keyChar = 'N';
        break;
    case sf::Keyboard::O:
        keyChar = 'O';
        break;
    case sf::Keyboard::P:
        keyChar = 'P';
        break;
    case sf::Keyboard::Q:
        keyChar = 'Q';
        break;
    case sf::Keyboard::R:
        keyChar = 'R';
        break;
    case sf::Keyboard::S:
        keyChar = 'S';
        break;
    case sf::Keyboard::T:
        keyChar = 'T';
        break;
    case sf::Keyboard::U:
        keyChar = 'U';
        break;
    case sf::Keyboard::V:
        keyChar = 'V';
        break;
    case sf::Keyboard::W:
        keyChar = 'W';
        break;
    case sf::Keyboard::X:
        keyChar = 'X';
        break;
    case sf::Keyboard::Y:
        keyChar = 'Y';
        break;
    case sf::Keyboard::Z:
        keyChar = 'Z';
        break;
    case sf::Keyboard::Num0:
        keyChar = '0';
        break;
    case sf::Keyboard::Num1:
        keyChar = '1';
        break;
    case sf::Keyboard::Num2:
        keyChar = '2';
        break;
    case sf::Keyboard::Num3:
        keyChar = '3';
        break;
    case sf::Keyboard::Num4:
        keyChar = '4';
        break;
    case sf::Keyboard::Num5:
        keyChar = '5';
        break;
    case sf::Keyboard::Num6:
        keyChar = '6';
        break;
    case sf::Keyboard::Num7:
        keyChar = '7';
        break;
    case sf::Keyboard::Num8:
        keyChar = '8';
        break;
    case sf::Keyboard::Num9:
        keyChar = '9';
        break;
    case sf::Keyboard::Space:
        keyChar = ' ';
        break;
    case sf::Keyboard::Enter:
        keyChar = '\n';
        break;
    case sf::Keyboard::Backspace:
        keyChar = '\b';
        break;
    case sf::Keyboard::Tab:
        keyChar = '\t';
        break;
    case sf::Keyboard::Escape:
        keyChar = '\x1b';
        break;
    default:
        break;
    }
    return keyChar;
}
///
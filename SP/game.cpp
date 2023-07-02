#include <SFML/Graphics.hpp> //seni is gay
#include<thread>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include<cmath>

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

//power ups
#define ENEMY_PLAYER_RANGE 700
#define ENEMY_PLAYER_SHOOTING_RANGE 400
#define HEALTH_KIT 1
#define HEALTH_POTION 2
#define SPEED_POTION 3
#define DAMAGE_POTION 4
#define RIFLE_AMMO 5
#define LISER_AMMO 6


using namespace std;
using namespace sf;

RenderWindow window(sf::VideoMode(1920, 1080), "Game", sf::Style::Fullscreen);
Event event;

fstream out_names, in_names, lname;

//make the variable 
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
health_kit_tex, health_potion_tex, speed_potion_tex, damage_potion_tex, rifle_ammo_tex, laser_ammo_tex, deathScreenBGTex,
deathScreenFGTex, deathScreenFGTex2, NewGameTex, LeaderBoardTex, LeaderBoardSheetTex, LoadGameTex, CreditsBG, CreditsFG;


Sprite TS_BGSpr, TS_TandGSpr, TS_LSpr, TS_VSpr, TS_LogoSpr, TS_PESpr, TS_buttonsSpr, TS_SSSpr, NewGameSpr,
TS_OlSpr, OptionsSpr, MusicControlSpr, PMSpr, deathScreenBGSpr, deathScreenFGSpr, deathScreenFGSpr2, LeaderBoardSpr, LeaderBoardSheetSpr, CreditsBGSpr, CreditsFGSpr,
LoadGameSpr, SoundEffectsControlSpr;


Music TS_BGTheme;
Music TS_BGFireFX;


SoundBuffer MenuClickB, MenuScrollB, DeathScreenFXB, GamePlayB, MenuReturnB, CreditsMusicB;
Sound MenuClick, MenuScroll, DeathScreenFX, GamePlayTheme, MenuReturn, startvoice, CreditsMusic;


Clock timer, timer2, timer4, escTimer, eventTimer, LB_timer;


View view(Vector2f(0, 0), Vector2f(1920, 1080));

bool creditsactive = 1, cutscene, cuton;
const int fadeinT = 2000;
const int fadeoutT = 2000;
const int waitT = 2000;
const int totalT = fadeinT + fadeoutT + waitT;
const float AlphaIncrementC = float(255) / fadeinT;
float CurrentAlpha;
float CreditsAlpha = 0;
float CreditsCntX = 0;
float CreditsCntY = 0;
float CreditsFGCntX = 0;
float CreditsFGCntY = 0;

Clock CreditsTClock;
int currentF = 0;
int currentR = 0;
int numF = 12;

float TS_TandGCnt = 0, TS_LCnt = 0, TS_LogoCnt = 0, TS_PECnt = 0, AlphaPE = 255;
int TS_ButtonsCnt = 0, TSS_ButtonsCnt = 0, OptionsSprCnt = 0, deathScreenFG2cnt = 0;

bool game_ended = 0;

Font nameFont;
Text nameDis, LeaderBoardText, LoadSaveText;
// Moved from down to here because

int bgCounter = 0, leftEnd, rightEnd;//indicator for current map, start of map, end of map
bool  isMoved = false, ismoved2 = 0, ismoved3 = 0;//flags for clearing the previous map
Clock blackscreenTimer;

int full_time_played = 0, damage_boost = 1;

vector <pair <string, string>> names_save;
vector <pair <int, string>> leaderboard_vec;
string current_name = "", last_name = "";

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
void FBCutScene();
//player
struct Player
{
	//?
	float plcounter = 0, pl_upper_melee_ctr = 0, pl_lower_melee_ctr = 0, pl_death_ctr = 0;
	Texture upperbodyTex, lowerbodyTex;
	Sprite upperbodySprite, lowerbodySprite;

	RectangleShape rec;//make rectangle to  collisions
	Vector2f Velocity = { 0,0 }, last_enemy_pos;
	float health = 100;

	SoundBuffer playerdeath_B;
	Sound playerdeath;

	int score = 0, speed_boost = 1, num_of_lives = 3;
	int gun = PISTOL, kill_count = 0;
	bool canshoot = 0;
	bool crouch = 0;
	bool is_shooting = 0;
	bool one_sprite_needed = 0;
	int last_key = 0;
	bool live = 1, is_getting_damaged = 0;
	Clock damage_timer;
	bool isdead = 0;
	bool holding_knife = 0, powerup_is_on = 0;
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
		player.playerdeath.setBuffer(player.playerdeath_B);
		player.playerdeath.setVolume(40);
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
	//rifle variables
	float  damage = 0.9 * damage_boost;
	float shoot_timer = 0;
	int range = 1200, ammo = 200;
	Texture tex;

	//sounds
	SoundBuffer sound_B, gunvoice_B;
	Sound sound, gunvoice;

	//bullets
	vector<pair<RectangleShape, pair< int, double>>>rects;// vector < bullets , piar< last key , distance   >  >
	vector<Sprite>sprite;// bullets sprite

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
		if (rifle.ammo > 0)//cheeck if rifle ammo more than zero 
		{
			rifle.shoot_timer += 0.08;//delay
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
			//last key = left
			if (rifle.rects[x].second.first == LEFT)
			{
				rifle.rects[x].first.move(-10, 0);
				rifle.sprite[x].move(-10, 0);
				window.draw(rifle.sprite[x]);
			}//last key = right
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
	float damage = 0.001 * damage_boost;
	bool can_use_laser = 0;
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
		if (liser.shooting_timer.getElapsedTime().asSeconds() > 6)//restart liser power  
		{
			liser.shooting_timer.restart();
		}
		liser.sound.play();
		liser.damage = 0.0001 * liser.shooting_timer.getElapsedTime().asMilliseconds();
		Vector2f pl = player.lowerbodySprite.getPosition();
		RectangleShape rect(sf::Vector2f(100, 2 * liser.shooting_timer.getElapsedTime().asSeconds() + 5));
		rect.setOrigin(-pl.x, -(pl.y + 50));
		rect.setFillColor(Color::Red);
		liser.rects.push_back({ rect ,player.last_key });
		if (liser.shooting_timer.getElapsedTime().asSeconds() > 8)
		{
			liser.shooting_timer.restart();
		}

	}

	void drawliser(Liser& liser)
	{
		for (int x = 0; x < liser.rects.size(); x++)
		{
			//check last key
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
	void restart()//restart liser power  

	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased)
			{
				liser.shooting_timer.restart();
			}
		}
	}
}liser;

// shooting <pistol>
struct Pistol
{
	float  damage = 1.1 * damage_boost;
	float shoot_timer = 0;
	//sound
	SoundBuffer soundb;
	Sound sound;


	Texture tex;
	vector<Sprite>sprite;
	vector<pair<RectangleShape, pair<int, double>>>rects;// bullets //checker //position.x

	int range = 1200;//maximum distance
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
		pistol.sound.play();
		Vector2f pl = player.lowerbodySprite.getPosition();
		RectangleShape rect(sf::Vector2f(10, 10));
		rect.setOrigin(-pl.x, -(pl.y + 50));
		Sprite spr;
		spr.setTexture(pistol.tex);
		spr.setOrigin(-pl.x, -(pl.y + 50));
		pistol.rects.push_back({ rect ,{player.last_key,0} });
		pistol.sprite.push_back(spr);
	}
	void drawpistol(RenderWindow& window, Pistol& pistol)
	{
		for (int x = 0; x < pistol.rects.size(); x++)
		{
			// check last key
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


//draw blood when player damaged
struct Blood_Spatter
{
	Texture tex;
	Sprite sprite;
	float indicator; //sprite indicator
	bool animationdone;

	void setup(Blood_Spatter& blood)//load files and set up before game loop
	{
		blood.tex.loadFromFile(pathh + "Blood Spatter.png");
		blood.sprite.setTexture(blood.tex);
		blood.sprite.setTextureRect(IntRect(0, 0, 500, 500));
		blood.sprite.setScale(0.5, 0.5);
	}
	void update(Blood_Spatter& blood)//animation and restart timer
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
	void move(Blood_Spatter& blood)//update position  
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
struct File
{
	void load()
	{
		in_names.open("names.txt", ios::in);

		string temp = "";

		for (int i = 0; !in_names.eof(); i++)
		{
			names_save.push_back({ temp, temp });
			in_names >> names_save[i].first >> names_save[i].second;
		}
		last_name = names_save[names_save.size() - 1].first;
		names_save.pop_back();
		in_names.close();
	}
	void save()
	{
		out_names.open("names.txt", ios::out);

		for (int i = 0; i < names_save.size(); i++)
		{
			if (names_save[i].first == current_name)
			{
				if (player.score > stoi(names_save[i].second))
					names_save[i].second = to_string(player.score);
			}
		}
		for (int i = 0; i < names_save.size(); i++)
			out_names << names_save[i].first << ' ' << names_save[i].second << '\n';
		out_names << current_name << ' ' << 'a';
		out_names.close();
	}
}file;
struct LeaderBoard
{
	float XanimCtr = 0, YanimCtr = 0;
	bool is_compared = false;

	void setup()
	{
		LeaderBoardText.setFont(nameFont);
		LeaderBoardText.setCharacterSize(54);

	}
	void compare()
	{
		for (int i = 0; i < names_save.size(); i++)
		{
			leaderboard_vec.push_back({ 0, "" });
			leaderboard_vec[i].first = stoi(names_save[i].second);
			leaderboard_vec[i].second = names_save[i].first;
		}

		sort(leaderboard_vec.begin(), leaderboard_vec.end());
	}
	void leaderboard_show()
	{
		for (int i = leaderboard_vec.size() - 1; i >= 0; i--)
			cout << leaderboard_vec[i].second << ' ' << leaderboard_vec[i].first << '\n';
	}
	void draw()
	{
		LeaderBoardSpr.setTextureRect(IntRect(int(XanimCtr) * 1920, int(YanimCtr) * 1080, 1920, 1080));
		LeaderBoardSpr.setPosition(view.getCenter().x - (980 * view.getSize().x / 1600.0), view.getCenter().y - (600 * view.getSize().y / 1080.0));
		XanimCtr += 0.06 * 10;
		if (XanimCtr > 4)
		{
			XanimCtr = 0;
			YanimCtr = YanimCtr == 0 ? 1 : 0;
		}

		LeaderBoardSheetSpr.setPosition(view.getCenter().x - (940 * view.getSize().x / 1600.0), view.getCenter().y - (500 * view.getSize().y / 1080.0));



		window.clear();

		window.draw(LeaderBoardSpr);
		window.draw(LeaderBoardSheetSpr);
		for (int i = 0, k = leaderboard_vec.size() - 1; i < names_save.size(); i++, k--)
		{
			for (int j = 0; j < 3; j++)
			{

				switch (j)
				{
				case 0:
					LeaderBoardText.setString("#" + to_string(i + 1));
					LeaderBoardText.setPosition(view.getCenter().x - 550, view.getCenter().y - 230 + i * 50);
					break;
				case 1:
					LeaderBoardText.setString(leaderboard_vec[k].second);
					LeaderBoardText.setPosition(view.getCenter().x - 300, view.getCenter().y - 230 + i * 50);
					break;
				case 2:
					LeaderBoardText.setString(to_string(leaderboard_vec[k].first));
					LeaderBoardText.setPosition(view.getCenter().x + 500, view.getCenter().y - 230 + i * 50);
					break;
				}

				window.draw(LeaderBoardText);
			}
		}
		window.display();

	}
}LeaderBoard;
struct LoadGameScreen
{
	bool isOpen = false;
	int selected = 1;

	void move_up()
	{
		selected = (selected == 1 ? names_save.size() : selected - 1);
		MenuScroll.play();
	}

	void move_down()
	{
		selected = (selected == names_save.size() ? 1 : selected + 1);
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
		TS_OlSpr.setPosition(0.f, 0.f);

		window.draw(TS_BGSpr);
		window.draw(TS_TandGSpr);
		window.draw(TS_LSpr);
		window.draw(TS_OlSpr);
		window.draw(LoadGameSpr);
		for (int i = 0; i < names_save.size(); i++)
		{
			LoadSaveText.setString(names_save[i].first);
			LoadSaveText.setPosition(850, 300 + i * 75);

			if (selected - 1 == i)
				LoadSaveText.setFillColor(Color::Red);
			else
				LoadSaveText.setFillColor(Color::White);
			window.draw(LoadSaveText);

		}
		window.display();
	}
}LoadGameScreen;

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
		deathScreenFGSpr2.setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
		deathScreenFGSpr2.setTextureRect(IntRect(deathScreenFG2cnt * 1920, 0, 1920, 1080));
		TS_OlSpr.setScale(3 * view.getSize().x / 1920.0, 3 * view.getSize().y / 1080.0);
		TS_OlSpr.setPosition(view.getCenter().x - (1000 * view.getSize().x / 1920.0), view.getCenter().y - (600 * view.getSize().y / 1080.0));

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
struct CreditScreen
{
	bool isOpen = false;

	void creditsSetup() {
		CreditsBG.loadFromFile("Leaderboard BG Sprite Sheet.png");
		CreditsFG.loadFromFile("Credits Sprite Sheet.png");
		CreditsBGSpr.setTexture(CreditsBG);
		CreditsFGSpr.setTexture(CreditsFG);
		CreditsMusicB.loadFromFile("Credits Music.wav");
		CreditsMusic.setBuffer(CreditsMusicB);
	}

	void creditsAnimation()
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			isOpen = false;
		CreditsBGSpr.setTextureRect(IntRect(int(CreditsCntX) * 1920, int(CreditsCntY) * 1080, 1920, 1080));
		CreditsCntX += 0.5;
		if (CreditsCntX > 4)
		{
			CreditsCntX = 0;
			CreditsCntY = CreditsCntY == 0 ? 1 : 0;
		}
		float elapsedT = CreditsTClock.getElapsedTime().asMilliseconds();
		//cout << elapsedT << endl;
		if (elapsedT < fadeinT) {
			CurrentAlpha = elapsedT * AlphaIncrementC;
			CreditsFGSpr.setTextureRect(IntRect(currentF * 1920, currentR * 1080, 1920, 1080));
			CreditsFGSpr.setColor(Color(255, 255, 255, CurrentAlpha));
		}
		else if (elapsedT < fadeinT + waitT) {
			CurrentAlpha = 255;
			CreditsFGSpr.setTextureRect(IntRect(currentF * 1920, currentR * 1080, 1920, 1080));
			CreditsFGSpr.setColor(Color(255, 255, 255, CurrentAlpha));
		}
		else if (elapsedT < totalT) {
			CurrentAlpha = 255 - ((elapsedT - (fadeinT + waitT)) * AlphaIncrementC);
			CreditsFGSpr.setTextureRect(IntRect(currentF * 1920, currentR * 1080, 1920, 1080));
			CreditsFGSpr.setColor(Color(255, 255, 255, CurrentAlpha));
		}
		else if (elapsedT >= totalT) {
			CreditsTClock.restart();
			currentF = (currentF + 1);
			if (currentR == 1 && currentF == 6)
				creditsactive = 0;
			if (currentF > 5)
			{
				currentF = 0;
				currentR = currentR == 0 ? 1 : 0;
			}
			CreditsFGSpr.setColor(Color(255, 255, 255, CurrentAlpha));
		}
		window.clear();
		window.draw(CreditsBGSpr);
		if (creditsactive) {
			window.draw(CreditsFGSpr);
		}
		window.display();
	}

}CreditScreen;
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
		hud.lives_number_1_sprite.setTextureRect(IntRect(lives_num1_index * (290 / 10), 0, 290 / 10, 29));
		hud.lives_number_2_sprite.setTextureRect(IntRect(lives_num2_index * (290 / 10), 0, 290 / 10, 29));

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
		hud.lives_display();
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
	void lives_display() {
		lives_num1_index = 9 - ((player.num_of_lives / 10) - (player.num_of_lives / 100) * 10);
		lives_num2_index = 9 - (player.num_of_lives % 10);
	}
}hud;

//PowerUps4
struct PowerUps
{
	int pick_power_up = 1 + rand() % 6;
	Sprite powerup_sprite;
	Clock powerup_timer;
	void drop_power_up(vector<PowerUps>& powerups)
	{
		powerup.pick_power_up = 1 + rand() % 6;
		if (player.kill_count % 4 == 0)
		{
			if (pick_power_up == HEALTH_KIT)
			{
				powerup_sprite.setTexture(health_kit_tex);
				powerup_sprite.setTextureRect(IntRect(0, 0, 22, 20));
			}
			else if (pick_power_up == HEALTH_POTION)
			{
				powerup_sprite.setTexture(health_potion_tex);
				powerup_sprite.setTextureRect(IntRect(0, 0, 15, 24));
			}
			else if (pick_power_up == SPEED_POTION)
			{
				powerup_sprite.setTexture(speed_potion_tex);
				powerup_sprite.setTextureRect(IntRect(0, 0, 15, 24));
			}
			else if (pick_power_up == DAMAGE_POTION)
			{
				powerup_sprite.setTexture(damage_potion_tex);
				powerup_sprite.setTextureRect(IntRect(0, 0, 15, 24));
			}
			else if (pick_power_up == RIFLE_AMMO)
			{
				powerup_sprite.setTexture(rifle_ammo_tex);
				powerup_sprite.setTextureRect(IntRect(0, 0, 26, 17));
			}
			else if (pick_power_up == LISER_AMMO)
			{
				powerup_sprite.setTexture(laser_ammo_tex);
				powerup_sprite.setTextureRect(IntRect(0, 0, 22, 20));
			}
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
					player.speed_boost = 2;


				}
				else if (pick_power_up == DAMAGE_POTION)
				{
					damage_boost = 2;

				}
				else if (pick_power_up == RIFLE_AMMO)
				{
					rifle.ammo += 100;
				}
				else if (pick_power_up == LISER_AMMO)
				{
					liser.can_use_laser = 1;
				}
				powerups[i].powerup_sprite.setScale(0, 0);
				player.powerup_is_on = true;
			}
			if (player.powerup_is_on == false) {
				powerups[i].powerup_timer.restart();
			}
			if (powerups[i].powerup_timer.getElapsedTime().asSeconds() > 20 && player.powerup_is_on == true) {
				damage_boost = 1;
				player.speed_boost = 1;
				liser.can_use_laser = 0;
				if (player.gun == LISER) {
					player.gun = PISTOL;
				}
				player.powerup_is_on = false;
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
	float health = 5;
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
						if (abs(player.upperbodySprite.getPosition().x - enemy1[i].rec.getPosition().x) - 10 * i <= ENEMY_PLAYER_SHOOTING_RANGE && player.health > 0)
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
		{//if enemies is dead ->velocity =0
			for (int i = 0; i < 30; i++)
				enemy1[i].velocity.x = 0;
		}
		//move enemies
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
				if (player.kill_count % 4 == 0) {
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
	void reset()
	{
		for (int i = 0; i < 30; i++)
		{
			RS[i].sprite.setTexture(RS[i].runningtex);
			RS[i].sprite.setScale(plScale, plScale);
			RS[i].sprite.setTextureRect(IntRect(0, 0, 36, 48));
			RS[i].sprite.setPosition(RS[i].initial_position, 670);
			RS[i].isCarrying_a_weapon = 0;
			RS[i].is_alive = true;
			RS[i].health = 5;
			RS[i].damage = 1;
			RS[i].sprite_indicator[10] = {  };
			RS[i].is_getting_damaged = false;
			RS[i].death_animation_done = 0;
			RS[i].check = 0;
			RS[i].reversed_direction = 0;
			RS[i].stopped = 0;
			RS[i].shoot_timer = 0;
			RS[i].last_key = RIGHT;
		}

	}
}RS[33];//Rebel Soldier


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

	void setup(Enemy2 enemy2[6])
	{
		for (int i = 0; i < 6; i++)
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
	void Damaged(Enemy2 enemy2[6])
	{
		for (int i = 0; i < 6; i++)
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

			if (player.kill_count % 4 == 0) {
				player.last_enemy_pos.x = s.RWSpr.getPosition().x;
				player.last_enemy_pos.y = s.RWSpr.getPosition().y;
				powerup.drop_power_up(powerups);
			}
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
			for (int i = 0; i < 6; i++)
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

			for (int i = 0; i < 6; i++)
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
	void reset()
	{
		for (int i = 0; i < 6; i++)
		{
			enemy2[i].RWSpr.setTexture(RWTexRun);
			enemy2[i].RWSpr.setScale(plScale, plScale);
			enemy2[i].RWSpr.setTextureRect(IntRect(0, 0, 36, 48));
			enemy2[i].RWSpr.setPosition(RwInitialPos + i * 1500, 750);
			enemy2[i].is_alive = true;
			enemy2[i].health = 10;
			enemy2[i].damage = 0.02 * 3;
			enemy2[i].run_indicator = enemy2[i].attack_indicator = enemy2[i].death_indicator = 0;
			enemy2[i].is_getting_damaged = false;
		}
	}
	void draw(Enemy2 enemy2[6])
	{
		for (int i = 0; i < 6; i++)
			window.draw(enemy2[i].RWSpr);
	}
}enemy2[6];


struct SoundEffectsControl
{
	int selected = 1, soundlevel = 100;
	bool isOpen = false;

	void move_right()
	{
		selected = (selected + 1 <= 7 ? selected + 1 : 7);
		soundlevel = (100 - (selected - 1) * 15 < 11 ? 0 : 100 - (selected - 1) * 15);
		MenuScroll.play();
		rifle.gunvoice.setVolume(soundlevel);
		rifle.sound.setVolume(soundlevel);
		pistol.sound.setVolume(soundlevel);
		liser.sound.setVolume(soundlevel);
		for (int i = 0; i < 30; i++)
		{
			RS[i].deathSound.setVolume(soundlevel);
			RS[i].deathSound2.setVolume(soundlevel);
			RS[i].shootingSound.setVolume(soundlevel);
		}
		TS_BGFireFX.setVolume(soundlevel);
		player.playerdeath.setVolume(soundlevel);
		startvoice.setVolume(soundlevel);
	}
	void move_left()
	{
		selected = (selected - 1 == 0 ? 1 : selected - 1);
		soundlevel = (100 - (selected - 1) * 15 < 11 ? 0 : 100 - (selected - 1) * 15);
		MenuScroll.play();
		rifle.sound.setVolume(soundlevel);
		pistol.sound.setVolume(soundlevel);
		liser.sound.setVolume(soundlevel);
		for (int i = 0; i < 30; i++)
		{
			RS[i].deathSound.setVolume(soundlevel);
			RS[i].deathSound2.setVolume(soundlevel);
			RS[i].shootingSound.setVolume(soundlevel);
		}
		TS_BGFireFX.setVolume(soundlevel);
		player.playerdeath.setVolume(soundlevel);
		startvoice.setVolume(soundlevel);
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
			SoundEffectsControlSpr.setTextureRect(IntRect((selected - 1) * 755, 0, 755, 240));
			window.draw(TS_BGSpr);
			window.draw(TS_TandGSpr);
			window.draw(TS_LSpr);
			window.draw(TS_OlSpr);
			window.draw(SoundEffectsControlSpr);
			window.display();
		}

		else
		{
			TS_OlSpr.setScale(3 * view.getSize().x / 1920.0, 3 * view.getSize().y / 1080.0);
			TS_OlSpr.setPosition(view.getCenter().x - (1000 * view.getSize().x / 1920), view.getCenter().y - (600 * view.getSize().y / 1080.0));
			SoundEffectsControlSpr.setScale(view.getSize().x / 1920.0, view.getSize().y / 1080.0);
			SoundEffectsControlSpr.setPosition(view.getCenter().x - (400 * view.getSize().x / 1920.0), view.getCenter().y - (150 * view.getSize().y / 1080.0));

			SoundEffectsControlSpr.setTextureRect(IntRect((selected - 1) * 755, 0, 755, 240));
			window.clear();
			window_draw();
			window.draw(TS_OlSpr);
			window.draw(SoundEffectsControlSpr);
			window.display();

		}

	}
}SoundEffectsControl;


Clock push_timer;
float delay = 0.3;

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
	int health = 5;
	int damage = 1;
	bool check = 1;
	vector <pair <RectangleShape, int>> bullet; //enemy3 pistol
	vector<Sprite>bulletssprites;
	int cnt = 0; //to cnt 

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
				if (player.kill_count % 4 == 0) {
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
				new_enemy.cnt++;
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
	bool is_done(vector <Enemy3>& enemy3)
	{
		int a = 0;
		for (int i = 0; i < enemy3.size(); i++)
		{
			if (!enemy3[i].is_alive)
				a++;
		}

		if (a == 20)
			return true;
		else
			return false;


	}
	void reset(vector<Enemy3>& enemy3)
	{
		for (int i = 0; i < enemy3.size(); i++)
		{
			enemy3[i].isCarrying_a_weapon = 0;
			enemy3[i].stopped = 0;
			enemy3[i].shoot_timer = 0;
			enemy3[i].last_key = RIGHT;
			enemy3[i].death_animation_done = 0;
			enemy3[i].is_getting_damaged = 0;
			enemy3[i].sprite_indicator[i] = 0;
			enemy3[i].is_alive = 0;
			enemy3[i].check = 0;
			enemy3[i].damage = 0;
			enemy3[i].health = 5;
		}
		push_timer.restart();
		cnt = 0;
		delay = 0.3;
		if (!enemy3.empty())
			enemy3.clear();
	}
}new_enemy;
vector<Enemy3>enemy3;
void new_enemy_setup()
{
	new_enemy.rec.setSize(Vector2f(100, 130));
	new_enemy.rec.setPosition(Vector2f(19650, 830));
	new_enemy.sprite.setTexture(RS[0].runningtex);
	new_enemy.sprite.setScale(plScale, plScale);
	new_enemy.sprite.setTextureRect(IntRect(0, 0, 312 / 12, 40));
	new_enemy.sprite.setPosition(Vector2f(19650, 830));
}
struct FINAL_BOSS
{
	Texture idletex, runningtex, fightingtex1, fightingtex2, deadtex, done_T, running_attacktex;
	float health = 10, damage = 0, sprite_indicator = 0.0;
	Vector2f velocity = { 0,0 };
	Vector2f initialposition = { 26000,550 };
	bool live = 1, deaths_animation_done = 0, is_alive = 1, is_getting_damaged = 0;
	Sprite sprite, done;
	bool can_run = 0, stopped = 0, death_animation_done = 0;
	bool can_fight = 0;
	Clock damage_timer;
	float animation_indicator[10] = {};
	RectangleShape hitbox;
	SoundBuffer win_b, short_attack_B, long_attack_B;
	Sound win, short_attack, long_attack;
	bool comlete = 0, runnning_attack_done = 0;;

	void setup(FINAL_BOSS& boss)
	{
		hitbox.setSize(Vector2f(190, 180));

		boss.idletex.loadFromFile("idle.png");
		boss.deadtex.loadFromFile("death.png");
		boss.fightingtex1.loadFromFile("Normal Attack.png");
		boss.fightingtex2.loadFromFile("long range attack.png");
		boss.runningtex.loadFromFile("moving.png");
		boss.running_attacktex.loadFromFile("running attack.png");
		boss.done_T.loadFromFile("Mission Complete.png");
		boss.done.setTexture(boss.done_T);

		boss.win_b.loadFromFile("Mission Complete Sound.wav");
		boss.short_attack_B.loadFromFile("Fire Attack Short.wav");
		boss.long_attack_B.loadFromFile("Fire Attack Long.wav");
		boss.win.setBuffer(win_b);
		boss.short_attack.setBuffer(boss.short_attack_B);
		boss.long_attack.setBuffer(boss.long_attack_B);

		boss.sprite.setPosition(boss.initialposition);
		boss.sprite.setTexture(boss.idletex);
		boss.sprite.setScale(-3.25, 3.25);

	}
	void status(FINAL_BOSS& boss)
	{
		if (bgCounter == LEVEL_1_D_BG && boss.live)
		{

			boss.damaged(boss);
			if (!boss.stopped)
			{
				cout << "WORKING? " << boss.can_run << '\n';
				if (abs(player.upperbodySprite.getPosition().x - boss.sprite.getPosition().x) < 1000)
					boss.can_run = 1;
				(abs(player.upperbodySprite.getPosition().x - boss.sprite.getPosition().x) < 350 && player.Velocity.x == 0) ? boss.can_fight = 1 : boss.can_fight = 0;
				if (boss.can_run)
					(boss.can_fight) ? boss.fighting(boss) : boss.running(boss);
				else
				{
					cout << "IDLE\n";
					boss.idle(boss);
				}
				boss.sprite.move(boss.velocity);
			}
		}
	}
	void damaged(FINAL_BOSS& boss)
	{
		if (boss.is_alive)
		{

			for (int j = 0; j < pistol.rects.size(); j++)
			{
				if (boss.sprite.getGlobalBounds().intersects(pistol.rects[j].first.getGlobalBounds()) && pistol.rects[j].second.first != 0)
				{
					boss.velocity.x = 0;
					boss.health -= pistol.damage;
					boss.is_getting_damaged = 1;
					pistol.rects[j].second.first = 0;
				}
			}
			for (int j = 0; j < rifle.rects.size(); j++)
			{
				if (boss.sprite.getGlobalBounds().intersects(rifle.rects[j].first.getGlobalBounds()) && rifle.rects[j].second.first != 0)
				{
					boss.velocity.x = 0;
					boss.health -= rifle.damage;
					boss.is_getting_damaged = 1;
					rifle.rects[j].second.first = 0;
				}
			}
			for (int j = 0; j < liser.rects.size(); j++)
			{
				if (boss.sprite.getGlobalBounds().intersects(liser.rects[j].first.getGlobalBounds()) && liser.rects[j].second != 0)
				{
					boss.velocity.x = 0;
					boss.health -= liser.damage;
					boss.is_getting_damaged = 1;
					liser.rects[j].second = 0;
				}
			}
			if (boss.is_getting_damaged == 1)  // this adds red color to enemies when damaged
			{
				if (boss.damage_timer.getElapsedTime().asMilliseconds() <= 300) {
					boss.sprite.setColor(Color::Red);
				}
				else
				{
					boss.is_getting_damaged = 0;
				}
			}
			else
			{
				boss.sprite.setColor(Color::White);
				boss.damage_timer.restart();
			}
			if (boss.health <= 0)
			{
				boss.velocity.x = 0;
				boss.Death_animation(boss);
			}
		}
		if (player.holding_knife && player.rec.getGlobalBounds().intersects(boss.sprite.getGlobalBounds()))
		{
			boss.health -= 0.3 / 2;
			boss.is_getting_damaged = true;
		}

	}
	void Death_animation(FINAL_BOSS& boss)
	{

		boss.stopped = 1;
		if (!boss.death_animation_done)
		{
			boss.hitbox.setScale(0, 0);
			boss.velocity.x = 0;
			boss.sprite.setTexture(boss.deadtex);
			boss.sprite.setOrigin(3960 / 33 / 2, -5);
			boss.animation_indicator[4] += 0.2;
			if (boss.animation_indicator[4] > 33)
			{
				player.score += 10000;
				boss.death_animation_done = 1;
			}
			boss.sprite.setTextureRect(IntRect(int(boss.animation_indicator[4]) * 3960 / 33, 0, 3960 / 33, 64));
		}
		else
		{

			boss.is_alive = 0;
			//death sound here 
		}
	}
	void fighting2(FINAL_BOSS& boss)//long range 
	{

		boss.sprite.setOrigin(3300 / 11 / 4, -8);
		boss.velocity.x = 0;
		if (player.upperbodySprite.getPosition().x > boss.sprite.getPosition().x)
		{
			boss.sprite.setScale(3.25, 3.25);
		}
		else
		{
			boss.sprite.setScale(-3.25, 3.25);
		}

		boss.sprite.setTexture(boss.fightingtex2);
		EnemiAnimation(boss.sprite, 10.9, 3300 / 11, 63, 0.008, boss.animation_indicator[3]);
		boss.long_attack.play();

	}
	void fighting(FINAL_BOSS& boss)//melee
	{
		boss.velocity.x = 0;
		boss.sprite.setOrigin((3300 / 11) / 2, 0);
		if (player.upperbodySprite.getPosition().x > boss.sprite.getPosition().x)
		{
			boss.sprite.setScale(3.25, 3.25);
			hitbox.setPosition(boss.sprite.getPosition().x - 90, boss.sprite.getPosition().y + 50);
		}
		else
		{
			boss.sprite.setScale(-3.25, 3.25);
			hitbox.setPosition(boss.sprite.getPosition().x - 90, boss.sprite.getPosition().y + 50);
		}
		boss.sprite.setTexture(boss.fightingtex1);
		EnemiAnimation(boss.sprite, 20.9, 4200 / 21, 74, 0.008, boss.animation_indicator[2]);
		boss.short_attack.play();

	}
	void runningattack(FINAL_BOSS& boss)
	{
		boss.sprite.setOrigin(3600 / 18 / 2, 0);
		if (!boss.runnning_attack_done)
		{
			if (player.upperbodySprite.getPosition().x > boss.sprite.getPosition().x)
			{
				boss.velocity.x = 12;
				boss.sprite.setScale(3.25, 3.25);
			}
			else
			{
				boss.velocity.x = -12;
				boss.sprite.setScale(-3.25, 3.25);
			}
			boss.runnning_attack_done = 1;
		}
		boss.sprite.setTexture(boss.running_attacktex);
		EnemiAnimation(boss.sprite, 17.9, 3600 / 18, 98, 0.008, boss.animation_indicator[6]);


	}
	void running(FINAL_BOSS& boss)
	{
		if (boss.stopped == 1)
			return;

		if (player.upperbodySprite.getPosition().x > boss.sprite.getPosition().x)
		{
			//move to right
			boss.velocity.x = 5;
			boss.sprite.setScale(3.25, 3.25);
		}
		else
		{   //move to left
			boss.velocity.x = -5;
			boss.sprite.setScale(-3.25, 3.25);
		}
		boss.sprite.setOrigin(1000 / 10 / 2, 0);
		boss.sprite.setTexture(boss.runningtex);
		EnemiAnimation(boss.sprite, 9.9, 1000 / 10, 73, 0.008, boss.animation_indicator[1]);
		boss.sprite.move(velocity);
	}
	void idle(FINAL_BOSS& boss)
	{
		boss.sprite.setOrigin(856 / 8 / 2, 0);
		boss.sprite.setTexture(boss.idletex);
		EnemiAnimation(boss.sprite, 7.9, 856 / 8, 50, 0.05, boss.animation_indicator[0]);

	}
}boss;
void call()
{
	if (bgCounter == LEVEL_1_C_BG)
	{
		if (push_timer.getElapsedTime().asSeconds() * delay > 2 && enemy3.size() < 20)
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
					cout << new_enemy.cnt << endl;
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
	boss.setup(boss);
	file.load();
	CreditScreen.creditsSetup();
	LeaderBoard.setup();
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
	file.save();
	return 0;
}
//DEFINITIONS

void Menu()
{
	while (window.isOpen())
	{

		if (NewGameScreen.is_open)
		{
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::KeyPressed)
				{

					if (event.key.code == sf::Keyboard::Backspace)
						NewGameScreen.name.pop_back();
					else if (event.key.code == sf::Keyboard::Enter && NewGameScreen.name.size() > 2)
					{
						if (names_save.size() < 10)
						{
							current_name = NewGameScreen.name;
							names_save.push_back({ NewGameScreen.name, "0" });
							NewGameScreen.is_open = false;
							menu.game_started = true;
						}
					}
					else
						NewGameScreen.name += key_code(event.key.code);
				}
				if (event.type == Event::Closed)
				{
					window.close();
				}
			}
		}
		else
			windowclose();
		mouse_pos();
		if (player.isdead)
		{
			if (player.num_of_lives == 0)
			{
				DeathScreen.main_screen_draw();

				if (DeathScreen.selected == 1 && Keyboard::isKeyPressed(Keyboard::Enter))
				{
					RS->reset();
					enemy2->reset();
					new_enemy.reset(enemy3);
					LoadGameScreen.isOpen = true;
					player.isdead = false;
					menu.game_started = false;
					player.live = true;
					player.health = 100;
					player.num_of_lives = 3;
					player.upperbodySprite.setPosition(0, 600);
					player.lowerbodySprite.setPosition(0, 600);
					view.setCenter(960, 540);
					window.setView(view);
					bgCounter = LEVEL_1_A_BG;
					rifle.ammo = 200;
					player.score = 0;
					this_thread::sleep_for(chrono::milliseconds(100));



				}
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
				if (DeathScreen.selected == 2 && Keyboard::isKeyPressed(Keyboard::Enter))
				{
					MenuClick.play();
					window.close();
				}
			}
			else
			{
				player.isdead = false;
				player.health = 100;
				MenuClick.play();
				timer.restart();
				DeathScreen.is_active = false;
				player.score = 0;
				rifle.ammo = 200;
				if (bgCounter == LEVEL_1_A_BG) {
					player.upperbodySprite.setPosition(0, 600);
					player.lowerbodySprite.setPosition(0, 600);
					RS[0].reset();

				}
				else if (bgCounter == LEVEL_1_B_BG) {
					isMoved = 0;
					enemy2[0].reset();
				}
				else if (bgCounter == LEVEL_1_C_BG) {
					ismoved2 = 0;
					new_enemy.reset(enemy3);
				}
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
					if (!menu.options_screen.music_slider.isOpen && !SoundEffectsControl.isOpen)
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
					if (Keyboard::isKeyPressed(Keyboard::Enter) && menu.options_screen.selected == 2 && timer4.getElapsedTime().asMilliseconds() > 200 || SoundEffectsControl.isOpen)
					{
						timer4.restart();
						SoundEffectsControl.draw();
						if (!SoundEffectsControl.isOpen)
						{
							SoundEffectsControl.isOpen = true;
							MenuClick.play();
						}

						if (Keyboard::isKeyPressed(Keyboard::Right) && timer.getElapsedTime().asMilliseconds() > 200)
						{
							timer.restart();
							SoundEffectsControl.move_left();
						}
						if (Keyboard::isKeyPressed(Keyboard::Left) && timer.getElapsedTime().asMilliseconds() > 200)
						{
							timer.restart();
							SoundEffectsControl.move_right();
						}
						if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
						{
							MenuReturn.play();
							escTimer.restart();
							SoundEffectsControl.isOpen = false;
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
					if (!menu.start_selected && !menu.options_screen.isOpen && !NewGameScreen.is_open && !LoadGameScreen.isOpen && !CreditScreen.isOpen)
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
								if (!NewGameScreen.is_open && !LoadGameScreen.isOpen)
									menu.start_screen.draw();
								if (!menu.start_selected)
								{
									MenuClick.play();
									menu.start_selected = true;
									timer.restart();

								}
								if (menu.start_screen.selected == 1 && Keyboard::isKeyPressed(Keyboard::Enter) && timer.getElapsedTime().asMilliseconds() > 200)
								{
									current_name = last_name;
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
								if (menu.start_screen.selected == 3 && Keyboard::isKeyPressed(Keyboard::Enter) && timer4.getElapsedTime().asMilliseconds() > 300 || LoadGameScreen.isOpen)
								{
									if (!LoadGameScreen.isOpen)
										MenuClick.play();
									if (!LoadGameScreen.isOpen)
									{
										LoadGameScreen.isOpen = true;
										timer.restart();
									}
									//timer.restart();
									LoadGameScreen.draw();
									timer4.restart();
									if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 300)
									{
										escTimer.restart();
										LoadGameScreen.isOpen = false;
										this_thread::sleep_for(chrono::milliseconds(150));
									}
									if (Keyboard::isKeyPressed(Keyboard::Up) && timer.getElapsedTime().asMilliseconds() > 200)
									{
										LoadGameScreen.move_up();
										timer.restart();
									}
									if (Keyboard::isKeyPressed(Keyboard::Down) && timer.getElapsedTime().asMilliseconds() > 200)
									{
										LoadGameScreen.move_down();
										timer.restart();
									}
									if (Keyboard::isKeyPressed(Keyboard::Enter) && timer.getElapsedTime().asMilliseconds() > 200)
									{
										current_name = names_save[LoadGameScreen.selected - 1].first;
										menu.game_started = true;
										LoadGameScreen.isOpen = false;
										timer.restart();
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
								if (!menu.options_screen.music_slider.isOpen && !SoundEffectsControl.isOpen)
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
								if (Keyboard::isKeyPressed(Keyboard::Enter) && menu.options_screen.selected == 2 && timer4.getElapsedTime().asMilliseconds() > 200 || SoundEffectsControl.isOpen)
								{
									timer4.restart();
									SoundEffectsControl.draw("main");
									if (!SoundEffectsControl.isOpen)
									{
										SoundEffectsControl.isOpen = true;
										MenuClick.play();
									}

									if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
									{
										MenuReturn.play();
										escTimer.restart();
										SoundEffectsControl.isOpen = false;
									}

									if (Keyboard::isKeyPressed(Keyboard::Right) && timer.getElapsedTime().asMilliseconds() > 200)
									{
										timer.restart();
										SoundEffectsControl.move_left();
									}
									if (Keyboard::isKeyPressed(Keyboard::Left) && timer.getElapsedTime().asMilliseconds() > 200)
									{
										timer.restart();
										SoundEffectsControl.move_right();
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
							else if (menu.selected == CREDITS && Keyboard::isKeyPressed(Keyboard::Enter) || CreditScreen.isOpen)
							{
								if (!CreditScreen.isOpen)
								{
									CreditsTClock.restart();
									currentF = 0;
									currentR = 0;
									numF = 12;
									creditsactive = 1;
								}
								CreditScreen.isOpen = true;
								CreditScreen.creditsAnimation();


								if (!creditsactive)
								{
									CreditScreen.isOpen = false;
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

	//lvl 1 d bg
	bgTexture[4].loadFromFile(pathh + "Final Level BG upscaled2.png");
	bgSprite[4].setTexture(bgTexture[4]);
	bgSprite[4].setPosition(22000, 0);

	//lvl 1 d fg
	lvl1FGtex[4].loadFromFile(pathh + "Final Level FG upscaled2.png");
	Lvl1FG[4].setTexture(lvl1FGtex[4]);
	Lvl1FG[4].setPosition(22000, 0);
	create(ground, 8, 5120, 20, 22000, 800);



}
void windowfunction()
{

	//delta time
	dt = clock_pl.getElapsedTime().asMicroseconds();
	dt /= 750;
	clock_pl.restart();

	liser.restart();

	boss.status(boss);

	if (Keyboard::isKeyPressed(Keyboard::X))
		player.isdead = true;
	if (Keyboard::isKeyPressed(Keyboard::Escape) && escTimer.getElapsedTime().asMilliseconds() > 200)
	{
		MenuReturn.play();
		pause_menu.is_paused = true;
		escTimer.restart();
	}
	if (Keyboard::isKeyPressed(Keyboard::Q) && rifle.ammo > 0 && ((player.gun == PISTOL) || (player.gun == LISER)))
	{
		player.gun = RIFLE;
		rifle.gunvoice.play();
	}
	else if (Keyboard::isKeyPressed(Keyboard::W) && ((player.gun == RIFLE) || (player.gun == LISER)))
	{
		player.gun = PISTOL;
	}
	else if (Keyboard::isKeyPressed(Keyboard::E) && ((player.gun == PISTOL) || (player.gun == RIFLE)) && liser.can_use_laser)
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


}
void windowclose()
{
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
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
		if (!ismoved3)
		{
			player.upperbodySprite.setPosition(22100, 600);
			player.lowerbodySprite.setPosition(22100, 600);
			full_time_played += hud.hud_time.getElapsedTime().asSeconds();
			hud.hud_time.restart();
			ismoved3 = true;
			view.setSize(2000, 1120);
		}
		leftEnd = 22000;
		rightEnd = 22000 + 5120;
		//area where no black edges can appear
		if (!cutscene && player.upperbodySprite.getPosition().x > leftEnd + 950)
		{
			FBCutScene();
			cutscene = true;
		}
		else
		{
			if (player.upperbodySprite.getPosition().x <= 22000 + 5120 - (1250) && player.upperbodySprite.getPosition().x >= 22000 + (1250))
			{
				view.setCenter(player.upperbodySprite.getPosition().x, 560);  //camera focus on player
			}
			//area where  black edge appear from right
			else if (player.upperbodySprite.getPosition().x > 22000 + 5120 - (1250))  //camera stop
			{
				view.setCenter(22000 + 5120 - (1250), 560);
			}
			//area where  black edge appear from left
			else if (player.upperbodySprite.getPosition().x < 22000 + (1250))
			{
				view.setCenter(22000 + (1250), 560);  //camera stop
			}
		}
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
	else if (player.upperbodySprite.getPosition().x > rightEnd && bgCounter == 2) {
		transition();
		transition_reverse();
		bgCounter = 3;
		this_thread::sleep_for(chrono::milliseconds(300));
	}
	else if (player.upperbodySprite.getPosition().x > rightEnd && bgCounter == 3 || game_ended)
	{
		game_ended = true;
		if (!LeaderBoard.is_compared)
		{
			names_save[names_save.size() - 1].second = to_string(player.score);
			LB_timer.restart();
			LeaderBoard.compare();
			LeaderBoard.is_compared = true;
			LeaderBoard.leaderboard_show();
		}

		if (LB_timer.getElapsedTime().asSeconds() < 10)
			LeaderBoard.draw();
		else
		{
			RS->reset();
			enemy2->reset();
			new_enemy.reset(enemy3);
			LoadGameScreen.isOpen = false;
			player.isdead = false;
			menu.game_started = false;
			player.live = true;
			player.health = 100;
			player.num_of_lives = 3;
			player.upperbodySprite.setPosition(0, 600);
			player.lowerbodySprite.setPosition(0, 600);
			view.setCenter(960, 540);
			window.setView(view);
			bgCounter = LEVEL_1_A_BG;
			game_ended = false;
			this_thread::sleep_for(chrono::milliseconds(100));
		}
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
		if (!cuton)
		{
			move_with_animation(s, maxframe, x, y, delay, index);
			if (player.gun == PISTOL)
				move_with_animation(player.upperbodySprite, maxframe, x, y, delay, index);
			else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
				move_with_animation(player.upperbodySprite, 11.9, 528 / 11, 29, delay, 32);
		}

	}

	//move player
	if (player.gun == PISTOL)
	{
		if (!pause_menu.is_paused)
		{
			s.move(player.Velocity.x * (player.speed_boost), player.Velocity.y * player.speed_boost);
			player.upperbodySprite.move(player.Velocity.x * (player.speed_boost), player.Velocity.y * (player.speed_boost));
		}
	}
	else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
	{
		if (!pause_menu.is_paused)
		{
			s.move(player.Velocity.x * (player.speed_boost) * 0.7, player.Velocity.y * player.speed_boost);
			player.upperbodySprite.move(player.Velocity.x * (player.speed_boost) * 0.7, player.Velocity.y * (player.speed_boost));
		}
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
				if (player.gun == RIFLE && rifle.ammo > 0)
					rifle.shooting(rifle);

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
				if (player.gun == PISTOL)
				{
					player.upperbodySprite.setTexture(PTexRPU);
					player.lowerbodySprite.setTexture(PTexRPL);
					animation(s, maxframe, x, y, delay, index);
				}
				else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
				{
					player.upperbodySprite.setTexture(PTexRRU);
					player.lowerbodySprite.setTexture(PTexRRL);
					animation(player.upperbodySprite, 11.9, 528 / 12, 29, 0.004, 32);
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
				if (player.gun == RIFLE && rifle.ammo > 0)
					rifle.shooting(rifle);

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
				if (player.gun == PISTOL)
				{
					player.upperbodySprite.setTexture(PTexRPU);
					player.lowerbodySprite.setTexture(PTexRPL);
					animation(s, maxframe, x, y, delay, index);
				}
				else if (player.gun == RIFLE || player.gun == LISER)
				{
					player.upperbodySprite.setTexture(PTexRRU);
					player.lowerbodySprite.setTexture(PTexRPL);
					animation(player.upperbodySprite, 11.9, 528 / 12, 29, 0.004, 32);
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
					player.upperbodySprite.setTexture(PTexSSPU);
					// animation(player.upperbodySprite, 9.9, 520 / 10, 41, pistolshooting_delay, 10);
					animiindecator[10] += 0.01 * dt;
					if (animiindecator[10] > 10)
					{
						pistol.shooting(pistol);
						animiindecator[10] = 0;
					}

					player.upperbodySprite.setTextureRect(IntRect(int(animiindecator[10]) * 52, 0, 52, 41));
				}
				else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
				{
					player.upperbodySprite.setTexture(PTexSSRU);
					animation(player.upperbodySprite, 3.9, 240 / 4, 27, rifleshooting_delay, 10);
				}

				if (player.gun == RIFLE && rifle.ammo > 0)
					rifle.shooting(rifle);
				if (player.gun == LISER)
					liser.shooting(liser);

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
	for (int i = 0; i < 5; i++)
	{
		if (i == 1)continue;
		window.draw(Lvl1FG[i]);
	}
	for (int i = 0; i < powerups.size(); i++)
	{
		window.draw(powerups[i].powerup_sprite);
	}
	if (!cuton)
		hud.draw(hud);
	enemy2->draw(enemy2);
	if (boss.live)
		window.draw(boss.sprite);
	if (boss.comlete == 1)
	{
		window.draw(boss.done);
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
	if (player.gun == PISTOL)
	{
		player.upperbodySprite.setTexture(PTexSSPU);
		animiindecator[10] += 0.01 * dt;
		if (animiindecator[10] > 9.9)
		{
			pistol.shooting(pistol);
			animiindecator[10] = 0;
		}
		player.upperbodySprite.setTextureRect(IntRect(int(animiindecator[10]) * 520 / 10, 0, 520 / 10, 41));
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
		if (player.gun == PISTOL)
		{
			player.lowerbodySprite.setTexture(PTexSCPL);
			animiindecator[14] += 0.01 * dt;
			if (animiindecator[14] > 9.9)
			{
				animiindecator[14] = 0;
				pistol.shooting(pistol);
			}
			player.lowerbodySprite.setTextureRect(IntRect(int(animiindecator[14]) * 520 / 10, 0, 520 / 10, 29));

		}
		else if ((player.gun == RIFLE && rifle.ammo > 0) || player.gun == LISER)
		{
			player.lowerbodySprite.setTexture(PTexSCRL);
			animation(player.lowerbodySprite, 3.9, 268 / 4, 28, pistolshooting_delay, 14);
		}

		if (player.gun == RIFLE && rifle.ammo > 0)
		{
			rifle.shooting(rifle);
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
		if (player.gun == PISTOL)
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
	if (player.gun == PISTOL)
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
		if (player.upperbodySprite.getGlobalBounds().intersects(boss.hitbox.getGlobalBounds()))
		{
			player.health -= boss.damage;
			player.is_getting_damaged = 1;
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
			--player.num_of_lives;
			player.isdead = 1;
		}
		if (player.pl_death_ctr >= 0.2 && player.pl_death_ctr < 0.5)
		{
			//sound
			player.playerdeath.play();
		}
		if (!player.isdead)
			player.lowerbodySprite.setTextureRect(IntRect(int(player.pl_death_ctr) * 480 / 10, 0, 480 / 10, 44));
	}
	else if (player.num_of_lives == 0)
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
	TS_BGTheme.setVolume(70.f);
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
	TS_OlSpr.setScale(5, 3);

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

	LeaderBoardTex.loadFromFile("Leaderboard BG Sprite Sheet.png");
	LeaderBoardSpr.setTexture(LeaderBoardTex);
	LeaderBoardSpr.setScale(1, 1.25);

	LeaderBoardSheetTex.loadFromFile("Leaderboard Sprite.png");
	LeaderBoardSheetSpr.setTexture(LeaderBoardSheetTex);

	SoundEffectsControlSpr.setPosition(580, 289);
	SoundEffectsControlSpr.setTexture(MusicControlTex);
	SoundEffectsControlSpr.setTextureRect(IntRect(0, 0, 755, 240));


	LoadGameSpr.setPosition(-12, 100);
	LoadGameTex.loadFromFile("Load game.png");
	LoadGameSpr.setTexture(LoadGameTex);

	LoadSaveText.setFont(nameFont);
	LoadSaveText.setCharacterSize(54);
}
void texture_setup()//&buffers setup
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
	laser_ammo_tex.loadFromFile(pathh + "Laser Icon.png");
	startvoice_B.loadFromFile("Mission 1 Start.wav");
	startvoice.setBuffer(startvoice_B);
	startvoice.setVolume(60);
}

void FBCutScene()
{

	cuton = true;
	Clock t1, t2;
	float ctr = 20;
	while (t2.getElapsedTime().asSeconds() <= 1)
	{
		window.clear();

		window_draw();
		boss.idle(boss);
		view.setCenter(boss.sprite.getPosition().x, 550);
		window.setView(view);


		window.display();
	}
	t1.restart();
	while (t1.getElapsedTime().asSeconds() <= 2.5)
	{
		window.clear();

		window_draw();
		boss.idle(boss);
		view.setCenter(boss.sprite.getPosition().x - ctr, 550);
		window.setView(view);
		window.display();
		ctr += 20;
	}
	cuton = false;

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
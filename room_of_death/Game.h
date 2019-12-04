// @Author Michael Rainsford Ryan
#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Declare variables
const int numBullets = 10;
const int numOfEnemies = 30;
const int numOfWindows = 8;

enum class GameStates
{
	MainMenu,
	GameOn,
	DeathScreen,
	PauseScreen,
};

// Initialise the zombie class
class Zombie
{
public:
	sf::Sprite sprite;
	sf::CircleShape collisionCircle;
	int health;
	int respawnCounter;
	bool alive;
};

// Initialise the bullet class
class Bullet
{
public:
	sf::Sprite sprite;
	sf::Vector2f velocity;
	bool active;
};

class Player
{
public:
	sf::Sprite sprite;
	sf::CircleShape collisionCircle;
	int health;
};

class Window
{
public:
	sf::Sprite sprite;
	sf::Vector2f spawnPosition;
	int health;
};

class Game
{
public:
	Game();
	~Game();

	void run();
private:
	void respawnEnemy(Zombie & t_enemy);
	float signOf(float t_value);
	void screenShake(sf::RenderWindow & t_window, sf::Vector2f t_position, int t_shakeMagnitude, int & t_counter);
	void setupGame();
	void update();
	void gamePlaying();
	void render();

	//create Window
	sf::RenderWindow m_window;

	GameStates gameState;

	sf::RectangleShape m_onScreenArea{ sf::Vector2f{ 1080.0f, 700.0f } };
	
	// Declare variables
	bool readyToFire = false;

	bool m_fired = false;

	int waitToFireCounter = 30;
	int score = 0;
	int screenShakeCounter = 0;

	float playerRotation = 0;

	// Declare sprites and shapes
	Player player;
	Zombie enemies[numOfEnemies];
	Bullet bullets[numBullets];
	Window m_entranceWindows[numOfWindows];

	sf::Sprite backgroundSprite;
	sf::Sprite backgroundWallSprite;
	sf::Sprite backgroundBackWallSprite;
	sf::Sprite bloodSplatSprite;

	sf::Sprite menuScreenSprite;

	// Declare vectors
	sf::Vector2f playerVelocity{ 0,0 };
	sf::Vector2f playerLookDirection{ 0,-1 };
	sf::Vector2f playerPosition{ 400.0f, 300.0f };

	// Declare textures
	sf::Texture playerTexture;
	sf::Texture bulletTexture;
	sf::Texture zombieTexture;
	sf::Texture m_windowBoardsTexture;

	sf::Texture backgroundTexture;
	sf::Texture backgroundWallTexture;
	sf::Texture backgroundBackWallTexture;
	sf::Texture bloodSplatTexture;

	sf::Texture menuScreenTexture;

	// Setup music
	sf::Music battleMusic;

	// Setup sound buffer
	sf::SoundBuffer fireBuffer;
	sf::SoundBuffer zombieBuffer;
	sf::SoundBuffer deathSoundBuffer;
	sf::SoundBuffer deathSplatBuffer;
	sf::SoundBuffer painBuffer;

	// Setup sound
	sf::Sound fireSound;
	sf::Sound snakeDeath;
	sf::Sound deathSound;
	sf::Sound deathSplatSound;
	sf::Sound painSound;

	// Setup font
	sf::Font impactFont;

	// Setup text
	sf::Text endGameText;
	sf::Text scoreText;
	sf::Text healthText;
};

#endif // !GAME

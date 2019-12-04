// arrays of bullets and arrays of apples with bulletvecelocities array
// @Author Michael Rainsford Ryan
// @Date 07/12/2018
// C00239510
// Session 1: 07/12/2018 - Approx 1 to 1.30 hours
// Session 2: 11/12/2018 - Approx 4 to 5 hours
// Session 3: 12/12/2018 - Aprrox 2 hours adding the Game class

#include "Game.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h> 

Game::Game()	:
	m_window(sf::VideoMode(800, 600), "Game"),
	gameState{ GameStates::MainMenu }
{
	setupGame();
}

Game::~Game()
{

}

void Game::run()
{
	// set timePerFrame to 1 60th of a second. 60 frames per second
	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	// the clock object keeps the time.
	sf::Clock clock;
	clock.restart();

	while (m_window.isOpen())
	{
		// check if the close window button is clicked on.
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_window.close();
		}

		//add to the time since last update and restart the clock
		timeSinceLastUpdate += clock.restart();

		//update every 60th of a secound
		//only when the time since last update is greater than 1/60 update the world.

		if (timeSinceLastUpdate > timePerFrame)
		{
			update();

			// reset the timeSinceLastUpdate to 0 
			timeSinceLastUpdate = sf::Time::Zero;
		}
	}
}

// Main setup loop for the game
void Game::setupGame()
{
	// Load textures
	if (!playerTexture.loadFromFile("ASSETS/SPRITES/player.png")) { /* Error */ }
	if (!bulletTexture.loadFromFile("ASSETS/SPRITES/bullet.png")) { /* Error */ }
	if (!zombieTexture.loadFromFile("ASSETS/SPRITES/zombie.png")) { /* Error */ }

	if (!backgroundTexture.loadFromFile("ASSETS/SPRITES/floorboards.png")) { /* Error */ }
	if (!backgroundWallTexture.loadFromFile("ASSETS/SPRITES/wall1.png")) { /* Error */ }
	if (!backgroundBackWallTexture.loadFromFile("ASSETS/SPRITES/backWall1.png")) { /* Error */ }
	if (!bloodSplatTexture.loadFromFile("ASSETS/SPRITES/bloodSplat.png")) { /* Error */ }
	if (!m_windowBoardsTexture.loadFromFile("ASSETS/SPRITES/boards.png")) { /* Error */ }

	if (!menuScreenTexture.loadFromFile("ASSETS/SPRITES/title.png")) { /* Error */ }

	// Setup the sprites

	// Player
	player.sprite.setTexture(playerTexture);
	player.sprite.setTextureRect(sf::IntRect{ 0,0,100,96 });
	player.sprite.setPosition(400.0f, 300.0f);
	player.sprite.setOrigin(player.sprite.getGlobalBounds().width / 2 - 20, player.sprite.getGlobalBounds().height / 2);
	player.sprite.setScale(0.6f, 0.6f);

	player.health = 10;

	player.collisionCircle.setRadius(25);
	player.collisionCircle.setOrigin(25, 25);
	player.collisionCircle.setPosition(player.sprite.getPosition());

	// Background
	backgroundSprite.setTexture(backgroundTexture);


	// Background wall
	backgroundWallSprite.setTexture(backgroundWallTexture);
	backgroundWallSprite.setPosition(-45.0f, -59.0f);

	// Background back wall
	backgroundBackWallSprite.setTexture(backgroundBackWallTexture);
	backgroundBackWallSprite.setPosition(0.0f, -59.0f);

	// blood splat
	bloodSplatSprite.setTexture(bloodSplatTexture);

	// Title screen
	menuScreenSprite.setTexture(menuScreenTexture);

	// Setup the entrance windows
	for (int i = 0; i < numOfWindows; i++)
	{
		m_entranceWindows[i].sprite.setTexture(m_windowBoardsTexture);
		m_entranceWindows[i].sprite.setTextureRect(sf::IntRect{ 0, 0, 39, 40 });

		if (i == 0)
		{
			m_entranceWindows[i].sprite.setPosition(266, -46);
		}
	}

	for (int i = 0; i < numOfEnemies; i++)
	{
		enemies[i].sprite.setTexture(zombieTexture);
		enemies[i].sprite.setTextureRect(sf::IntRect{ 0, 0, 175, 161 });
		enemies[i].sprite.setOrigin(enemies[i].sprite.getGlobalBounds().width / 2, enemies[i].sprite.getGlobalBounds().height / 2);
		enemies[i].sprite.setScale(0.3f, 0.3f);

		enemies[i].collisionCircle.setRadius(20.0f);
		enemies[i].collisionCircle.setOrigin(20.0f, 20.0f);
		enemies[i].collisionCircle.setPosition(enemies[i].sprite.getPosition());

		respawnEnemy(enemies[i]);
	}

	for (int i = 0; i < numBullets; i++)
	{
		bullets[i].sprite.setTexture(bulletTexture);
		bullets[i].sprite.setOrigin(bullets[i].sprite.getGlobalBounds().width / 2, bullets[i].sprite.getGlobalBounds().height / 2);
		bullets[i].sprite.setScale(0.5f, 0.5f);

		bullets[i].velocity = sf::Vector2f{ 0.0f,0.0f };

		bullets[i].active = false;
	}

	if (!battleMusic.openFromFile("ASSETS/MUSIC/BiologicalWeapon.ogg"));

	battleMusic.setLoop(true);
	battleMusic.setVolume(50.0f);
	battleMusic.play();

	if (!fireBuffer.loadFromFile("ASSETS/AUDIO/gunshot.wav")) { /* Error */ }
	if (!zombieBuffer.loadFromFile("ASSETS/AUDIO/zombieDeath.wav")) { /* Error */ }
	if (!deathSoundBuffer.loadFromFile("ASSETS/AUDIO/deathNo.wav")) { /* Error */ }
	if (!deathSplatBuffer.loadFromFile("ASSETS/AUDIO/deathSplat.wav")) { /* Error */ }
	if (!painBuffer.loadFromFile("ASSETS/AUDIO/pain.wav")) { /* Error */ }

	fireSound.setBuffer(fireBuffer);
	snakeDeath.setBuffer(zombieBuffer);
	deathSound.setBuffer(deathSoundBuffer);
	deathSplatSound.setBuffer(deathSplatBuffer);
	painSound.setBuffer(painBuffer);

	if (!impactFont.loadFromFile("ASSETS/FONTS/impact.ttf")) { /* Error */ }

	endGameText.setFont(impactFont);
	endGameText.setCharacterSize(64);
	endGameText.setString("YOU LOST!");
	endGameText.setPosition(260.0f, 250.0f);
	endGameText.setFillColor(sf::Color::White);

	scoreText.setFont(impactFont);
	scoreText.setCharacterSize(22);
	scoreText.setString("Score: ");
	scoreText.setPosition(0.0f, 0.0f);
	scoreText.setFillColor(sf::Color::White);

	healthText.setFont(impactFont);
	healthText.setCharacterSize(22);
	healthText.setString("Health: ");
	healthText.setPosition(700.0f, 0.0f);
	healthText.setFillColor(sf::Color::White);
}

// Main update loop for the game
void Game::update()
{
	switch (gameState)
	{
	case GameStates::MainMenu:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			gameState = GameStates::GameOn;
		}
		break;
	case GameStates::GameOn:

		// Call the game playing function for when the game is in play
		gamePlaying();

		break;
	case GameStates::DeathScreen:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			for (int i = 0; i < numOfEnemies; i++)
			{
				respawnEnemy(enemies[i]);
			}

			for (int i = 0; i < numBullets; i++)
			{
				bullets[i].velocity = sf::Vector2f{ 0.0f,0.0f };

				bullets[i].active = false;
			}

			player.sprite.setPosition(400.0f, 300.0f);
			playerPosition = player.sprite.getPosition();
			m_window.setView(sf::View{ playerPosition, sf::Vector2f{ 800.0f,600.0f } });
			player.health = 10;
			score = 0;


			gameState = GameStates::GameOn;
		}
		break;
	case GameStates::PauseScreen:
		break;
	}

	render();
}

// Main game state
void Game::gamePlaying()
{
	playerVelocity.x = 0;
	playerVelocity.y = 0;

	// get keyboard input.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		playerVelocity.x = -5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		playerVelocity.x = 5;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		playerVelocity.y = -5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		playerVelocity.y = 5;
	}
	if (playerVelocity != sf::Vector2f{ 0.0f, 0.0f })
	{
		playerLookDirection.x = signOf(playerVelocity.x);
		playerLookDirection.y = signOf(playerVelocity.y);
	}

	playerRotation = atan2f(playerLookDirection.y, playerLookDirection.x) * 180 / 3.14;
	player.sprite.setRotation(playerRotation);
	playerPosition += playerVelocity;

	if (playerPosition.x > 0 && playerPosition.x < m_onScreenArea.getGlobalBounds().width)
	{
		if (playerPosition.y > 0 && playerPosition.y < m_onScreenArea.getGlobalBounds().height)
		{
			player.sprite.move(playerVelocity);

			m_window.setView(sf::View{ playerPosition, sf::Vector2f{ 800.0f,600.0f } });

			player.collisionCircle.setPosition(player.sprite.getPosition());
		}
		else
		{
			playerPosition -= playerVelocity;
		}
	}
	else
	{
		playerPosition -= playerVelocity;
	}


	for (int i = 0; i < numBullets; i++)
	{
		if (bullets[i].active)
		{
			bullets[i].sprite.move(bullets[i].velocity);
			if (m_onScreenArea.getGlobalBounds().intersects(bullets[i].sprite.getGlobalBounds()) == 0)
			{
				bullets[i].active = false;
				bullets[i].velocity.x = 0;
				bullets[i].velocity.y = 0;
			}
		}
	}
	if (readyToFire)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (!m_fired)
			{
				for (int i = 0; i < numBullets; i++)
				{
					if (!bullets[i].active)
					{
						bullets[i].sprite.setPosition(player.sprite.getPosition() + playerLookDirection * 40.0f);
						bullets[i].active = true;
						bullets[i].velocity = playerLookDirection;
						bullets[i].velocity.x *= 10;
						bullets[i].velocity.y *= 10;

						screenShakeCounter = 5;

						float rotationAngle = atan2f(bullets[i].velocity.y, bullets[i].velocity.x) * 180 / 3.14;
						bullets[i].sprite.setRotation(rotationAngle);
						readyToFire = false;
						m_fired = true;
						waitToFireCounter = 5;
						fireSound.play();
						break;
					}

				}
			}
		}
		else
		{
			m_fired = false;
		}
	}
	else {
		waitToFireCounter--;
		if (waitToFireCounter <= 0)
		{
			readyToFire = true;
		}
	}


	// Enemy loop
	for (int i = 0; i < numOfEnemies; i++)
	{
		if (enemies[i].alive)
		{
			enemies[i].sprite.move(rand() % 5 - 2, rand() % 5 - 2);


			sf::Vector2f distanceVector = player.sprite.getPosition() - enemies[i].sprite.getPosition();
			float magnitudeOfDistance = sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
			sf::Vector2f enemyVelocity = distanceVector / magnitudeOfDistance;

			float currentRotation = atan2f(enemyVelocity.y, enemyVelocity.x) * 180 / 3.14;

			enemies[i].sprite.setRotation(currentRotation);

			enemies[i].sprite.move(enemyVelocity);
			enemies[i].collisionCircle.setPosition(enemies[i].sprite.getPosition());
		}
		else if (enemies[i].respawnCounter > 0)
		{
			enemies[i].respawnCounter--;
		}
		else
		{
			enemies[i].alive = true;
		}
	}

	//collision detection
	for (int i = 0; i < numOfEnemies; i++)
	{
		if (enemies[i].alive)
		{
			// Enemy - player collisions
			if (enemies[i].collisionCircle.getGlobalBounds().intersects(player.collisionCircle.getGlobalBounds()))
			{

				respawnEnemy(enemies[i]);
				player.health--;
				painSound.play();
			}

			// Enemy - bullet collisions
			for (int j = 0; j < numBullets; j++)
			{
				if (bullets[j].active)
				{
					if (enemies[i].collisionCircle.getGlobalBounds().intersects(bullets[j].sprite.getGlobalBounds()))
					{
						bullets[j].active = false;
						bullets[j].velocity.x = 0;
						bullets[j].velocity.y = 0;

						// Kill enemy
						respawnEnemy(enemies[i]);

						snakeDeath.play();
						score++;
					}
				}
			}
		}
	}

	// Update the text objects
	healthText.setString("Health: " + std::to_string(player.health));
	scoreText.setString("Kills: " + std::to_string(score));

	healthText.setPosition(m_window.getView().getCenter().x + 300, m_window.getView().getCenter().y - 300);
	scoreText.setPosition(m_window.getView().getCenter().x - 400, m_window.getView().getCenter().y - 300);


	// Check if dead
	if (player.health <= 0)
	{
		gameState = GameStates::DeathScreen;
		endGameText.setPosition(m_window.getView().getCenter().x - 110, m_window.getView().getCenter().y - 30);
		scoreText.setPosition(m_window.getView().getCenter().x - 20, m_window.getView().getCenter().y + 50);
		bloodSplatSprite.setPosition(m_window.getView().getCenter().x - 400, m_window.getView().getCenter().y - 300);
		deathSound.play();
		deathSplatSound.play();
	}

	screenShake(m_window, player.sprite.getPosition(), 4, screenShakeCounter);
}


// Render function to draw the game
void Game::render()
{
	switch (gameState)
	{
	case GameStates::MainMenu:
		m_window.clear();
		m_window.draw(menuScreenSprite);
		m_window.display();
		break;
	case GameStates::GameOn:
		m_window.clear();

		m_window.draw(backgroundSprite);
		m_window.draw(backgroundBackWallSprite);

		// Draw the enemies
		for (int i = 0; i < numOfEnemies; i++)
		{
			m_window.draw(enemies[i].sprite);
		}

		// Draw the player
		m_window.draw(player.sprite);

		// Draw the bullets
		for (int i = 0; i < numBullets; i++)
		{
			if (bullets[i].active)
			{
				m_window.draw(bullets[i].sprite);
			}
		}

		for (int i = 0; i < numOfWindows; i++)
		{
			m_window.draw(m_entranceWindows[i].sprite);
		}

		m_window.draw(backgroundWallSprite);

		m_window.draw(healthText);
		m_window.draw(scoreText);

		m_window.display();

		break;
	case GameStates::DeathScreen:
		m_window.clear();

		m_window.draw(backgroundSprite);
		m_window.draw(backgroundBackWallSprite);

		// Draw the enemies
		for (int i = 0; i < numOfEnemies; i++)
		{
			m_window.draw(enemies[i].sprite);
		}

		// Draw the player
		m_window.draw(player.sprite);

		// Draw the bullets
		for (int i = 0; i < numBullets; i++)
		{
			if (bullets[i].active)
			{
				m_window.draw(bullets[i].sprite);
			}
		}

		m_window.draw(backgroundWallSprite);

		m_window.draw(bloodSplatSprite);
		m_window.draw(endGameText);
		m_window.draw(scoreText);

		m_window.display();
		break;
	case GameStates::PauseScreen:
		break;
	default:
		break;
	}
}



// Sets random spawn location for enemy
void Game::respawnEnemy(Zombie & t_enemy)
{
	int spawnPosition = rand() % 8;

	switch (spawnPosition)
	{
	case 0:
		t_enemy.sprite.setPosition(m_entranceWindows[0].sprite.getPosition());
		break;
	case 1:
		t_enemy.sprite.setPosition(m_entranceWindows[0].sprite.getPosition());
		break;
	case 2:
		t_enemy.sprite.setPosition(m_entranceWindows[0].sprite.getPosition());
		break;
	case 3:
		t_enemy.sprite.setPosition(m_entranceWindows[0].sprite.getPosition());
		break;
	case 4:
		t_enemy.sprite.setPosition(m_entranceWindows[0].sprite.getPosition());
		break;
	case 5:
		t_enemy.sprite.setPosition(m_entranceWindows[0].sprite.getPosition());
		break;
	case 6:
		t_enemy.sprite.setPosition(m_entranceWindows[0].sprite.getPosition());
		break;
	case 7:
		t_enemy.sprite.setPosition(m_entranceWindows[0].sprite.getPosition());
		break;
	}

	t_enemy.alive = false;
	t_enemy.respawnCounter = rand() % 120 + 30;
}

float Game::signOf(float t_value)
{
	float sign{ 0.0f };

	if (t_value > 0.0f)
	{
		sign = 1.0f;
	}
	else if (t_value < 0.0f)
	{
		sign = -1.0f;
	}

	return sign;
}

void Game::screenShake(sf::RenderWindow & t_window, sf::Vector2f t_position, int t_shakeMagnitude, int & t_counter)
{
	if (t_counter > 0)
	{
		t_counter--;
		t_window.setView(sf::View{ sf::Vector2f{ t_position.x + rand() % t_shakeMagnitude - t_shakeMagnitude / 2, t_position.y + rand() % t_shakeMagnitude - t_shakeMagnitude / 2 }, sf::Vector2f{ 800.0f, 600.0f } });
	}
}
#pragma once

#include "Snake.h"

class Game
{
public:
	// Constructor & Destructors
	Game();
	~Game();

	// Functions
	void pollEvents();
	void updateDt() { this->dt = this->dtClock.restart().asSeconds(); }

	void lose();
	void endScreen();
	void snakeCollision();
	void spawnApple(int amount);
	void restart();

	void update();
	void render();

	void run();

private:
	// Window
	sf::RenderWindow window;
	unsigned int frame_limit = 0;

	// Dt
	sf::Clock dtClock;
	float dt = 0.f;

	// Snake
	Snake snake = Snake(sf::Vector2f(400.f, 400.f), sf::Vector2f(16.f, 16.f), sf::Color::Yellow);

	// Apples
	std::vector<Apple> apples;

	// Game Logic
	sf::Clock timeClock;
	sf::Time timeSinceStart;
	unsigned int score = 0;
	unsigned int bestScore = 0;
	unsigned int bestLength = 0;
	bool gameLost = false;

	// Font
	sf::Font font;
	sf::Text text;
};


#include "Game.h"

// Constructor & Destructors
Game::Game() :
	window(sf::VideoMode(800, 800), "Snake", sf::Style::Close | sf::Style::Titlebar)
{
	// Init Window
	this->window.setVerticalSyncEnabled(false);
	this->frame_limit = 10;

	// Init Fonts
	if (!this->font.loadFromFile("Assets/press-start-2p-font/PressStart2P-vaV7.ttf")) {
		std::cout << "Game.cpp:Game: Could not load font!\n";
	}
	this->text.setFont(this->font);
	this->text.setCharacterSize(24);
	this->text.setFillColor(sf::Color::White);
	this->text.setStyle(sf::Text::Bold);
	this->text.setPosition(10.f, 10.f);

	// Init Scores
	std::ifstream file("high_score.txt");
	if (file.is_open()) {
		std::string s;
		std::stringstream ss;
		int count = 0;
		while (std::getline(file, s)) {
			ss << s;
			if (count == 0) {
				ss >> this->bestScore;
				ss.clear();
			}
			else
				ss >> this->bestLength;
			count++;
		}
	}
	file.close();

	// Init Apples
	this->spawnApple(1);
}

Game::~Game()
{
	this->window.~RenderWindow();
}

// Functions
void Game::pollEvents()
{
	sf::Event events;
	while (this->window.pollEvent(events)) {
		switch (events.type) {
		case sf::Event::Closed:
			this->window.close();
			break;
		}
	}
}

void Game::lose()
{
	if (this->score > this->bestScore || this->snake.getSnakeBody().size() > this->bestLength) {
		std::ofstream file("high_score.txt");
		if (file.is_open()) {
			if (this->score > this->bestScore)
				this->bestScore = this->score;
			file << this->bestScore;
			file << "\n";
			if (this->snake.getSnakeBody().size() > this->bestLength)
				this->bestLength = this->snake.getSnakeBody().size();
			file << this->bestLength;
		}
		file.close();
	}

	this->gameLost = true;
}

void Game::endScreen() {
	// Snake
	sf::Color colorR = sf::Color::Red;
	sf::Color colorG = sf::Color::Green;
	std::vector<sf::Color> colors;
	colors.push_back(colorG);
	colors.push_back(colorR);

	for (int j = 0; j < this->snake.getSnakeBody().size(); j++) {
		srand(time(0));
		int random = rand() % 2;
		this->snake.getSnakeBody()[j].setFillColor(colors[random]);
	}

	// UI
	int textSize = this->text.getCharacterSize();
	this->text.setPosition(sf::Vector2f(this->window.getSize().x / 2 - (textSize * 2 + 150), 10.f));
	this->text.setString("	END GAME!!\n	 Score: " + std::to_string(this->score) + "\nPress 'R' to restart!");
}

void Game::snakeCollision()
{
	// Apple Collision
	for (int i = 0; i < this->apples.size(); i++) {
		if (this->snake.getSnakeBody()[0].getGlobalBounds().intersects(this->apples[i].getGlobalBounds())) {
			this->apples.erase(this->apples.begin() + i);
			this->score++;
			this->spawnApple(1);
			this->snake.extend();
			this->snake.velocity.x = this->snake.velocity.y += 20.f;
			this->frame_limit += 1;
		}
	}

	// Window Bounds
	if (this->snake.getSnakeBody()[0].getPosition().x > this->window.getSize().x + this->snake.getSnakeBody()[0].getSize().x) {
		this->snake.getSnakeBody()[0].setPosition(sf::Vector2f(-this->snake.getSnakeBody()[0].getSize().x, this->snake.getSnakeBody()[0].getPosition().y));
	}
	if (this->snake.getSnakeBody()[0].getPosition().x < -this->snake.getSnakeBody()[0].getSize().x) {
		this->snake.getSnakeBody()[0].setPosition(sf::Vector2f(this->window.getSize().x + this->snake.getSnakeBody()[0].getSize().x, this->snake.getSnakeBody()[0].getPosition().y));
	}

	if (this->snake.getSnakeBody()[0].getPosition().y > this->window.getSize().y + this->snake.getSnakeBody()[0].getSize().y) {
		this->snake.getSnakeBody()[0].setPosition(sf::Vector2f(this->snake.getSnakeBody()[0].getPosition().x, -this->snake.getSnakeBody()[0].getSize().y));
	}
	if (this->snake.getSnakeBody()[0].getPosition().y < -this->snake.getSnakeBody()[0].getSize().y) {
		this->snake.getSnakeBody()[0].setPosition(sf::Vector2f(this->snake.getSnakeBody()[0].getPosition().x, this->window.getSize().y + this->snake.getSnakeBody()[0].getSize().y));
	}

	// Snake Collision
	for (int i = 4; i < this->snake.getSnakeBody().size(); i++) {
		SnakeSegment& snakePart = this->snake.getSnakeBody()[i];
		SnakeSegment& snakeHead = this->snake.getSnakeBody()[0];

		if (snakeHead.getGlobalBounds().intersects(snakePart.getGlobalBounds())) {
			this->lose();
		}
	}
}

void Game::spawnApple(int amount)
{
	for (int i = 0; i < amount; i++) {
		this->apples.push_back(Apple(sf::Vector2f(rand() % (this->window.getSize().x - 16), rand() % (this->window.getSize().y - 16)),
			sf::Vector2f(16.f, 16.f), sf::Color::Red));
	}
}

void Game::restart()
{
	this->snake.restart();
	this->score = 0;
	this->gameLost = false;
	this->text.setPosition(sf::Vector2f(10.f, 10.f));
	this->frame_limit = 10;
}

void Game::update()
{
	this->timeSinceStart = this->timeClock.getElapsedTime();

	if (!this->gameLost) {

		// Snake
		if (this->timeSinceStart.asSeconds() > 0.2f)
			this->snakeCollision();
		this->snake.update(this->dt, this->frame_limit, this->window);
		this->text.setString("Score: " + std::to_string(this->score));
	}
	else {
		// End Screen
		this->endScreen();

		// Restart
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			this->restart();
		}
	}
}

void Game::render()
{
	// Clear the screen
	this->window.clear(sf::Color::Black);

	// Render stuff only when the game hasn't been lost
	if (!this->gameLost) {
		// Render UI
		this->window.draw(this->text);

		// Render Apples
		for (int i = 0; i < this->apples.size(); i++) {
			this->apples[i].render(this->window);
		}
	}
	else {
		// Render UI
		this->window.draw(this->text);
	}

	// Snake
	this->snake.render(this->window);

	// Display everything on the screen
	this->window.display();
}

void Game::run()
{
	while (this->window.isOpen()) {
		this->pollEvents();
		this->updateDt();
		this->update();
		this->render();
	}
}

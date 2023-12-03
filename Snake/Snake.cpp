#include "Snake.h"

// Constructor & Destructors
Snake::Snake(sf::Vector2f position, sf::Vector2f size, sf::Color fillColor)
{
	// Init Snake
	this->startPosition = position;
	this->startColor = fillColor;
	this->startSize = size;
	this->snakeBody.push_back(SnakeSegment(position, size, fillColor));

	for (int i = 0; i < 4; i++) {
		this->extend();
	}
}

Snake::~Snake()
{
}

// Helper Functions
void Snake::extend()
{
	if (this->snakeBody.empty()) return;

	SnakeSegment& lastPart = this->snakeBody[this->snakeBody.size() - 1];
	SnakeSegment snake(sf::Vector2f(lastPart.getPosition().x, lastPart.getPosition().y + (lastPart.getSize().y + 1.f)),
		lastPart.getSize(), sf::Color::Green);
	this->snakeBody.push_back(snake);
}

void Snake::movement(const float& dt)
{
	// Return if empty
	if (this->snakeBody.empty()) return;

	// Tail
	for (int i = this->snakeBody.size() - 1; i > 0; i--)
	{
		SnakeSegment& tail = this->snakeBody[i];
		SnakeSegment& behindTail = this->snakeBody[i - 1];

		tail.setPosition(behindTail.getPosition());
	}

	// Head
	this->snakeBody[0].move(this->velocity.x * this->directions.x * dt,
		this->velocity.y * this->directions.y * dt);
}

void Snake::restart()
{
	this->directions = sf::Vector2i(0, -1);
	this->velocity = sf::Vector2f(200.f, 200.f);
	this->snakeBody.clear();

	this->snakeBody.push_back(SnakeSegment(this->startPosition, this->startSize, this->startColor));
	for (int i = 0; i < 4; i++) {
		this->extend();
	}
}

// Functions
void Snake::input()
{
	// Right & Left Directions
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && this->directions.x != 1) {
		this->directions.x = -1;
		this->directions.y = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && this->directions.x != -1) {
		this->directions.x = 1;
		this->directions.y = 0;
	}

	// Up & Down Directions
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && this->directions.y != 1) {
		this->directions.y = -1;
		this->directions.x = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && this->directions.y != -1) {
		this->directions.y = 1;
		this->directions.x = 0;
	}
}

void Snake::update(const float& dt, unsigned int frame_limit, sf::RenderWindow& window)
{
	// Inputs
	window.setFramerateLimit(0);
	this->input();
	window.setFramerateLimit(frame_limit);

	// Movement
	this->movement(dt);
}

void Snake::render(sf::RenderTarget& target)
{
	for (int i = 0; i < this->snakeBody.size(); i++) {
		this->snakeBody[i].render(target);
	}
}

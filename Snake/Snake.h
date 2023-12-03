#pragma once

#include "Apple.h"


class SnakeSegment {
public:
	// Constructor & Destructor
	SnakeSegment(sf::Vector2f position, sf::Vector2f size, sf::Color fillColor) {
		this->sprite.setPosition(position);
		this->sprite.setFillColor(fillColor);
		this->sprite.setSize(size);
	}
	~SnakeSegment() {}

	// Accessors
	const sf::Vector2f getPosition() const { return this->sprite.getPosition(); }
	const sf::Vector2f getSize() const { return this->sprite.getSize(); }
	const sf::FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	const sf::Color getFillColor() const { return this->sprite.getFillColor(); }

	// Helper Functions
	void move(const float offsetX, const float offsetY) { this->sprite.move(offsetX, offsetY); }
	void setPosition(sf::Vector2f position) { this->sprite.setPosition(position); }
	void setFillColor(sf::Color color) { this->sprite.setFillColor(color); }

	// Functions
	void render(sf::RenderTarget& target) { target.draw(this->sprite); }

private:
	sf::RectangleShape sprite;
};

class Snake
{
public:
	// Variables
	sf::Vector2f velocity = sf::Vector2f(200.f, 200.f);

	// Constructor & Destructors
	Snake(sf::Vector2f position, sf::Vector2f size, sf::Color fillColor);
	~Snake();

	// Accessors
	std::vector<SnakeSegment>& getSnakeBody() { return this->snakeBody; }

	// Helper Functions
	void extend();
	void movement(const float& dt);
	void restart();

	// Functions
	void input();
	void update(const float& dt, unsigned int frame_limit, sf::RenderWindow& window);
	void render(sf::RenderTarget& target);


private:
	// Snake Body
	std::vector<SnakeSegment> snakeBody;

	// Defaults
	sf::Vector2f startPosition;
	sf::Vector2f startSize;
	sf::Color startColor;

	// Movement
	sf::Vector2i directions = sf::Vector2i(0, -1);
};


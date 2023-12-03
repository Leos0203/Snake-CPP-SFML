#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Apple
{
public:
	// Constructor & Destructor
	Apple(sf::Vector2f position, sf::Vector2f size, sf::Color color) {
		this->sprite.setPosition(position);
		this->sprite.setSize(size);
		this->sprite.setFillColor(color);
	}
	~Apple() {}

	// Accessors
	const sf::Vector2f getPosition() const { return this->sprite.getPosition(); }
	const sf::Vector2f getSize() const { return this->sprite.getSize(); }
	const sf::Color getFillColor() const { return this->sprite.getFillColor(); }
	const sf::FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }

	// Functions
	void render(sf::RenderTarget& target) { target.draw(this->sprite); }

private:
	// Texture stuff
	sf::RectangleShape sprite;
};


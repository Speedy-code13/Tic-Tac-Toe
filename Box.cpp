#include "stdafx.h"
#include "Box.h"

Box::Box(sf::Texture& texture, const sf::Vector2f& size, const sf::IntRect& oRect, const sf::IntRect& xRect, sf::RenderWindow& window, const bool& canDraw)
	: texture(texture), window(window), drawn(false), state(State::None), oRect(oRect), xRect(xRect), drawnForWin(false), canDraw(canDraw)
{	
	shape.setSize(size);

	turn = Turn::X;
	
}

void Box::setPosition(const sf::Vector2f& pos)
{
	shape.setPosition(pos);
}

void Box::update(const sf::Vector2f& mousePos)
{
	if (!drawn)
	{
		if (shape.getGlobalBounds().contains(mousePos))
		{
			//Hover
			shape.setFillColor(sf::Color(220, 220, 220));

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && canDraw)
			{
				//Draw X 
				if (turn == Turn::X)
				{
					shape.setFillColor(sf::Color::White);
					shape.setTexture(&texture, true);
					shape.setTextureRect(xRect);

					state = State::X;
					turn = Turn::O;
					drawnForWin = true;
				}
				//Draw O
				else
				{
					shape.setFillColor(sf::Color::White);

					shape.setTexture(&texture, true);
					shape.setTextureRect(oRect);

					state = State::O;
					turn = Turn::X;
					drawnForWin = true;
				}
				drawn = true;
			}
		}
		else
		{
			//None
			shape.setFillColor(sf::Color::Transparent);
		}
	}
}

void Box::render()
{
	window.draw(shape);
}

const sf::Vector2f& Box::getPosition() const
{
	return shape.getPosition();
}

const State& Box::getState() const
{
	return state;
}

const bool Box::drawnBox() 
{
	if (drawnForWin)
	{
		drawnForWin = false;
		return true;
	}

	return false;
}

void Box::reset()
{
	drawn = false;
	drawnForWin = false;
	state = State::None;

	shape.setTextureRect(sf::IntRect());
	turn = Turn::X;
}

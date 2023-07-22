#include "stdafx.h"
#include "Box.h"

Box::Box(sf::Texture& texture, const sf::Vector2f& size, const sf::IntRect& oRect, const sf::IntRect& xRect,
	sf::RenderWindow& window, const bool& canDraw, const float& dt,  std::unordered_map<std::string, sf::Sound>& sounds)
	: texture(texture), window(window), drawn(false), state(BoxState::None), oRect(oRect),
	  xRect(xRect), drawnForWin(false), canDraw(canDraw), initialTransparency(30), fadeInSpeed(500.f),
      dt(dt), sounds(sounds)
	 
{	
	shape.setSize(size);

	turn = Turn::X;
	
}

void Box::setPosition(const sf::Vector2f& pos)
{
	shape.setPosition(pos);
}

void Box::forceFinishAnimation()
{
	if(drawn)
		shape.setFillColor(sf::Color::White);
}

void Box::updateAnimation()
{
	sf::Color currentColor = shape.getFillColor();
	if (currentColor.a != 255) {
		if (currentColor.a + dt * fadeInSpeed > 255) currentColor.a = 255;
		else currentColor.a += dt * fadeInSpeed;
		shape.setFillColor(currentColor);
	}
}

void Box::update(const sf::Vector2f& mousePos)
{
	if (drawn) {
		updateAnimation();
		return;
	}
	
	if (shape.getGlobalBounds().contains(mousePos))
	{
		//Hover
		shape.setFillColor(sf::Color(195, 195, 195));
	}
	else
	{
		//None
		shape.setFillColor(sf::Color::Transparent);
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

const BoxState& Box::getState() const
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

void Box::onMouseClick(const sf::Vector2f& mousePos)
{
	if (shape.getGlobalBounds().contains(mousePos) && canDraw && !drawn)
	{
		drawn = true;
		drawnForWin = true;
		shape.setFillColor(sf::Color(255, 255, 255, initialTransparency));
		shape.setTexture(&texture, true);
		sounds["DRAW_BOX"].play();
		
		//Draw X 
		if (turn == Turn::X)
		{
			shape.setTextureRect(xRect);

			state = BoxState::X;
			turn = Turn::O;
		}
		//Draw O
		else
		{
			shape.setTextureRect(oRect);

			state = BoxState::O;
			turn = Turn::X;
		}
	}

}

void Box::reset()
{
	drawn = false;
	drawnForWin = false;
	state = BoxState::None;

	shape.setTextureRect(sf::IntRect());
	turn = Turn::X;
}

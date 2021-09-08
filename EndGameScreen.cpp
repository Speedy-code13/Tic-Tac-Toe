#include "stdafx.h"
#include "EndGameScreen.h"

EndGameScreen::EndGameScreen(const Winner& winner, sf::RenderWindow& window, const sf::Font& font, const float& dt, std::function<void()> gameReset)
	: winner(winner), window(window), font(font), dt(dt), gameReset(gameReset)
{
	initVariables();
	initTextures();
	initBackground();
	initReloadButton();
	initText();
}

void EndGameScreen::initVariables()
{
	backgroundSpeed = 700.f;
	backgroundLoaded = false;
	scoreX = 0;
	scoreO = 0;
}

void EndGameScreen::initTextures()
{
	textures["RELOAD_BUTTON"].loadFromFile("rsc/img/reload.png");
}

void EndGameScreen::initBackground()
{
	background.setSize({ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y + 25.f) });
	background.setFillColor(sf::Color(20, 20, 20, 200));
	background.setPosition({0.f, -background.getSize().y});
}

void EndGameScreen::initReloadButton()
{
	reloadButton.setSize({200.f, 200.f});
	reloadButton.setTexture(&textures["RELOAD_BUTTON"]);
	reloadButton.setPosition(window.getSize().x / 2.f - reloadButton.getGlobalBounds().width / 2.f, window.getSize().y / 2.f - reloadButton.getGlobalBounds().height / 2.f);
}

void EndGameScreen::initText()
{
	winText.setFont(font);
	winText.setCharacterSize(32);
	winText.setFillColor(sf::Color::White);
	winText.setString("Error string!");
	winText.setPosition({ window.getSize().x / 2.f - winText.getGlobalBounds().width / 2.f, 0.f });


	scoreText.setFont(font);
	scoreText.setCharacterSize(32);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setString("Error string!");
	scoreText.setPosition({ window.getSize().x / 2.f - scoreText.getGlobalBounds().width / 2.f, (0.f + reloadButton.getPosition().y ) / 2.f - scoreText.getGlobalBounds().height });
}

void EndGameScreen::reset()
{
	background.setPosition({ 0.f, -background.getSize().y });
	backgroundLoaded = false;
	gameReset();
}

void EndGameScreen::teamHasWon()
{
	switch (winner)
	{
	case Winner::O:
		scoreO++;
		winText.setString("O has won!");
		winText.setPosition({ window.getSize().x / 2.f - winText.getGlobalBounds().width / 2.f, 0.f });
		winText.setFillColor(sf::Color::Cyan);
		break;
	case Winner::X:
		scoreX++;
		winText.setString("X has won!");
		winText.setPosition({ window.getSize().x / 2.f - winText.getGlobalBounds().width / 2.f, 0.f });
		winText.setFillColor(sf::Color::Red);
		break;
	case Winner::Draw:
		winText.setString("DRAW!");
		winText.setPosition({ window.getSize().x / 2.f - winText.getGlobalBounds().width / 2.f, 0.f });
		winText.setFillColor(sf::Color::Green);
		break;
	}
	scoreText.setString(std::to_string(scoreO) + '-' + std::to_string(scoreX));
	scoreText.setPosition({ window.getSize().x / 2.f - scoreText.getGlobalBounds().width / 2.f, (0.f + reloadButton.getPosition().y) / 2.f - scoreText.getGlobalBounds().height });
}

void EndGameScreen::update(const sf::Vector2f& mousePos)
{
	backgroundLoaded = !(background.getPosition().y <= -20.f);
	if (!backgroundLoaded)
		background.move(0.f, backgroundSpeed * dt);

	else {
		if (reloadButton.getGlobalBounds().contains(mousePos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				reset();
		}
	}
	
}

void EndGameScreen::render()
{
	window.draw(background);

	if (backgroundLoaded)
	{
		window.draw(winText);
		window.draw(scoreText);
		window.draw(reloadButton);
	}
}


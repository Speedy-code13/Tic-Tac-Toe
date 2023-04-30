#include "stdafx.h"
#include "EndGameScreen.h"

EndGameScreen::EndGameScreen(const GameState& gameState, sf::RenderWindow& window, const sf::Font& font,
	const float& dt, std::unordered_map<std::string, sf::Sound>& sounds, std::function<void()> gameReset)
	: gameState(gameState), window(window), font(font), dt(dt), gameReset(gameReset), sounds(sounds)
{
	initVariables();
	initTextures();
	initBackground();
	initReloadButton();
	initText();
}

void EndGameScreen::initVariables()
{
	backgroundSpeed = 550.f;
	backgroundAnimated = false;
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
	background.setFillColor(sf::Color(20, 20, 20, 220));
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
	backgroundAnimated = false;
	gameReset();
}

void EndGameScreen::onRoundFinish()
{
	switch (gameState)
	{
		case GameState::OWin:
			scoreO++;
			winText.setString("O has won!");
			winText.setPosition({ window.getSize().x / 2.f - winText.getGlobalBounds().width / 2.f, 0.f });
			winText.setFillColor(sf::Color::Cyan);
			break;
		case GameState::XWin:
			scoreX++;
			winText.setString("X has won!");
			winText.setPosition({ window.getSize().x / 2.f - winText.getGlobalBounds().width / 2.f, 0.f });
			winText.setFillColor(sf::Color::Red);
			break;
		case GameState::Draw:
			winText.setString("DRAW!");
			winText.setPosition({ window.getSize().x / 2.f - winText.getGlobalBounds().width / 2.f, 0.f });
			winText.setFillColor(sf::Color::Green);
			break;
	}
	scoreText.setString(std::to_string(scoreO) + " - " + std::to_string(scoreX));
	scoreText.setPosition({ window.getSize().x / 2.f - scoreText.getGlobalBounds().width / 2.f, (0.f + reloadButton.getPosition().y) / 2.f - scoreText.getGlobalBounds().height });
}

void EndGameScreen::update(const sf::Vector2f& mousePos)
{
	backgroundAnimated = !(background.getPosition().y <= -20.f);
	if (!backgroundAnimated) {
		background.move(0.f, backgroundSpeed * dt);
		return;
	}

	if (reloadButton.getGlobalBounds().contains(mousePos)) // resetting the game when player presses the reload button
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			reset();
			sounds["RESTART_BUTTON"].play();
		}
	}
	
}

void EndGameScreen::render()
{
	window.draw(background);

	if (!backgroundAnimated) return;

	window.draw(winText);
	window.draw(scoreText);
	window.draw(reloadButton);
}


#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	initVariables();
	initWindow();
	initTextures();
	initFonts();
	initBackground();
	initCombinations();
	initBoxes();
	initEndScreen();
	initSounds();
}

Game::~Game()
{
}

void Game::initWindow()
{
	width = 549.f;
	height = 554.f;

	window = std::make_shared<sf::RenderWindow>(
		sf::VideoMode(static_cast<unsigned int>(width), static_cast<unsigned int>(height)),
		"Tic Tac Toe",
		sf::Style::Close
		);

	window->setFramerateLimit(60);

	if (icon.loadFromFile("rsc/img/Tic-Tac-Toe-Game.png"))
		window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Game::initTextures()
{
	textures["BACKGROUND"].loadFromFile("rsc/img/background.png");
	textures["BOX"].loadFromFile("rsc/img/box.png");
}

void Game::initFonts()
{
	font.loadFromFile("rsc/fonts/arial.ttf");
}

void Game::initBackground()
{
	background.setSize({ width, height });
	background.setTexture(&textures["BACKGROUND"]);
}

void Game::positionBoxes()
{
	const float distanceBetweenBoxesX = 22.f;
	const float distanceBetweenBoxesY = 21.f;

	boxes[0]->setPosition(sf::Vector2f(34.f, 39.f));
	boxes[1]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + distanceBetweenBoxesX + boxWidth, boxes[0]->getPosition().y));
	boxes[2]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + (distanceBetweenBoxesX + boxWidth) * 2, boxes[0]->getPosition().y));
	boxes[3]->setPosition(sf::Vector2f(boxes[0]->getPosition().x, boxes[0]->getPosition().y + distanceBetweenBoxesY + boxHeight));
	boxes[4]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + distanceBetweenBoxesX + boxWidth, boxes[0]->getPosition().y + distanceBetweenBoxesY + boxHeight));
	boxes[5]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + (distanceBetweenBoxesX + boxWidth) * 2, boxes[0]->getPosition().y + distanceBetweenBoxesY + boxHeight));
	boxes[6]->setPosition(sf::Vector2f(boxes[0]->getPosition().x, boxes[0]->getPosition().y + (distanceBetweenBoxesY + boxHeight)* 2));
	boxes[7]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + distanceBetweenBoxesX + boxWidth, boxes[0]->getPosition().y + (distanceBetweenBoxesY + boxHeight) * 2));
	boxes[8]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + (distanceBetweenBoxesX + boxWidth) * 2, boxes[0]->getPosition().y + (distanceBetweenBoxesY + boxHeight) * 2));
}

void Game::initBoxes()
{
	boxWidth = 148.f;
	boxHeight = 145.f;
	oRect = sf::IntRect(0, 0, static_cast<int>(boxWidth), static_cast<int>(boxHeight));
	xRect = sf::IntRect(static_cast<int>(boxWidth), 0, static_cast<int>(boxWidth), static_cast<int>(boxHeight));

	for (auto& box : boxes)	
	{
		box = std::make_unique<Box>(textures["BOX"], sf::Vector2f(boxWidth, boxHeight), oRect, xRect, *window,
			canDraw, dt, sounds);
	}
	positionBoxes();
}

void Game::initEndScreen()
{
	endScreen = std::make_unique<EndGameScreen>(gameState, *window, font, dt, sounds, 
		[&]()
		{
			reset();
		}
	);
}

void Game::initSounds()
{
	initSound("DRAW_BOX", "rsc/sfx/clickselect2-92097.wav", 35.f);
	initSound("RESTART_BUTTON", "rsc/sfx/select-sound-121244.wav", 50.f);
	initSound("ROUND_OVER", "rsc/sfx/mixkit-completion-of-a-level-2063.wav");
}

void Game::initSound(std::string name, std::string path, float volume)
{
	soundBuffers[name].loadFromFile(path);
	sounds[name].setBuffer(soundBuffers[name]);
	sounds[name].setVolume(volume);
}

const GameState& Game::getGameState()
{
	//Lazy method using brute force
	for (auto& winCombination : winCombinations)
	{
		if (hasTeamWon(BoxState::X, winCombination.pos1, winCombination.pos2, winCombination.pos3)) {
			currentWinCombination = &winCombination;
			return GameState::XWin;
		}

		if (hasTeamWon(BoxState::O, winCombination.pos1, winCombination.pos2, winCombination.pos3)) {
			currentWinCombination = &winCombination;
			return GameState::OWin;
		}
			
	}

	if (checkForDraw()) {
		currentWinCombination = nullptr;
		return GameState::Draw;
	}

	return GameState::Ongoing;

}

void Game::forceFinishBoxesAnimations()
{
	for(auto& box : boxes)
	{
		box->forceFinishAnimation();
	}
}

bool Game::hasTeamWon(BoxState team, int winnerPos1, int winnerPos2, int winnerPos3)
{
	if (boxes[winnerPos1]->getState() == team && boxes[winnerPos2]->getState() == team && boxes[winnerPos3]->getState() == team)
		return true;	

	return false;
}

const bool Game::checkForDraw()
{
	int numOfOccupiedBoxes = 0;
	for (auto& box : boxes)
	{
		BoxState currBoxState = box->getState();

		if (currBoxState == BoxState::X || currBoxState == BoxState::O)
		{
			numOfOccupiedBoxes++;
		}
	}
	return numOfOccupiedBoxes == boxes.size();
}

void Game::boxesOnMouseClick()
{
	for (auto& box : boxes) {
		box->onMouseClick(mousePos);
	}
}

void Game::updateCanDraw()
{
	if (resetClock.getElapsedTime().asMilliseconds() > 200)
		canDraw = true;
}

void Game::updateEvents()
{
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				window->close();
				break;

			case sf::Event::MouseButtonReleased:
				if (!inEndScreen) {
					boxesOnMouseClick();
					updateGameState();

				}

				break;
		}
	}
}

void Game::update()
{
	updateEvents();

	mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));

	updateDt();

	if (inEndScreen)
	{
		endScreen->update(mousePos);
	}
	else
	{
		updateCanDraw();

		for (auto& i : boxes)
			i->update(mousePos);
	}
}

void Game::render()
{
	window->clear();

	window->draw(background);

	for (auto& i : boxes)
		i->render();

	if (inEndScreen)
		endScreen->render();

	window->display();
}

void Game::updateGameState()
{
	this->gameState = getGameState();

	if (this->gameState != GameState::Ongoing)
	{
		sounds["ROUND_OVER"].play();
		endScreen->onRoundFinish();
		this->forceFinishBoxesAnimations();
		this->inEndScreen = true;
	}
}

void Game::updateDt()
{
	dt = dtClock.restart().asSeconds();
}

void Game::run()
{
	while (window->isOpen())
	{
		update();
		render();
	}
}

void Game::initCombinations()
{
	winCombinations[0].init(0, 1, 2);
	winCombinations[1].init(3, 4, 5);
	winCombinations[2].init(6, 7, 8);
	winCombinations[3].init(0, 3, 6);
	winCombinations[4].init(1, 4, 7);
	winCombinations[5].init(2, 5, 8);
	winCombinations[6].init(0, 4, 8);
	winCombinations[7].init(2, 4, 6);
}

void Game::initVariables()
{
	inEndScreen = false;	
	canDraw = false;
	dt = 0.f;
	gameState = GameState::Ongoing;
	currentWinCombination = nullptr;
}

void Game::reset()
{
	for (auto& i : boxes)
		i->reset();

	inEndScreen = false;

	canDraw = false;
	resetClock.restart();

}


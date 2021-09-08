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
	const float distanceBetweenBoxes = 10.f;

	boxes[0]->setPosition(sf::Vector2f(27.f, 34.f));
	boxes[1]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + distanceBetweenBoxes + boxWidth, boxes[0]->getPosition().y));
	boxes[2]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + (distanceBetweenBoxes + boxWidth) * 2, boxes[0]->getPosition().y));
	boxes[3]->setPosition(sf::Vector2f(boxes[0]->getPosition().x, boxes[0]->getPosition().y + distanceBetweenBoxes + boxHeight));
	boxes[4]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + distanceBetweenBoxes + boxWidth, boxes[0]->getPosition().y + distanceBetweenBoxes + boxHeight));
	boxes[5]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + (distanceBetweenBoxes + boxWidth) * 2, boxes[0]->getPosition().y + distanceBetweenBoxes + boxHeight));
	boxes[6]->setPosition(sf::Vector2f(boxes[0]->getPosition().x, boxes[0]->getPosition().y + (distanceBetweenBoxes + boxHeight)* 2));
	boxes[7]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + distanceBetweenBoxes + boxWidth, boxes[0]->getPosition().y + (distanceBetweenBoxes + boxHeight) * 2));
	boxes[8]->setPosition(sf::Vector2f(boxes[0]->getPosition().x + (distanceBetweenBoxes + boxWidth) * 2, boxes[0]->getPosition().y + (distanceBetweenBoxes + boxHeight) * 2));
}

void Game::initBoxes()
{
	boxWidth = 160.f;
	boxHeight = 155.f;
	oRect = sf::IntRect(0, 0, static_cast<int>(boxWidth), static_cast<int>(boxHeight));
	xRect = sf::IntRect(static_cast<int>(boxWidth), 0, static_cast<int>(boxWidth), static_cast<int>(boxHeight));

	for (auto& i : boxes)	
	{
		i = std::make_unique<Box>(textures["BOX"], sf::Vector2f(boxWidth, boxHeight), oRect, xRect, *window, canDraw);
	}
	positionBoxes();
}

void Game::initEndScreen()
{
	endScreen = std::make_unique<EndGameScreen>(winner, *window, font, dt, 
		[&]()
		{
			reset();
		}
	);
}

const Winner& Game::getWinner()
{
	//Lazy method using brute force
	for (auto& i : winCombinations)
	{
		if (checkTeamForWin(State::X, i.comb1, i.comb2, i.comb3))
			return Winner::X;

		if (checkTeamForWin(State::O, i.comb1, i.comb2, i.comb3))
			return Winner::O;
	}

	if (checkForDraw())
		return Winner::Draw;

	//Else
	return Winner::None;

}

bool Game::checkTeamForWin(State team, int winnerPos1, int winnerPos2, int winnerPos3)
{
	if (boxes[winnerPos1]->getState() == team && boxes[winnerPos2]->getState() == team && boxes[winnerPos3]->getState() == team)
		return true;	

	return false;
}

void Game::updateShouldCheckForWin()
{
	//If any box is drawn shouldCheckForWin is true
	for (auto& i : boxes)
	{
		if (i->drawnBox())
		{
			shouldCheckForWin = true;
			return;
		}
	}
}

const bool Game::checkForDraw()
{
	int numOfOccupiedBoxes = 0;
	for (auto& i : boxes)
	{
		State currBoxState = i->getState();

		if (currBoxState == State::X || currBoxState == State::O)
		{
			numOfOccupiedBoxes++;
			continue;
		}

		break;
	}
	if (numOfOccupiedBoxes == boxes.size())
		return true;

	return false;
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
		}
	}
}

void Game::update()
{
	updateEvents();

	mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));

	updateDt();

	if (!inEndScreen)
	{
		updateCanDraw();

		for (auto& i : boxes)
			i->update(mousePos);

		updateWin();
	}
	else
		endScreen->update(mousePos);

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

void Game::updateWin()
{
	updateShouldCheckForWin();

	if (shouldCheckForWin)
	{
		winner = getWinner();
		if (winner != Winner::None)
		{
			endScreen->teamHasWon();
			inEndScreen = true;
		}
			shouldCheckForWin = false;
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
	shouldCheckForWin = false;
	inEndScreen = false;	
	canDraw = false;
	dt = 0.f;
	winner = Winner::None;
}

void Game::reset()
{
	for (auto& i : boxes)
		i->reset();

	inEndScreen = false;

	canDraw = false;
	resetClock.restart();
}


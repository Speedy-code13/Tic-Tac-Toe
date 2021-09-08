#pragma once
#include "Box.h"
#include "EndGameScreen.h"

struct WinCombination
{
	short unsigned comb1;
	short unsigned comb2;
	short unsigned comb3;

	void init(short unsigned comb1, short unsigned comb2, short unsigned comb3)
	{
		this->comb1 = comb1;
		this->comb2 = comb2;
		this->comb3 = comb3;
	}
	
};
class Game
{
private:
	float width, height;
	sf::Event event;
	sf::Image icon;
	std::shared_ptr<sf::RenderWindow> window;

	std::unordered_map<std::string, sf::Texture> textures;
	sf::Font font;

	sf::Vector2f mousePos;

	sf::Clock dtClock;
	sf::Clock resetClock;
	float dt;

	sf::RectangleShape background;

	std::array<std::unique_ptr<Box>, 9> boxes;

	float boxWidth;
	float boxHeight;

	bool shouldCheckForWin;

	sf::IntRect oRect;
	sf::IntRect xRect;

	std::array<WinCombination, 8> winCombinations;


	Winner winner;
	std::unique_ptr<EndGameScreen> endScreen;

	bool inEndScreen;

	bool canDraw;

private:
	//Initializers
	void initVariables();
	void initWindow();
	void initTextures();
	void initFonts();
	void initBackground();
	void initCombinations();
	void positionBoxes();
	void initBoxes();
	void initEndScreen();
	

	//Core 

	bool checkTeamForWin(State team, int winnerPos1, int winnerPos2, int winnerPos3);
	void updateShouldCheckForWin();
	const bool checkForDraw();
	const Winner& getWinner();

	void reset();

	void updateCanDraw(); 
	void updateEvents();
	void updateWin();
	void updateDt();

	void update();
	void render();
public:
	Game();
	virtual ~Game();

	void run();
};


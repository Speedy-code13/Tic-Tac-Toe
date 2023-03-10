#pragma once
#include "Box.h"
#include "EndGameScreen.h"

struct WinCombination
{
	short unsigned pos1;
	short unsigned pos2;
	short unsigned pos3;

	void init(short unsigned pos1, short unsigned pos2, short unsigned pos3) 
	{
		this->pos1 = pos1, this->pos2 = pos2, this->pos3 = pos3;
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
	std::unordered_map<std::string, sf::Sound> sounds;
	std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
	sf::Font font;

	sf::Vector2f mousePos;

	sf::Clock dtClock;
	sf::Clock resetClock;
	float dt;

	sf::RectangleShape	background;

	std::array<std::unique_ptr<Box>, 9> boxes;

	float boxWidth;
	float boxHeight;

	sf::IntRect oRect;
	sf::IntRect xRect;
	

	std::array<WinCombination, 8> winCombinations;

	GameState gameState;
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
	void initSounds();
	void initSound(std::string name, std::string path, float volume = 100.f);
	

	//Core 

	bool hasTeamWon(BoxState team, int winnerPos1, int winnerPos2, int winnerPos3);
	const bool checkForDraw();
	const GameState& getGameState();
	void forceFinishBoxesAnimations();

	void reset();

	void boxesOnMouseClick();
	void updateCanDraw(); 
	void updateEvents();
	void updateGameState();
	void updateDt();

	void update();
	void render();
public:
	Game();
	virtual ~Game();

	void run();
};


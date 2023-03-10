#pragma once
enum class GameState { Ongoing = 0, Draw, XWin, OWin };
class EndGameScreen
{
private:
	const GameState& gameState;
    sf::RenderWindow& window;
	const sf::Font& font;
	const float& dt;

	std::unordered_map<std::string, sf::Texture> textures;
	std::unordered_map<std::string, sf::Sound>& sounds;
	sf::RectangleShape background;
	sf::RectangleShape reloadButton;

	bool backgroundAnimated;
	float backgroundSpeed;

	sf::Text winText;
	sf::Text scoreText;

	unsigned short scoreX;
	unsigned short scoreO;


	std::function<void()> gameReset;


private:
	void initVariables();
	void initTextures();
	void initBackground();
	void initReloadButton();
	void initText();

	void reset();

public:
	EndGameScreen(const GameState& gameState, sf::RenderWindow& window, const sf::Font& font, 
		const float& dt, std::unordered_map<std::string, sf::Sound>& sounds, std::function<void()> gameReset);

	void onRoundFinish();

	void update(const sf::Vector2f& mousePos);
	void render();
};


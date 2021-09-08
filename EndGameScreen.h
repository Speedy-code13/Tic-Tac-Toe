#pragma once
enum class Winner { None = 0, Draw, X, O };
class EndGameScreen
{
private:
	const Winner& winner;
    sf::RenderWindow& window;
	const sf::Font& font;
	const float& dt;

	std::unordered_map<std::string, sf::Texture> textures;
	sf::RectangleShape background;
	sf::RectangleShape reloadButton;

	bool backgroundLoaded;
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
	EndGameScreen(const Winner& winner, sf::RenderWindow& window, const sf::Font& font, const float& dt, std::function<void()> gameReset);

	void teamHasWon();

	void update(const sf::Vector2f& mousePos);
	void render();
};


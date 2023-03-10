#pragma once
enum class BoxState {None = 0, X, O};
enum class Turn {X = 0, O};

static Turn turn;

class Box
{
private:
	sf::RenderWindow& window;

	sf::RectangleShape shape;
	BoxState state;
	sf::Texture& texture;
	std::unordered_map<std::string, sf::Sound>& sounds;

	const bool& canDraw;
	const float& dt;

	bool drawn;

	bool drawnForWin;

	const sf::IntRect& oRect;
	const sf::IntRect& xRect;


	const sf::Uint8 initialTransparency;
	const float fadeInSpeed;


public:

	Box(sf::Texture& texture, const sf::Vector2f& size, const sf::IntRect& oRect, const sf::IntRect& xRect,
		sf::RenderWindow& window, const bool& canDraw, const float& dt, std::unordered_map<std::string, sf::Sound>& sounds);

	void setPosition(const sf::Vector2f& pos);
	void forceFinishAnimation();

	const bool drawnBox();

	const sf::Vector2f& getPosition() const;
	const BoxState& getState() const;


	void onMouseClick(const sf::Vector2f& mousePos);

	void reset();
	
	void updateAnimation();
	void update(const sf::Vector2f& mousePos);
	void render();
};


#pragma once
enum class State {None = 0, X, O};
enum class Turn {X = 0, O};

static Turn turn;

class Box
{
private:
	sf::RenderWindow& window;

	sf::RectangleShape shape;
	State state;
	sf::Texture& texture;

	const bool& canDraw;

	bool drawn;

	bool drawnForWin;

	const sf::IntRect& oRect;
	const sf::IntRect& xRect;


public:

	Box(sf::Texture& texture, const sf::Vector2f& size, const sf::IntRect& oRect, const sf::IntRect& xRect, sf::RenderWindow& window, const bool& canDraw);

	void setPosition(const sf::Vector2f& pos);


	const bool drawnBox();

	const sf::Vector2f& getPosition() const;
	const State& getState() const;

	void reset();

	void update(const sf::Vector2f& mousePos);
	void render();
};


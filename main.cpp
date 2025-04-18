#include <SFML/Graphics.hpp>
#include "Map.h"
#include <string>
#include <iostream>
#include <cmath>
using namespace sf;

class Pac;
class Ghost;
class Pinky;
class Blinky;
class Clyde;
class Inky;

class Pac {
private:
	CircleShape pacman;
	Vector2f velocity;
	Map* map;

public:
	Pac(Map* gameMap) : pacman(16.f), velocity(-3.f, 0.f), map(gameMap) {
		pacman.setPosition(22 * 15.f + 8.f, 38.f * 15.f + 8.f); // Center in tile
		pacman.setFillColor(Color::Yellow);
		pacman.setOrigin(8.f, 8.f); // Center origin
	}

	void draw(RenderWindow& window) {
		window.draw(pacman);
	}

	void handleInput(Event& event) {
		if (event.type == Event::KeyPressed) {
			Vector2f temp = velocity;
			if (event.key.code == Keyboard::Right)
				velocity = Vector2f(2.f, 0.f);
			else if (event.key.code == Keyboard::Left)
				velocity = Vector2f(-2.f, 0.f);
			else if (event.key.code == Keyboard::Up)
				velocity = Vector2f(0.f, -2.f);
			else if (event.key.code == Keyboard::Down)
				velocity = Vector2f(0.f, 2.f);
			Vector2f checkVelocity(velocity.x * 7, velocity.y * 7);
			Vector2f newPosition = pacman.getPosition() + checkVelocity;
			if (willCollide(newPosition)) {
				velocity = temp;
			}
		}
	}

	void teleport(Vector2f& newPosition) {
		if (newPosition.x < 10) {
			pacman.setPosition(670, newPosition.y);
			pacman.move(velocity);
			newPosition = pacman.getPosition();
		}
		else if (newPosition.x > 670) {
			pacman.setPosition(10, newPosition.y);
			pacman.move(velocity);
			newPosition = pacman.getPosition();
		}
	}

	void update() {
		// Predict new position
		Vector2f newPosition = pacman.getPosition() + velocity;

		teleport(newPosition);

		// Check collision in movement direction
		if (!willCollide(newPosition)) {
			pacman.move(velocity);
		}
	}

private:
	std::string direction() const {
		if (velocity.x > 0) return "Right";
		else if (velocity.x < 0) return "Left";
		else if (velocity.y > 0) return "Down";
		else if (velocity.y < 0) return "Up";
		return "None";
	}

	bool willCollide(const Vector2f& newPosition) {
		// Get the 4 corners of Pacman's collision circle
		float radius = pacman.getRadius();
		Vector2f edges[8] = {
			Vector2f(newPosition.x - radius, newPosition.y - radius), // top-left
			Vector2f(newPosition.x + radius, newPosition.y - radius), // top-right
			Vector2f(newPosition.x - radius, newPosition.y + radius), // bottom-left
			Vector2f(newPosition.x + radius, newPosition.y + radius), // bottom-right
			Vector2f(newPosition.x - radius, newPosition.y), //left
			Vector2f(newPosition.x + radius, newPosition.y), //right
			Vector2f(newPosition.x, newPosition.y - radius), //top
			Vector2f(newPosition.x, newPosition.y + radius), //bottom
		};

		// Check each corner against walls
		for (const auto& edge : edges) {
			int col = static_cast<int>((edge.x / 15.f));
			int row = static_cast<int>((edge.y / 15.f));
			if (map->isWall(row, col)) {
				return true;
			}
		}
		return false;
	}

	friend class Pinky;
	friend class Inky;
	friend class Clyde;
	friend class Blinky;
};

class Ghost {
protected:
	RectangleShape ghost;
	Vector2f position;
	Color color;
	Vector2f velocity;
	float speed;
	Map* map;
	bool isScared = false;

	bool willCollide(const Vector2f& newPosition) const {
		float halfSize = 8.f;
		Vector2f edges[4] = {
			Vector2f(newPosition.x - halfSize, newPosition.y - halfSize),
			Vector2f(newPosition.x + halfSize, newPosition.y - halfSize),
			Vector2f(newPosition.x - halfSize, newPosition.y + halfSize),
			Vector2f(newPosition.x + halfSize, newPosition.y + halfSize)
		};

		for (const auto& edge : edges) {
			int col = static_cast<int>(edge.x / 15.f);
			int row = static_cast<int>(edge.y / 15.f);
			if (map->isWall(row, col)) {
				return true;
			}
		}
		return false;
	}

	void changeDirection() {
		//somelogic
	}

public:

	Ghost(Map* gameMap, Color ghostColor, float startX, float startY) :
		map(gameMap), color(ghostColor) {
		ghost.setSize(Vector2f(32.f, 32.f));
		ghost.setFillColor(ghostColor);
		position = Vector2f(startX * 15.f + 8.f, startY * 15.f + 8.f);
		ghost.setPosition(position);
		ghost.setOrigin(16.f, 16.f);
		velocity = (Vector2f(0.f, 0.f));
	}

	void draw(RenderWindow& window) {
		window.draw(ghost);
	}

	virtual void update() {
		ghost.move(velocity);
	}

	virtual void MoveGhost(Pac& pac) = 0; // pure virtual

	virtual ~Ghost() {}
	Vector2f getPosition() const { return position; }
};

class Blinky : public Ghost {
private:
public:
	Blinky(Map* map) : Ghost(map, Color(255, 0, 0, 255), 43, 2) {}

	void MoveGhost(Pac& pac) override {
		//moves faster than the rest
		//As pacman eats the food it gets faster
		//Use bfs
	}
};

class Pinky : public Ghost {
public:
	Pinky(Map* map) : Ghost(map, Color(246, 87, 214), 22.5, 23) {}

	void MoveGhost(Pac& pac) override
	{
		//pacman up - pinky 4 up and 4 left
		//pacman down - pinky 4 down of pacman
		//pacman left - pinky 4 left of pacman
		//pacman right - pinky 4 right of pacman
	};
};

class Inky : public Ghost {
public:
	Inky(Map* map) : Ghost(map, Color::Cyan, 19.5, 23) {}

	void MoveGhost(Pac& pac) override {
		/*He uses both Pac-Man's position and Blinky’s position to calculate a target tile.
		He imagines a point two tiles ahead of Pac-Man (based on direction).
		Then, he forms a vector from Blinky’s position to that point, doubles it, and uses the resulting tile as his chase target.*/
	}
};

class Clyde : public Ghost {
public:
	Clyde(Map* map) : Ghost(map, Color(255, 165, 0), 25.5, 23) {}

	void MoveGhost(Pac& pac) override {
		/*If he's far from Pac-Man (more than 8 tiles), he chases Pac-Man like Blinky.
		If he's close to Pac-Man (within 8 tiles), he gets scared and retreats to his scatter corner (bottom-left of the map).
		Steps:
		1.Calculate the distance between Clyde and Pac-Man.
		2.If the distance is greater than 8 tiles, use BFS to chase Pac-Man.
		3.If the distance is less than or equal to 8 tiles, use BFS to move toward a fixed corner (e.g., tile (0, mapHeight - 1)).*/
	}
};


class GameSimulation {};

int main() {
	RenderWindow window(VideoMode(690, 765), "PAC-MAN");
	window.setFramerateLimit(60);

	Map map;
	Pac pac(&map);
	Blinky shadow(&map);
	Clyde pokey(&map); //pookie ghost hehe :ribbon
	Pinky speedy(&map);
	Inky bashful(&map);


	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			pac.handleInput(event);
		}

		pac.update();

		window.clear(Color::Black);
		map.draw(window);
		pac.draw(window);
		shadow.draw(window);
		pokey.draw(window);
		speedy.draw(window);
		bashful.draw(window);
		window.display();
	}

	return 0;
}

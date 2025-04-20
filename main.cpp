// Hatim Check exactly WHY the ghosts are rubbing against the walls 

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <string>
#include <iostream>
#include <cmath>
#include <queue>
#include <map>
#include <set>

using namespace sf;

class Pac;
class Ghost;
class Pinky;
class Blinky;
class Clyde;
class Inky;
int const khaana = 184;


// A rather Interesting Concept To
//template <typename T>
//T clamp(T value, T min, T max) {
//	return (value < min) ? min : (value > max) ? max : value;
//}

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

	float getR() {
		return pacman.getRadius();
	}

	Vector2f getP() {
		return pacman.getPosition();
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

	// I added this getVelocity function here

	Vector2f getVelocity() {
		return velocity;
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
	friend class Food;
};

class Food {
	CircleShape f;
	Map* fmap;
	Pac* pman;
	bool isEaten;
	int rows, cols;

public:
	Food() {}
	Food(Map* fmap, Pac* pman, int rows, int cols) :isEaten(false), fmap(fmap), pman(pman), rows(rows), cols(cols), f(3.f) {

		f.setFillColor(sf::Color::White);
		f.setPosition(cols * 15.f + 5.f, rows * 15.f + 5.f);
	}

	bool willEat(const Vector2f& newPosition) {

		float radius = pman->getR();
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

		// Check each corner against food
		for (const auto& edge : edges) {
			int col = static_cast<int>(((edge.x + 8.f) / 15.f));
			int row = static_cast<int>(((edge.y + 8.f) / 15.f));
			if (row == rows && col == cols) {
				return true;
			}
		}
		return false;
	}

	friend class FoodManager;

};

class FoodManager {
private:
	Food* food;
	Map* fmap;
	Pac* pman;
	Vector2f velocity;
	int score = 0;


public:
	FoodManager(Food* food, Map* fmap, Pac* pman) : food(food), fmap(fmap), pman(pman), velocity(-3.f, 0.f) {
	}

	void Score() {
		// Predict new position
		Vector2f newPosition = pman->getP() + velocity;

		pman->teleport(newPosition);

		// update score when food is eaten
		for (int k = 0; k < khaana; ++k) {
			if (food[k].willEat(newPosition) && !food[k].isEaten) {
				score += 10;
				food[k].isEaten = true;
			}
		}
	}
	void draw(RenderWindow& window) const {
		for (int k = 0; k < khaana; ++k) {
			if (!food[k].isEaten) {
				window.draw(food[k].f);
			}
		}
	}

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

	// BFS LOGIC WHICH I UNDERSTOOD (I should be graded additional marks in DSA) ~Fuzail Raza

	std::vector<Vector2f> bfs(const Vector2f& start, const Vector2f& target) {
		int startCol = static_cast<int>(start.x / 15.f);
		int startRow = static_cast<int>(start.y / 15.f);
		int targetCol = static_cast<int>(target.x / 15.f);
		int targetRow = static_cast<int>(target.y / 15.f);

		std::queue<std::pair<int, int>> q;
		std::map<std::pair<int, int>, std::pair<int, int>> parent;
		std::vector<std::vector<bool>> visited(51, std::vector<bool>(46, false));

		q.push({ startRow, startCol });
		visited[startRow][startCol] = true;

		int dr[4] = { -1, 1, 0, 0 };
		int dc[4] = { 0, 0, -1, 1 };

		while (!q.empty()) {
			std::pair<int, int> current = q.front(); q.pop();
			int r = current.first;
			int c = current.second;

			if (r == targetRow && c == targetCol)
				break;

			for (int i = 0; i < 4; i++) {
				int nr = r + dr[i];
				int nc = c + dc[i];

				if (nr >= 0 && nr < 51 && nc >= 0 && nc < 46 && !map->isWall(nr, nc) && !visited[nr][nc]) {
					visited[nr][nc] = true;
					q.push({ nr, nc });
					parent[{nr, nc}] = { r, c };
				}
			}
		}

		std::vector<Vector2f> path;
		std::pair<int, int> current = { targetRow, targetCol };

		while (current != std::make_pair(startRow, startCol)) {
			path.push_back(Vector2f(current.second * 15.f + 8.f, current.first * 15.f + 8.f));
			if (parent.find(current) == parent.end()) break;
			current = parent[current];
		}

		std::reverse(path.begin(), path.end());
		return path;
	}


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

	// lITERALLY 0 POINT OF THIS FUNCTION...

	//void changeDirection() {
	//	//somelogic
	//}

public:

	Ghost(Map* gameMap, Color ghostColor, float startX, float startY, Vector2f initialVelocity) :
		map(gameMap), color(ghostColor), velocity(initialVelocity) {
		ghost.setSize(Vector2f(32.f, 32.f));
		ghost.setFillColor(ghostColor);
		position = Vector2f(startX * 15.f + 8.f, startY * 15.f + 8.f);
		ghost.setPosition(position);
		ghost.setOrigin(16.f, 16.f);
		speed = 0.75f;  // basic speed
	}


	void draw(RenderWindow& window) {
		window.draw(ghost);
	}

	void update() {
		Vector2f newPos = ghost.getPosition() + velocity;
		if (willCollide(newPos)) {
			velocity = -velocity;
		}
		else {
			ghost.move(velocity);
		}
	}

	virtual void MoveGhost(Pac& pac) = 0; // pure virtual

	virtual ~Ghost() {}
	Vector2f getPosition() const { return position; }
};

class Blinky : public Ghost {
private:
public:
	Blinky(Map* map) : Ghost(map, Color(255, 0, 0, 255), 22.5, 18.5, Vector2f(-1.5f, 0.f)) {}

	void MoveGhost(Pac& pac) override {
		Vector2f startPos = ghost.getPosition();
		Vector2f targetPos = pac.getP();

		std::vector<Vector2f> path = bfs(startPos, targetPos);

		if (!path.empty()) {
			Vector2f next = path[0];
			Vector2f dir = next - startPos;

			float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
			if (length != 0)
				dir /= length;

			velocity = dir * 0.75f;
			ghost.move(velocity);
		}
	}

};

class Pinky : public Ghost {
private:
	// Will consider the timings thoroughly later

	bool released = false;
	sf::Clock releaseClock;

public:
	Pinky(Map* map) : Ghost(map, Color(246, 87, 214), 22.5, 23, Vector2f(0.f, 0.f)) {
		releaseClock.restart();
	}

	void MoveGhost(Pac& pac) override {
		Vector2f startPos = ghost.getPosition();
		Vector2f pacPos = pac.getP();
		Vector2f pacVel = pac.getVelocity();

		// Took everything from PacMan cuz uss nay mayray liyay kya chora tha </3

		Vector2f predictedPos = pacPos + pacVel * 4.f;

		// Predicted pos is basically 4 tiles ahead of where the pacman is facing 

		std::vector<Vector2f> path = bfs(startPos, predictedPos);

		if (!path.empty()) {
			Vector2f next = path[0];
			Vector2f dir = next - startPos;

			float length = std::sqrt(dir.x * dir.x + dir.y * dir.y); // Distance formula to check if it is 0 or nah
			if (length != 0) {
				dir /= length;
			}

			velocity = dir * 0.70f;  // Apparantely Pinky slower than Blinky
			ghost.move(velocity);
		}
	}

	
};


class Inky : public Ghost {
public:
	Inky(Map* map) : Ghost(map, Color::Cyan, 19.5, 23, Vector2f(0.f, 1.5f)) {}

	void MoveGhost(Pac& pac) override {
		// Uses logic of Blinky and Pinky
		// Will most likely try to implement soon... 
	}
};

class Clyde : public Ghost {
public:
	Clyde(Map* map) : Ghost(map, Color(255, 165, 0), 25.5, 23, Vector2f(0.f, -1.5f)) {}

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

	Clock clock;

	Map map;
	Pac pac(&map);
	Blinky shadow(&map);
	Clyde pokey(&map); //pookie ghost hehe :ribbon (why are you adding dumb comments to this very serious project of ours seniya  -zaid)
	Pinky speedy(&map);
	Inky bashful(&map);
	Food f[khaana];
	int foodIndex = 0;
	for (int i = 0; i < 51; ++i) {
		for (int j = 0; j < 46; ++j) {
			if (map.isFood(i, j) && foodIndex < khaana) {
				f[foodIndex] = Food(&map, &pac, i, j);
				++foodIndex;
			}
		}
	}


	FoodManager manage(f, &map, &pac);

	

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			pac.handleInput(event);
		}

		pac.update();
		manage.Score();

		// GHOST CHASE TIME >v<
		shadow.MoveGhost(pac); // Blinky chases Pac-Man
		speedy.MoveGhost(pac);  // Pinky bbg traps Pac-Man


		window.clear(Color::Black);
		map.draw(window);
		pac.draw(window);
		manage.draw(window);
		pokey.draw(window);
		speedy.draw(window);
		bashful.draw(window);
		shadow.draw(window);

		shadow.update();
		speedy.update();
		bashful.update();
		pokey.update();


		window.display();

	}

	return 0;
}

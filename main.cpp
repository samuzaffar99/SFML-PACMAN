#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>
#include "map.h"
using namespace sf;

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
        /*if (newPosition.x == 10 || newPosition.x == 680) {
            return false;
        }
        if (newPosition.x < 10) {
            pacman.setPosition(680, newPosition.y);
            return false;
        }
        if (newPosition.x > 680) {
            pacman.setPosition(10, newPosition.y);
            return false;
        }*/
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
};


int main() {
    RenderWindow window(VideoMode(690, 765), "PAC-MAN");
    window.setFramerateLimit(60);

    Map map;
    Pac pac(&map);

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
        window.display();
    }

    return 0;
}
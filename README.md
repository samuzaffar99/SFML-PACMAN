# SFML-PACMAN
Pacman game made in C++ and SFML

git clone https://github.com/Hatim-Mustafa/SFML-PACMAN.git
git submodule add -b 2.6.x https://github.com/SFML/SFML.git external/SFML

# From project root
git submodule update --init --recursive

mkdir build
cd build
cmake ..
cmake --build .


Blinky will target Pac-Man directly.
Pinky will target 4 tiles ahead of Pac-Man.
Inky's targeting often involves Blinky's position and a point 2 tiles in front of Pac-Man.
Clyde will target Pac-Man directly if far away, and a scatter target if close.
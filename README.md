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

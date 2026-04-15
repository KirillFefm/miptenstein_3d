# miptenstein_3d
my and marats project on 4 sem of cpp
Тактический шутер от первого лица в стиле Wolfenstein 3D.
git clone https://github.com/KirillFefm/miptenstein_3d.git
cd miptenstein_3d

mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DSFML_DIR="C:/msys64/ucrt64/lib/cmake/SFML"
mingw32-make

./Wolfenstein3D.exe

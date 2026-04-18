# miptenstein_3d
Тактический шутер от первого лица в стиле Wolfenstein 3D.
0.0.1 - самая первая версия 
0.0.2 - добавленны анимации смерти противников и трассеры выстрелов 
git clone https://github.com/KirillFefm/miptenstein_3d.git
cd miptenstein_3d

mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DSFML_DIR="C:/msys64/ucrt64/lib/cmake/SFML"
mingw32-make

./Wolfenstein3D.exe

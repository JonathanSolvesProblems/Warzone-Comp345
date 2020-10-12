echo Creating build/PlayerDriver Directory
mkdir -p build/PlayerDriver

echo Compiling Player.cpp, Card.cpp, Orders.cpp, Map.cpp and MapLoader.cpp to object file
g++ -I ./headers -o build/PlayerDriver/Player.o -c src/Player.cpp
g++ -I ./headers -o build/PlayerDriver/Map.o -c src/Map.cpp
g++ -I ./headers -o build/PlayerDriver/Orders.o -c src/Orders.cpp
g++ -I ./headers -o build/PlayerDriver/Card.o -c src/Card.cpp
g++ -I ./headers -o build/PlayerDriver/MapLoader.o -c src/MapLoader.cpp

echo Compiling and Linking PlayerDriver.cpp with Map.o \& MapLoader.o \& Card.o \& Orders.o \& Player.o
g++ -I ./headers -o PlayerDriver drivers/PlayerDriver.cpp build/PlayerDriver/Map.o build/PlayerDriver/MapLoader.o build/PlayerDriver/Player.o build/PlayerDriver/Orders.o build/PlayerDriver/Card.o

echo Cleaning up...
rm build/PlayerDriver/*.o
rmdir build/PlayerDriver

echo Done.
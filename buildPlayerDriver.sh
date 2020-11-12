echo Creating build/PlayerDriver Directory
mkdir -p build/PlayerDriver

echo Compiling to objects file
g++ -I ./headers -o build/PlayerDriver/Map.o -c src/Map.cpp
g++ -I ./headers -o build/PlayerDriver/Player.o -c src/Player.cpp
g++ -I ./headers -o build/PlayerDriver/Card.o -c src/Card.cpp
g++ -I ./headers -o build/PlayerDriver/Orders.o -c src/Orders.cpp
g++ -I ./headers -o build/PlayerDriver/MapLoader.o -c src/MapLoader.cpp

echo Compiling and Linking PlayerDriver.cpp with objects files.
g++ -I ./headers -o PlayerDriver drivers/PlayerDriver.cpp build/PlayerDriver/*.o

echo Cleaning up...
rm build/PlayerDriver/*.o
rmdir build/PlayerDriver

echo Done.
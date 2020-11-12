echo Creating build/MapDriver Directory
mkdir -p build/MapDriver

echo Compiling to objects file
g++ -I ./headers -o build/MapDriver/Map.o -c src/Map.cpp
g++ -I ./headers -o build/MapDriver/Player.o -c src/Player.cpp
g++ -I ./headers -o build/MapDriver/Card.o -c src/Card.cpp
g++ -I ./headers -o build/MapDriver/Orders.o -c src/Orders.cpp
g++ -I ./headers -o build/MapDriver/MapLoader.o -c src/MapLoader.cpp

echo Compiling and Linking MapDriver.cpp with objects files.
g++ -I ./headers -o MapDriver drivers/MapDriver.cpp build/MapDriver/*.o

echo Cleaning up...
rm build/MapDriver/*.o
rmdir build/MapDriver

echo Done.
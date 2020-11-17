echo Creating build/MapLoaderDriver Directory
mkdir -p build/MapLoaderDriver

echo Compiling to objects file
g++ -I ./headers -o build/MapLoaderDriver/Map.o -c src/Map.cpp
g++ -I ./headers -o build/MapLoaderDriver/Player.o -c src/Player.cpp
g++ -I ./headers -o build/MapLoaderDriver/Card.o -c src/Card.cpp
g++ -I ./headers -o build/MapLoaderDriver/Orders.o -c src/Orders.cpp
g++ -I ./headers -o build/MapLoaderDriver/MapLoader.o -c src/MapLoader.cpp

echo Compiling and Linking MapLoaderDriver.cpp with objects files.
g++ -I ./headers -o MapLoaderDriver drivers/MapLoaderDriver.cpp build/MapLoaderDriver/*.o -lstdc++fs

echo Cleaning up...
rm build/MapLoaderDriver/*.o
rmdir build/MapLoaderDriver

echo Done.
echo Creating build/MapLoaderDriver Directory
mkdir build/MapLoaderDriver

echo Compiling Map.cpp and MapLoader.cpp to object file
g++ -I ./headers -o build/MapLoaderDriver/Map.o -c src/Map.cpp
g++ -I ./headers -o build/MapLoaderDriver/MapLoader.o -c src/MapLoader.cpp

echo Compiling and Linking MapDriver.cpp with Map.o \& MapLoader.o
g++ -I ./headers -o MapLoaderDriver drivers/MapLoaderDriver.cpp build/MapLoaderDriver/Map.o build/MapLoaderDriver/MapLoader.o

echo Cleaning up...
rm build/MapLoaderDriver/*.o
rmdir build/MapLoaderDriver

echo Done.
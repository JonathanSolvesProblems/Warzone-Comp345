echo Creating build\MapLoaderDriver Directory
mkdir build\MapLoaderDriver

echo Compiling to objects file
g++ -std=c++17 -I .\headers -o build\MapLoaderDriver\Map.o -c src\Map.cpp
g++ -std=c++17 -I .\headers -o build\MapLoaderDriver\Player.o -c src\Player.cpp
g++ -std=c++17 -I .\headers -o build\MapLoaderDriver\Card.o -c src\Card.cpp
g++ -std=c++17 -I .\headers -o build\MapLoaderDriver\Orders.o -c src\Orders.cpp
g++ -std=c++17 -I .\headers -o build\MapLoaderDriver\MapLoader.o -c src\MapLoader.cpp

echo Compiling and Linking MapLoaderDriver.cpp with objects files.
g++ -std=c++17 -I .\headers -o MapLoaderDriver drivers\MapLoaderDriver.cpp build\MapLoaderDriver\*.o

echo Cleaning up...
del build\MapLoaderDriver\*.o
rmdir build\MapLoaderDriver

echo Done.
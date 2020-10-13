echo Creating build\CardDriver Directory
mkdir build\CardDriver

echo Compiling to objects file
g++ -I .\headers -o build\CardDriver\Map.o -c src\Map.cpp
g++ -I .\headers -o build\CardDriver\Player.o -c src\Player.cpp
g++ -I .\headers -o build\CardDriver\Card.o -c src\Card.cpp
g++ -I .\headers -o build\CardDriver\Orders.o -c src\Orders.cpp
g++ -I .\headers -o build\CardDriver\MapLoader.o -c src\MapLoader.cpp

echo Compiling and Linking CardDriver.cpp with objects files.
g++ -I .\headers -o CardDriver drivers\CardDriver.cpp build\CardDriver\*.o

echo Cleaning up...
del build\CardDriver\*.o
rmdir build\CardDriver

echo Done.
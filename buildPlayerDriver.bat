mkdir build\PlayerDriver

g++ -I .\headers -o build\PlayerDriver\Map.o -c src\Map.cpp
g++ -I .\headers -o build\PlayerDriver\Player.o -c src\Player.cpp
g++ -I .\headers -o build\PlayerDriver\Card.o -c src\Card.cpp
g++ -I .\headers -o build\PlayerDriver\Orders.o -c src\Orders.cpp

g++ -I .\headers -o build\PlayerDriver\Application.o -c src\Application.cpp
g++ -I .\headers -o build\PlayerDriver\GameEngine.o -c src\GameEngine.cpp

g++ -I .\headers -o build\PlayerDriver\GameObservers.o -c src\GameObservers.cpp

g++ -std=c++17 -I .\headers -o PlayerDriver drivers\PlayerDriver.cpp src\MapLoader.cpp build\PlayerDriver\*.o -lpdcurses -g

del build\PlayerDriver\*.o
rmdir build\PlayerDriver
pause
mkdir build\OrdersDriver

g++ -I .\headers -o build\OrdersDriver\Map.o -c src\Map.cpp
g++ -I .\headers -o build\OrdersDriver\Player.o -c src\Player.cpp
g++ -I .\headers -o build\OrdersDriver\Card.o -c src\Card.cpp
g++ -I .\headers -o build\OrdersDriver\Orders.o -c src\Orders.cpp
g++ -std=c++17 -I .\headers -o build\OrdersDriver\MapLoader.o -c src\MapLoader.cpp
g++ -I .\headers -o build\OrdersDriver\Observers.o -c src\GameObservers.cpp 

g++ -I .\headers -o OrdersDriver drivers\OrdersDriver.cpp build\OrdersDriver\*.o -lpdcurses -g


del build\OrdersDriver\*.o
rmdir build\OrdersDriver

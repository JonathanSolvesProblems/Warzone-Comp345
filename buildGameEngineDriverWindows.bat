mkdir build\GameEngineDriver

g++ -I .\headers -o build\GameEngineDriver\Map.o -c src\Map.cpp
g++ -I .\headers -o build\GameEngineDriver\Player.o -c src\Player.cpp
g++ -I .\headers -o build\GameEngineDriver\Card.o -c src\Card.cpp
g++ -I .\headers -o build\GameEngineDriver\Orders.o -c src\Orders.cpp
g++ -std=c++17 -I .\headers -o build\GameEngineDriver\MapLoader.o -c src\MapLoader.cpp
g++ -I .\headers -o build\GameEngineDriver\Observers.o -c src\GameObservers.cpp 
g++ -I .\headers -o build\GameEngineDriver\Application.o -c src\Application.cpp 
g++ -I .\headers -o build\GameEngineDriver\GameEngine.o -c src\GameEngine.cpp 


g++ -I .\headers -o GameEngineDriver drivers\GameEngineDriver.cpp build\GameEngineDriver\*.o -lpdcurses -g


del build\GameEngineDriver\*.o
rmdir build\GameEngineDriver


pause
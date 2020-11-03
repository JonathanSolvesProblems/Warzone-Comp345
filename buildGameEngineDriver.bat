mkdir build\GameEngineDriver

g++ -I .\headers -o build\GameEngineDriver\Observers.o -c src/Observers.cpp
g++ -I .\headers -o build\GameEngineDriver\Application.o -c src\Application.cpp
g++ -I .\headers -o build\GameEngineDriver\GameEngine.o -c src\GameEngine.cpp

g++ -I .\headers -o GameEngineDriver drivers\GameEngineDriver.cpp build\GameEngineDriver\*.o -lpdcurses 

del build\GameEngineDriver\*.o
rmdir build\GameEngineDriver
pause

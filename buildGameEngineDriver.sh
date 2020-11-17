echo Checking dependencies...

cnt=$(locate ncurses | wc -l);
cnt=$(($cnt + 0));
if [ "$cnt" -eq "0" ]; then
  echo Installing ncurses...
  apt-get install libncurses5-dev libncursesw5-dev
fi

echo Creating build/GameEngineDriver Directory
mkdir -p build/GameEngineDriver

echo Compiling and Linking GameEngineDriver.cpp with objects files.
g++ -I ./headers -o build/GameEngineDriver/Map.o -c src/Map.cpp
g++ -I ./headers -o build/GameEngineDriver/Player.o -c src/Player.cpp
g++ -I ./headers -o build/GameEngineDriver/Card.o -c src/Card.cpp
g++ -I ./headers -o build/GameEngineDriver/Orders.o -c src/Orders.cpp

g++ -I ./headers -o build/GameEngineDriver/Observers.o -c src/GameObservers.cpp
g++ -I ./headers -o build/GameEngineDriver/Application.o -c src/Application.cpp
g++ -I ./headers -o build/GameEngineDriver/GameEngine.o -c src/GameEngine.cpp

g++ -std=gnu++17 -I ./headers -o GameEngineDriver drivers/GameEngineDriver.cpp src/MapLoader.cpp build/GameEngineDriver/*.o -lstdc++fs -lcurses  $@

echo Cleaning up...
rm build/GameEngineDriver/*.o
rmdir build/GameEngineDriver
echo Done.

echo Checking dependencies...

cnt=$(locate ncurses | wc -l);
cnt=$(($cnt + 0));
if [ "$cnt" -eq "0" ]; then
  echo Installing ncurses...
  apt-get install libncurses5-dev libncursesw5-dev
fi

echo Creating build/PlayerDriver Directory
mkdir -p build/PlayerDriver

echo Compiling and Linking PlayerDriver.cpp with objects files.
g++ -I ./headers -o build/PlayerDriver/Map.o -c src/Map.cpp
g++ -I ./headers -o build/PlayerDriver/Player.o -c src/Player.cpp
g++ -I ./headers -o build/PlayerDriver/Card.o -c src/Card.cpp
g++ -I ./headers -o build/PlayerDriver/Orders.o -c src/Orders.cpp

g++ -I ./headers -o build/PlayerDriver/Observers.o -c src/GameObservers.cpp
g++ -I ./headers -o build/PlayerDriver/Application.o -c src/Application.cpp
g++ -I ./headers -o build/PlayerDriver/GameEngine.o -c src/GameEngine.cpp

g++ -std=gnu++17 -I ./headers -o PlayerDriver drivers/PlayerDriver.cpp src/MapLoader.cpp build/PlayerDriver/*.o -lstdc++fs -lcurses  $@

echo Cleaning up...
rm build/PlayerDriver/*.o
rmdir build/PlayerDriver
echo Done.

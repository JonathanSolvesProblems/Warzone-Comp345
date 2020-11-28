echo Checking dependencies...

cnt=$(locate ncurses | wc -l);
cnt=$(($cnt + 0));
if [ "$cnt" -eq "0" ]; then
  echo Installing ncurses...
  apt-get install libncurses5-dev libncursesw5-dev
fi

echo Creating build/$1 Directory
mkdir -p build/$1

echo Compiling and Linking $1.cpp with objects files.
g++ -I ./headers -o build/$1/Map.o -c src/Map.cpp
g++ -I ./headers -o build/$1/Player.o -c src/Player.cpp
g++ -I ./headers -o build/$1/Card.o -c src/Card.cpp
g++ -I ./headers -o build/$1/Orders.o -c src/Orders.cpp
g++ -I ./headers -o build/$1/PlayerStrategies.o -c src/PlayerStrategies.cpp

g++ -I ./headers -o build/$1/Observers.o -c src/GameObservers.cpp
g++ -I ./headers -o build/$1/Application.o -c src/Application.cpp
g++ -I ./headers -o build/$1/GameEngine.o -c src/GameEngine.cpp
g++ -I ./headers -o build/$1/GameModels.o -c src/GameModels.cpp

g++ -std=gnu++17 -I ./headers -o $1 drivers/$1.cpp src/MapLoader.cpp build/$1/*.o -lstdc++fs -lcurses  ${@:2}

echo Cleaning up...
rm build/$1/*.o
rmdir build/$1
echo Done.

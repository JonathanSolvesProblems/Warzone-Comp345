echo Checking dependencies...

cnt=$(locate ncurses | wc -l);
cnt=$(($cnt + 0));
if [ "$cnt" -eq "0" ]; then
  echo Installing ncurses...
  apt-get install libncurses5-dev libncursesw5-dev
fi

echo Creating build/GameEngineDriver Directory
mkdir -p build/GameEngineDriver

echo Compiling to objects file
g++ -I ./headers -o build/GameEngineDriver/Observers.o -c src/Observers.cpp
g++ -I ./headers -o build/GameEngineDriver/Application.o -c src/Application.cpp
g++ -I ./headers -o build/GameEngineDriver/GameEngine.o -c src/GameEngine.cpp

echo Compiling and Linking GameEngineDriver.cpp with objects files.
g++ -I ./headers -o GameEngineDriver drivers/GameEngineDriver.cpp build/GameEngineDriver/*.o -lmenu -lcurses $@

echo Cleaning up...
rm build/GameEngineDriver/*.o
rmdir build/GameEngineDriver

echo Done.
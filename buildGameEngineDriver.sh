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
if g++ -I ./headers -o GameEngineDriver drivers/GameEngineDriver.cpp src/GameObservers.cpp src/Application.cpp  src/GameEngine.cpp -lcurses $@; then
  echo Compilation Successful.
else
  echo Compilation Failed.
fi
  echo Cleaning up...
  rm build/GameEngineDriver/*.o
  rmdir build/GameEngineDriver
  echo Done.

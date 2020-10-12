echo Creating build\MapDriver Directory
mkdir build\MapDriver

echo Compiling Map.cpp to object file
g++ -I .\headers -o build\MapDriver\Map.o -c src\Map.cpp

echo Compiling and Linking MapDriver.cpp and Map.o
g++ -I .\headers -o MapDriver drivers\MapDriver.cpp build\MapDriver\Map.o

echo Cleaning up...
del build\MapDriver\*.o
rmdir build\MapDriver

echo Done.
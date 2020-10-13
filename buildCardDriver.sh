echo Creating build/CardDriver Directory
mkdir -p build/CardDriver

echo Compiling Card.cpp to object file
g++ -I ./headers -o build/CardDriver/Card.o -c src/Card.cpp
g++ -I ./headers -o build/CardDriver/Orders.o -c src/Orders.cpp

echo Compiling and Linking CardDriver.cpp and Card.o
g++ -I ./headers -o CardDriver drivers/CardDriver.cpp build/CardDriver/Card.o build/CardDriver/Orders.o

echo Cleaning up...
rm build/CardDriver/*.o
rmdir build/CardDriver

echo Done.
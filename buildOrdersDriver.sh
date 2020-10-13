echo Creating build/OrdersDriver Directory
mkdir -p build/OrdersDriver

echo Compiling to objects file
g++ -I ./headers -o build/OrdersDriver/Map.o -c src/Map.cpp
g++ -I ./headers -o build/OrdersDriver/Player.o -c src/Player.cpp
g++ -I ./headers -o build/OrdersDriver/Card.o -c src/Card.cpp
g++ -I ./headers -o build/OrdersDriver/Orders.o -c src/Orders.cpp
g++ -I ./headers -o build/OrdersDriver/MapLoader.o -c src/MapLoader.cpp

echo Compiling and Linking OrdersDriver.cpp with objects files.
g++ -I ./headers -o OrdersDriver drivers/OrdersDriver.cpp build/OrdersDriver/*.o

echo Cleaning up...
rm build/OrdersDriver/*.o
rmdir build/OrdersDriver

echo Done.
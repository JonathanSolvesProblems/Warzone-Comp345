echo Creating build/OrdersDriver Directory
mkdir build/OrdersDriver

echo Compiling Orders.cpp to object file
g++ -I ./headers -o build/OrdersDriver/Orders.o -c src/Orders.cpp

echo Compiling and Linking OrdersDriver.cpp and Orders.o
g++ -I ./headers -o OrdersDriver drivers/OrdersDriver.cpp build/OrdersDriver/Orders.o

echo Cleaning up...
rm build/OrdersDriver/*.o
rmdir build/OrdersDriver

echo Done.
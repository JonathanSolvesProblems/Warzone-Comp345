mkdir -p build/MapDriver
g++ -I ./headers -o build/MapDriver/Map.o -c src/Map.cpp
g++ -I ./headers -o MapDriver drivers/MapDriver.cpp build/MapDriver/Map.o
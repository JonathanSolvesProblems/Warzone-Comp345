#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale> 
#include "map/Map.h"
#include "map/Continent.h"
#include "map/Territory.h"


using namespace std;



class mapLoader {



  public:
  
  vector<string> continents; 
  vector<string> borders;    
  vector<string> countries;       
 
  // map test 
public: void loadFile(string filePath, map::Map& test, map::Continent &continent, map::Territory& territory);
// loadFile method which loads/checks the mapfile for all the contents (reads line by line)
public: void isBorder(string line, map::Map& test, map::Territory& territory, map::Continent& continent);// isBorder method which checks if the string is a border
// we can first start by checkng array content 
// in this case we can just check to see if the sequence of strings follow the pattern
public: void isCountry(string line, map::Map& test, map::Territory& territory, map::Continent& continent);// isCountry method whih checks if the string is a country 
// isContinent method whcih checks if the strings is a continent 
public: void isContinent(string line, map::Map& test,map::Continent &continent);



};

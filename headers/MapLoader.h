#pragma once
#pragma once
#include "Map.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

using std::endl;
using std::ostream;
using std::string;

class MapLoader
{

public:
	// default constructor
	MapLoader();

	//copy constructor
	MapLoader(const MapLoader& toCopy);

	// OS stream operator
	friend ostream& operator<<(ostream& out, const MapLoader& o);

	//assignment operator
	MapLoader& operator=(const MapLoader& o);

	//destructor
	~MapLoader();

	vector<string> continents;
	vector<string> borders;
	vector<string> countries;

	// map test
	int getNumOfContinents();
	int getNumOfTerritories();

	bool loadFile(string filePath, map::Map& test);
	// loadFile method which loads/checks the mapfile for all the contents (reads line by line)
	bool isBorder(string line, map::Map& test, bool& isValid); // isBorder method which checks if the string is a border
	bool isCountry(string line, map::Map& test, bool& isValid); // isCountry method whih checks if the string is a country
	bool isContinent(string line, map::Map& test, bool& isValid, int& continentID); // isContinent method whcih checks if the strings is a continent
	bool mapSelection(map::Map& test);
	vector<string> findMapFiles();

	// adds the parsed line to the map as a country
	bool isCountryAddToMap(map::Map& test, bool& hasFirstInt, bool& hasString, bool& hasSecondInt, bool& hasThirdInt, bool& hasFourthInt, vector<string>& countriesArr, int& y, int& val0);
	// checks the parsed line to see if it follows correct file syntax
	void isCountryLineValidator(int& val0, int& val2, bool& hasFirstInt, bool& hasString, bool& hasSecondInt, bool& hasThirdInt, bool& hasFourthInt, vector<string>& countriesArr);
	// parses the line and stores it in its respective vector
	void isCountryVector(vector<string>& countriesArr, string line, bool& isValid, int& y);
	// parses the line and stores it in its repective array
	void isContinentArray(string* continentsArr, string line, int& i);
	// checks the parsed line and sees if it follows correct file syntax
	void isContinentLineValidator(bool& hasFirstString, bool& hasInt, bool& hasSecondString, int& val1, string* continentsArr);
	// adds the parsed line to the map as a Continent
	bool isContinentAddToMap(string line, map::Map& test, bool& hasFirstString, bool& hasInt, bool& hasSecondString, int& i, int& val1, string* continentsArr, vector<string>& continents, int& continentID);
	// adds the parsed line to the map as a Border
	bool isBorderAddToMap(string line, map::Map& test, vector<string>& bordersArrReborn, bool& foundBorders, vector<string>& borders);
	// checks the parsed line to check to see if it follows correct file syntax 
	void isBorderLineValidator(vector<string>& bordersArrReborn, string line, bool& foundBorders);
	// parses the line and stores it in its respective array 
	void isBorderArray(vector<string>& bordersArrReborn, string line, bool& isValid);
};



// TODO
// --------------------------------------
class ConquestFileReader {
private:
	vector<string> continents;
	vector<string> borders;
	vector<string> countries;

public:
	// Implements code to read Conquest map files.
	bool loadFile(string mapName, map::Map& test);

	// stores all the components of the line for the isCountry method into the countriesArr vector
	void isContinentArray(string* continentsArr, string line, int& i);
	//validates the line used for the continent part
	void isContinentLineValidator(bool& hasFirstString, bool& hasInt, int& val1, string* continentsArr);
	bool isContinentAddToMap(string line, map::Map& test, bool& hasFirstString, bool& hasInt, int& i, int& val1, string* continentsArr, vector<string>& continents, int& continentID);
	bool isContinent(string line, map::Map& test, bool& isValid, int& continentID);
	int getNumOfContinents();
	int getNumOfTerritories();


	//Get the territory 
	bool isCountry(string line, map::Map& test, bool& isValid, int& territoryID);
	// adds the parsed line to the map as a country
	bool isCountryAddToMap(map::Map& test, bool& hasFirstInt, bool& hasString, bool& hasSecondInt, vector<string>& countriesArr, int& val0, int& val2, int& territoryID);
	// checks the parsed line to see if it follows correct file syntax
	void isCountryLineValidator(int& val0, int& val2, bool& hasFirstInt, bool& hasString, bool& hasSecondInt, bool& hasThirdInt, bool& hasFourthInt, vector<string>& countriesArr);
	// parses the line and stores it in its respective vector
	void isCountryVector(vector<string>& countriesArr, string line, bool& isValid);
	// parses the line and stores it in its repective array

};



class ConquestFileReaderAdapter : public MapLoader {
private:
	ConquestFileReader* conquestFileReader;
	vector<string> continents;
	vector<string> borders;
	vector<string> countries;

public:
	/*
	Needs to override methods in MapLoader class to call conquestFileReader->method
	(will have to make methods + destructor virtual in maploader class)

	Example:
	An overidden method bool mapSelection(map::Map &test) will call conquestFileReader->mapSelection(map::Map &test);
	*/

	// map test
	
	bool loadFile(string mapName, map::Map& test);
	



};
// --------------------------------------
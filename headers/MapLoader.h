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


using std::ostream;
using std::string;
using std::endl;

class MapLoader
{

public:
	// default constructor
	MapLoader();

	//copy constructor
	MapLoader(const MapLoader &toCopy);

	// OS stream operator
	friend ostream &operator<<(ostream &out, const MapLoader &o);

	//assignment operator
	MapLoader &operator=(const MapLoader &o);

	//destructor
	~MapLoader();

	vector<string> continents;
	vector<string> borders;
	vector<string> countries;

	// map test
public:
	bool loadFile(string filePath, map::Map &test);
	// loadFile method which loads/checks the mapfile for all the contents (reads line by line)
public:
	bool isBorder(string line, map::Map &test, bool &isValid); // isBorder method which checks if the string is a border
																														 // we can first start by checkng array content
																														 // in this case we can just check to see if the sequence of strings follow the pattern
public:
	bool isCountry(string line, map::Map &test, bool &isValid); // isCountry method whih checks if the string is a country
																															// isContinent method whcih checks if the strings is a continent
public:
	bool isContinent(string line, map::Map &test, bool &isValid,  int &continentID);

public:
	bool mapSelection(map::Map &test);
	vector<string> findMapFiles();
	bool isCountryAddToMap( map::Map &test,bool &hasFirstInt,bool &hasString, bool &hasSecondInt, bool &hasThirdInt, bool &hasFourthInt, vector<string> &countriesArr, int &y, int &val0 );
    void isCountryLineValidator(int& val0, int& val2 , bool &hasFirstInt,bool &hasString, bool &hasSecondInt, bool &hasThirdInt, bool &hasFourthInt, vector<string> &countriesArr );
	void isCountryVector(vector<string> &countriesArr, string line, bool &isValid, int &y);
    void isContinentArray(string *continentsArr, string line, int &i);
    void isContinentLineValidator(  bool &hasFirstString, bool& hasInt, bool& hasSecondString, int &val1, string *continentsArr );
    bool isContinentAddToMap( string line, map::Map &test,bool &hasFirstString, bool& hasInt, bool& hasSecondString, int &i, int &val1, string *continentsArr, vector<string> &continents, int &continentID  );
	bool isBorderAddToMap( string line, map::Map &test, vector<string> &bordersArrReborn , bool &foundBorders, vector<string> &borders);
	void isBorderLineValidator(vector<string> &bordersArrReborn , string line, bool &foundBorders);
	void isBorderArray(vector<string> &bordersArrReborn , string line, bool &isValid);

};

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
using namespace std;


class mapLoader {
  public:
  
  vector<string>* continents; 
  vector<string>* borders;    
  vector<string>* countries;       
  string* line;

  // invalid vectors
  vector<string>* continents_I; 
  vector<string>* borders_I;    
  vector<string>* countries_I;  
  // use the test to identify the length of the vector and to see 
  // what positions give you what data type 
  vector<string>* test_I;  

  // check to see if the files is a proper file and contains these 3 headers to read
  // if not we have to copy the whole file by first identifying if
  // if its a string then we can say that the whole line belongs to a territory 
  // so we would store all the contents of that file into a terriotory 
  // if its a number we have to parse it and see if the second entry is a number
  // if its a number it belongs to the border
  // else it belongs to the Countries 

// have to implement these under each 
  bool *foundContinents1; // checks for str
  bool *foundContinents2; // checks for int
  bool *foundContinents3; // checks for str

// example output of countries 1 s1 1 66 28
  bool *foundCountries1; // check for int
  bool *foundCountries2; // check for str
  bool *foundCountries3; // check for int
  bool *foundCountries4; // check for int
  bool *foundCountries5; // check for int

  bool *foundBorders;
  

// ave booleans to represent in each stage whther if a varibale is an int, string 
// this is for each variable that we knwo we must have
// so for the borders section we will have 1 variable and cehck to see if theyre all ints
// for the continents we will see if its 

// then when were giving the info we will parse it the same way with spaces and sssign the
// proer variables to their respective indexes 

// countires wil need 5 variables chec to see if its int, stirng, int, int , int
//[countries]
//1 s1 1 66 28

// continents will need 3 variables, check to see if its a string, int, and string
// [continents]
// Scandinavia 5 #fff780

// borders have to check if theyre all ints 
    string *continentsArr[3];
    string *countriesArr[5];
    string *bordersArr[999];       

// initiliaze thee pointers to 0 and then when they come up in their respective functions
// we will point them to another int whos value is 0 and reset from there
    int *i;
    int *y;
    int *x;
    int *z;

mapLoader(string filePath);
void loadFile(); // loadFile method which loads/checks the mapfile for all the contents (reads line by line)
void isBorder(string line);// isBorder method which checks if the string is a border
// we can first start by checkng array content 
// in this case we can just check to see if the sequence of strings follow the pattern
void isCountry(string line);// isCountry method whih checks if the string is a country 
// isContinent method whcih checks if the strings is a continent 
void isContinent(string line);



};


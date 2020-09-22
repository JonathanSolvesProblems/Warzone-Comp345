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
#include "MapLoader.h"

// ths map loader detects the files nto by headers or correct formting of the file but tby the sequence
// of the strings that make up a line
// for instance a line may be all ints and that will imply it belongs to the border vector

class MapLoader
{

public:
    vector<string> continents;
    vector<string> borders;
    vector<string> countries;
    string line;

    // invalid vectors to be decided depending on the way Nora want the erro handling to be done
    vector<string> continents_I;
    vector<string> borders_I;
    vector<string> countries_I;

    // use the test to identify the length of the vector and to see
    // what positions give you what data type
    vector<string> test_I;

    // booleans to determine if the line is a continent
    bool foundContinents1; // checks for str
    bool foundContinents2; // checks for int
    bool foundContinents3; // checks for str

    // booleans to determine if the line is a countries
    bool foundCountries1; // check for int
    bool foundCountries2; // check for str
    bool foundCountries3; // check for int
    bool foundCountries4; // check for int
    bool foundCountries5; // check for int

    // boolean to determine if the line is a border checks if theyre all ints
    bool foundBorders;

    // arrays to store the contents of the line seperated by a space delimiter
    string continentsArr[3];
    string countriesArr[5];
    string bordersArr[999];

    // variables to determine the number of strings in a line
    int *i;
    int *y;
    int *x;
    int z;

    // constructor that takes the string as a filepath
    MapLoader(string filePath)
    {

        //opens the file
        ifstream mapFile(filePath);
        if (mapFile.is_open())
        {
            //reads every line in the selected file through the variable line
            while (getline(mapFile, line))
            {

                if (line.length() == 0)
                {
                    continue;
                }

                if (line == "[continents]")
                {
                    continue;
                }

                if (line == "[borders]")
                {
                    continue;
                }

                if (line == "[countries]")
                {
                    continue;
                }
                // checks to see if the line is a Border
                isBorder(line);

                // checks to see if the line is a Country
                isCountry(line);

                // checks to see if the line is a Continent
                isContinent(line);
            }
            mapFile.close();
        }

        else
            cout << "Unable to open file... Please check your syntax and restart the program. ";
    };

    // method that stores the contents of the given line should it be a Border (all ints)
    void isBorder(string line)
    {

        int x2 = 0; //a variable to set our pointer to determine the number of strings
        x = &x2;

        // gets the string
        stringstream ssin(line);
        // determines number of strings in the line
        while (ssin.good())
        {
            ssin >> bordersArr[*x];
            ++x2;
        }

        // set the size of the array to check the strs
        string bordersArr2[*x];
        x2 = 0; // sets size variable back to zero

        // stores the contents of the line into another array bordersArr2
        for (int r = 0; r < sizeof(bordersArr2) / sizeof(bordersArr2[0]); r++)
        {
            bordersArr2[r] = bordersArr[r];
        }

        // run a for loop ad basically have 1 variable if it is not a int then have it
        // go to an if statement where the boolean is false and then break the loop
        for (int q = 0; q < sizeof(bordersArr2) / sizeof(bordersArr2[0]); q++)
        {

            try
            {

                int val0 = std::stoi(bordersArr2[q]);
                foundBorders = true;
            }
            catch (std::exception &e)
            {
                foundBorders = false;
                break;
            }
        }

        // if only ints were found then we add it to the map
        if (foundBorders == true)
            borders.push_back(line);
    };

    // isContinent method which checks to see if the string is a continent and adds it to the map
    void isContinent(string line)
    {
        int i2 = 0; //a variable to set our pointer to determine the number of strings
        i = &i2;
        // gets the string
        stringstream ssin(line);
        // parses all the contents of the line
        while (ssin.good() && *i < 3)
        {
            ssin >> continentsArr[*i];
            ++i2;
        }
        i2 = 0;

        // checks to see if this array index is a string
        try
        {

            int val0 = std::stoi(continentsArr[0]);
            foundContinents1 = false;
        }
        catch (std::exception &e)
        {
            foundContinents1 = true;
            // put the bool variable to true
            //cout << "This value is a string (what we want). Not an Int. " << '\n';
        }

        // tests to see if the second entry is a int
        try
        {
            // put the bool to true at the end
            int val1 = std::stoi(continentsArr[1]);
            //cout << val1<< "this is the value sedfghujkmg";
            foundContinents2 = true;
        }
        catch (std::exception &e)
        {
            // put the bool variable to false
            //cout << " Not an Int this is not what we want  " << '\n';
            foundContinents2 = false;
        }

        // checks to see if the array index is a string
        try
        {
            int val2 = std::stoi(continentsArr[2]);
            foundContinents3 = false;
        }
        catch (std::exception &e)
        {
            //  cout << " Not an Int this is not what we want  " << '\n';
            foundContinents3 = true;
        }

        // if it follows the string int string comvination then we store into the map
        if (foundContinents1 == true && foundContinents2 == true && foundContinents3 == true)
        {
            continents.push_back(line); // store into a map
        }
    };

    // isCountry method whcih checks if the string is a Country to be added to the map
    void isCountry(string line)
    {
        int y2 = 0; //a variable to set our pointer to determine the number of strings
        y = &y2;

        // test input  1 s1 1 66 28

        // gets the string
        stringstream ssin(line);

        // parses the string into an array
        while (ssin.good() && *y < 5)
        {
            ssin >> countriesArr[*y];
            ++y2;
        }
        y2 = 0; // resets the counter to 0

        // tests to see if the first entry is a int
        try
        {

            int val0 = std::stoi(countriesArr[0]);
            foundCountries1 = true;
        }
        catch (std::exception &e)
        {
            foundCountries1 = false;
        }

        // tests to see if the second entry is a str
        try
        {

            int val0 = std::stoi(countriesArr[1]);
            foundCountries2 = false;
        }
        catch (std::exception &e)
        {
            foundCountries2 = true;
        }

        // tests to see if the third entry is a int
        try
        {

            int val0 = std::stoi(countriesArr[2]);
            foundCountries3 = true;
        }

        catch (std::exception &e)
        {
            foundCountries3 = false;
        }

        // tests to see if the fourth entry is a int
        try
        {

            int val0 = std::stoi(countriesArr[3]);
            foundCountries4 = true;
        }

        catch (std::exception &e)
        {
            foundCountries4 = false;
        }

        // tests to see if the fifth entry is a int
        try
        {

            int val0 = std::stoi(countriesArr[4]);
            foundCountries5 = true;
        }

        catch (std::exception &e)
        {
            foundCountries5 = false;
        }

        // if all the strings follow the proper sequence of a country add it to the map
        if (foundCountries1 == true && foundCountries2 == true && foundCountries3 == true && foundCountries4 == true && foundCountries5 == true)
            countries.push_back(line); // change this one line for the method to add to map
    };
};

int main()
{

    MapLoader map("hello.txt");

    // remove 2 at the end
    cout << "Contients  " << map.continents.size() << " \n";
    for (int s = 0; s < map.continents.size(); s++)
    {
        cout << map.continents[s] << endl;
    }

    cout << "borders" << map.borders.size() << "\n";
    for (int s = 0; s < map.borders.size(); s++)
    {
        cout << map.borders[s] << endl;
    }

    cout << "countries" << map.countries.size() << "\n";
    for (int s = 0; s < map.countries.size(); s++)
    {
        cout << map.countries[s] << endl;
    }

    return 0;
}
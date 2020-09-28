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

#include "MapLoader.h"
using namespace std;
// ths map loader detects the files nto by headers or correct formting of the file but tby the sequence
// of the strings that make up a line
// for instance a line may be all ints and that will imply it belongs to the border vector



    vector<string> continents;
    vector<string> borders;
    vector<string> countries;
 

    // booleans to determine if the line is a continent
    //bool foundContinents1; // checks for str
    //bool foundContinents2; // checks for int
    //bool foundContinents3; // checks for str

    // booleans to determine if the line is a countries
    //bool foundCountries1; // check for int
    //bool foundCountries2; // check for str
    //bool foundCountries3; // check for int
    //bool foundCountries4; // check for int
    //bool foundCountries5; // check for int

    // boolean to determine if the line is a border checks if theyre all ints
    // bool foundBorders;

    // arrays to store the contents of the line seperated by a space delimiter
    //string continentsArr[3];
    //string countriesArr[5];
    //string bordersArr[999];

   
    // methods that takes the string as a filepath
    void mapLoader::loadFile(string filePath)
    {
        
           bool continentsFound;
           bool countriesFound;
           bool bordersFound;

           string line;

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


    // checks to see if there is the continents banner and checks and stores them all 
                if (line == "[continents]")
                {
                    continentsFound = true;
           

            while ( getline(mapFile,line))
                {

                 // end of the file stop the loop
                if (line.length() == 0)
                {
                   break;
                }

            else{
               // checks to see if the line is a Continent
               isContinent(line);
                 }

                }

                }



    // checks to see if there is the border banner and checks and stores them all 
                if (line == "[borders]")
                {
           
              bordersFound = true;


            while ( getline(mapFile,line))
                {

                 // end of the file stop the loop
                if (line.length() == 0)
                {
                   break;
                }

            else{
               // checks to see if the line is a Continent
               isBorder(line);
                 }

                }

                }




    // checks to see if there is the countries banner and checks and stores them all 
                if (line == "[countries]")
                {

                countriesFound = true;
           

            while ( getline(mapFile,line))
                {

                 // end of the file stop the loop
                if (line.length() == 0)
                {
                   break;
                }

            else{
               // checks to see if the line is a Continent
               isCountry(line);
                 }

                }

                }

               
              
            }

            mapFile.close();
        }

        else
            cout << "Unable to open file... Please check your syntax and restart the program. ";

        if(continentsFound == false || countriesFound == false || bordersFound == false ){
             cout << "Invalid Map File... Program Terminating... Please make sure all header banners [borders], [countries], [continents] are correct";
             exit(3);
        }
         

    };

    // method that stores the contents of the given line should it be a Border (all ints)
    void mapLoader::isBorder(string line)
    {
        // stores the parsed string into array elements
        vector<string> bordersArrReborn;

    string bordersArr[1];
    // boolean to determine if the line is a border checks if theyre all ints
    bool foundBorders;

     int x = 0; //a variable to set our pointer to determine the number of strings
       

        // gets the string
        stringstream ssin(line);
        // determines number of strings in the line

        /*   */
        while (ssin.good())
        {
            ssin<<bordersArr[0];
          bordersArrReborn.push_back(bordersArr[0]);
        }

        /*
        // set the size of the array to check the strs
        string bordersArr2[x];
        x = 0; // sets size variable back to zero

        // stores the contents of the line into another array bordersArr2
        for (int r = 0; r < sizeof(bordersArrReborn) / sizeof(bordersArrReborn); r++)
        {
            bordersArr2[r] = bordersArr[r];
        }

        */
        
        // run a for loop ad basically have 1 variable if it is not a int then have it
        // go to an if statement where the boolean is false and then break the loop
        for (int q = 0; q < sizeof(bordersArrReborn) / sizeof(bordersArrReborn); q++)
        {

            try
            {

                int val0 = std::stoi(bordersArrReborn[q]);
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
        
        else{
            cout << "Invalid Map File... Program Terminating... Please make sure all syntax is correct under the border banner";
             exit(3);
        }

    };

    // isContinent method which checks to see if the string is a continent and adds it to the map
    void mapLoader::isContinent(string line)
    {

  // arrays to store the contents of the line seperated by a space delimiter
    string continentsArr[99];
    // booleans to determine if the line is a continent
    bool foundContinents1; // checks for str
    bool foundContinents2; // checks for int
    bool foundContinents3; // checks for str

        int i= 0; //a variable to set our pointer to determine the number of strings
       
        // gets the string
        stringstream ssin(line);
        // parses all the contents of the line
        while (ssin.good())
        {
            ssin >> continentsArr[i];
            ++i;
        }
   

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

              //  cout << " Not an Int this is not what we want  " << '\n';
            if(continentsArr[1].length() == 0){
             foundContinents2 = false;

            }
            else
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
            if(continentsArr[2].length() == 0){
             foundContinents3 = false;
            }
            else
            foundContinents3 = true;
        }

        // if it follows the string int string comvination then we store into the map
        if (foundContinents1 == true && foundContinents2 == true && foundContinents3 == true && i ==3)
        {
            continents.push_back(line); // store into a map
        }

        else{
            cout << "Invalid Map File... Program Terminating... Please make sure all syntax is correct under the continent banner make sure there are no spaces after the last element";
             exit(3);
        }

     i = 0; // resets counter variable
    };

    // isCountry method whcih checks if the string is a Country to be added to the map
    void mapLoader::isCountry(string line)
    {
// stores the string into the elements in the array 
    string countriesArr[5];

// booleans to determine if the line is a countries
    bool foundCountries1; // check for int
    bool foundCountries2; // check for str
    bool foundCountries3; // check for int
    bool foundCountries4; // check for int
    bool foundCountries5; // check for int

        int y = 0; //a variable to set our pointer to determine the number of strings
       

        // test input  1 s1 1 66 28

        // gets the string
        stringstream ssin(line);

        // parses the string into an array
        while (ssin.good())
        {
            ssin >> countriesArr[y];
            y++;
        }

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
             if(countriesArr[1].length() == 0){
             foundCountries2= false;
            }
            else
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
        if (foundCountries1 == true && foundCountries2 == true && foundCountries3 == true && foundCountries4 == true && foundCountries5 == true && y==5)
            countries.push_back(line); // change this one line for the method to add to map

        
        else{
            cout << "Invalid Map File... Program Terminating... Please make sure all syntax is correct under the country banner make sure there are no spaces after the last element";
             exit(3);
        }
                y= 0; // resets the counter to 0

    };

int main()
{

    mapLoader map;
    map.loadFile("hello.txt");

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
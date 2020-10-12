#include "MapLoader.h"


// THIS IS THE OFFICIAL FILE

using namespace std;
// ths map loader detects the files nto by headers or correct formting of the file but tby the sequence
// of the strings that make up a line
// for instance a line may be all ints and that will imply it belongs to the border vector



vector<string> continents;
vector<string> borders;
vector<string> countries;



// methods that takes the string as a filepath a map, continent, and territory objects 
bool mapLoader::loadFile(string filePath, map::Map &test)
{

    bool continentsFound;
    bool countriesFound;
    bool bordersFound;
   static bool isValid =true;
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


                while (getline(mapFile, line))
                {

                    // end of the file stop the loop
                    if (line.length() == 0)
                    {
                        break;
                    }

                    else {
                        // checks to see if the line is a Continent

                        if (isContinent(line, test, isValid)==false  ) {
                            isValid = false;
                        }

                    }

                }

            }

            /**/

// checks to see if there is the border banner and checks and stores them all 
            if (line == "[borders]")
            {

                bordersFound = true;


                while (getline(mapFile, line))
                {

                    // end of the file stop the loop
                    if (line.length() == 0)
                    {
                        break;
                    }

                    else {
                        // checks to see if the line is a Continent

                        if (isBorder(line, test,isValid) == false) {
                            isValid = false;
                        }

                    }

                }

            }




            // checks to see if there is the countries banner and checks and stores them all 
            if (line == "[countries]")
            {

                countriesFound = true;


                while (getline(mapFile, line))
                {

                    // end of the file stop the loop
                    if (line.length() == 0)
                    {
                        break;
                    }

                    else {
                        // checks to see if the line is a Continent
                        if (isCountry(line, test,isValid) == false) {
                            isValid = false;
                        }

                    }

                }

            }



        }

        mapFile.close();
    }

    else {
        return false;
    }
    

    if (continentsFound == false || countriesFound == false || bordersFound == false || isValid == false) {
        return false;
    }

    else
        return true;


};

// method that stores the contents of the given line should it be a Border (all ints)
bool mapLoader::isBorder(string line, map::Map& test, bool &isValid)
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
    while (ssin.good() && x < 11)
    {
        ssin >> bordersArr[0];
        bordersArrReborn.push_back(bordersArr[0]);

    }

    x = 0;
   

    // run a for loop ad basically have 1 variable if it is not a int then have it
    // go to an if statement where the boolean is false and then break the loop
    for (int q = 0; q < bordersArrReborn.size(); q++)
    {

        try
        {

            int val0 = std::stoi(bordersArrReborn[q]);
            foundBorders = true;
        }
        catch (std::exception& e)
        {
            foundBorders = false;
            break;
        }
    }

    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false 
    if (isValid == false) {
        foundBorders = true;
        return false;
    }


    // if only ints were found then we add it to the map
    if (foundBorders == true) {

        borders.push_back(line);



        for (int q = 0;q < bordersArrReborn.size()-1; q++)
        {
            int land1 = std::stoi(bordersArrReborn[0]);
            int land2 = std::stoi(bordersArrReborn[q+1]);


            test.connectTerritories(land1, land2);
          // have to add neighbour
            test.getTerritory(land1)->addNeighbour(test.getTerritory(land2));
        }

        return true;
    }
       

    else {
        return false;
    }

};

// isContinent method which checks to see if the string is a continent and adds it to the map by creating a continent obj 
bool mapLoader::isContinent(string line, map::Map &test, bool &isValid)
{
    // a continent ID which follows standard syntax OF ALL FILES, which is to start at 1, and goes up to N continents
    static int continentID = 1;
    // arrays to store the contents of the line seperated by a space delimiter
    string continentsArr[99];
    // booleans to determine if the line is a continent
    bool foundContinents1; // checks for str
    bool foundContinents2; // checks for int
    bool foundContinents3; // checks for str

    int i = 0; //a variable to set our pointer to determine the number of strings
    int val1 = 0;
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
    catch (std::exception& e)
    {
        foundContinents1 = true;
        // put the bool variable to true
        //cout << "This value is a string (what we want). Not an Int. " << '\n';
    }

    // tests to see if the second entry is a int
    try
    {
        // put the bool to true at the end
        val1 = std::stoi(continentsArr[1]);
        //cout << val1<< "this is the value sedfghujkmg";
        foundContinents2 = true;
    }
    catch (std::exception& e)
    {
        // put the bool variable to false
        //cout << " Not an Int this is not what we want  " << '\n';

          //  cout << " Not an Int this is not what we want  " << '\n';
    
            foundContinents2 = false;
    }

    // checks to see if the array index is a string
    try
    {
        int val2 = std::stoi(continentsArr[2]);
        foundContinents3 = false;
    }
    catch (std::exception& e)
    {
    
        
            foundContinents3 = true;
    }


    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false 
    if (isValid == false) {
        i= 0;
        foundContinents3 = false;
        return false;
    }

    // if it follows the string int string comvination then we store into the map
    if (foundContinents1 == true && foundContinents2 == true && foundContinents3 == true && i == 3)
    {
        i = 0; // resets counter variable
        continents.push_back(line); // store into a map
        
      
// creates a continent object to be added to the list of continents        
        map::Continent *continent = new map::Continent(continentID, continentsArr[0], val1);
        continentID++;
        test.addContinent(continent);

       // deallocate the pointers 
       continent = NULL;
       delete continent;
      return true;
    }

    else {
        i = 0; // resets counter variable
       return false;
    }

 
};

// isCountry method whcih checks if the string is a Country to be added to the map by using the territory class
bool mapLoader::isCountry(string line ,map::Map& test, bool &isValid)
{
   
    // stores the string into the elements in the array 
   string countriesArrStore[1];
    vector<string> countriesArr;
    int val0 = 0;
    int val2 = 0;
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
        ssin >> countriesArrStore[0];
        string l = countriesArrStore[0];
      
        if (l.length() ==0) {
         
            continue;
      }
        else {
            countriesArr.push_back(countriesArrStore[0]);
            y++;
        }
        
           
    }

  

    // tests to see if the first entry is a int
    try
    {

        val0 = std::stoi(countriesArr[0]);
        foundCountries1 = true;
    }
    catch (std::exception& e)
    {
        foundCountries1 = false;
    }

    // tests to see if the second entry is a str
    try
    {

        int val1 = std::stoi(countriesArr[1]);
        foundCountries2 = false;
    }
    catch (std::exception& e)
    {
        
            foundCountries2 = true;
    }

    // tests to see if the third entry is a int
    try
    {

        int val2 = std::stoi(countriesArr[2]);
        foundCountries3 = true;
    }

    catch (std::exception& e)
    {
        foundCountries3 = false;
    }

    // tests to see if the fourth entry is a int
    try
    {

        int val3 = std::stoi(countriesArr[3]);
        foundCountries4 = true;
    }

    catch (std::exception& e)
    {
        foundCountries4 = false;
    }

    // tests to see if the fifth entry is a int
    try
    {

        int val4 = std::stoi(countriesArr[4]);
        foundCountries5 = true;
    }

    catch (std::exception& e)
    {
      
        foundCountries5 = false;
    }

    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false 
    if (isValid == false) {
        y = 0;
        foundCountries1 = false;
        return false;
    }


    // if all the strings follow the proper sequence of a country add it to the map
    if (foundCountries1 == true && foundCountries2 == true && foundCountries3 == true && foundCountries4 == true && foundCountries5 == true && y == 5)
    {
        countries.push_back(line); // change this one line for the method to add to map
       

        // do the third array string for the Continent ID

       // stores the territory into the proper class  

  
       
       //cout << *test.getContinent(std::stoi(countriesArr[2]));
        

       map::Territory* territory = new map::Territory(val0, countriesArr[1], *test.getContinent(std::stoi(countriesArr[2])));
        test.addTerritory(territory);
        

        // deallocate the pointers 
        territory = NULL;
        delete territory;

        y = 0;
        return true;

    }


    else {
        y = 0; // resets the counter to 0
        // invalid map file
 
        return false;
    }

};

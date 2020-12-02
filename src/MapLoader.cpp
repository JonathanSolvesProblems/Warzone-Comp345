#include "MapLoader.h"

/* ******************************* THIS IS THE OFFICIAL MAPLOADER FILE ************************************* */
// ths map loader detects the files by headers (with correct formating or not) and reads the the file whether if it is valid or not.
// the methods all return boolean values where if they return true, its because the file follows correct syntax and format.

#ifdef __linux__
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

#include <locale>
#include <codecvt>

using std::ifstream;
using std::stringstream;

std::wstring string_to_convert;

// default constructor
MapLoader::MapLoader()
{
}

//copy constructor
MapLoader::MapLoader(const MapLoader& copyto)
{
    this->continents = copyto.continents;
    this->borders = copyto.borders;
    this->countries = copyto.countries;
}

// destructor
MapLoader::~MapLoader()
{
}

// OS stream operator
ostream& operator<<(ostream& out, const MapLoader& o)
{

    for (int s = 0; s < o.continents.size(); s++)
    {
        out << o.continents[s] << endl;
    }
    return out;
}


// Conquest File Reader Class
bool ConquestFileReader::loadFile(string mapName, map::Map& test)
{
    int continentID = 1;
    int territoryID = 1;
    bool continentsFound = false;
    bool countriesFound = false;
    bool isValid = true;
    string line;

    //THIS IS THE PATH TO THE FOLDER THAT WE WANT
    string folderPath;
#ifdef __linux__
    folderPath = "exampleMaps/" + mapName;
#else
    folderPath = "exampleMaps\\" + mapName;
#endif

    //opens the file
    ifstream mapFile(folderPath);

    string continentsHeader = "[Continents]";
    string countriesHeader = "[Territories]";

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
            if (strstr(line.c_str(), continentsHeader.c_str()))
            {
                continentsFound = true;

                while (getline(mapFile, line))
                {
                  

                    // end of the file stop the loop
                    if (line.length() == 0)
                    {
                        break;
                    }

                    else
                    {
                        // checks to see if the line is a Continent
                        if (isContinent(line, test, isValid, continentID) == false)
                        {
                            isValid = false;
                        }
                    }
                }
            }

            // // checks to see if there is the countries banner and checks and stores them all
            if (strstr(line.c_str(), countriesHeader.c_str()))
             {
                

               countriesFound = true;
               while (getline(mapFile, line))
                {
                     // end of the file stop the loop
                if (line.length() == 0)
                    {
                        break;
                   }
                       else
                   {
                         // checks to see if the line is a Continent
                         if (isCountry(line, test, isValid, territoryID) == false)
                         {
                            isValid = false;
                        }
                     }
               }
             }
        }
        mapFile.close();
    }
    else
    {
        return false;
    }
    
    if (continentsFound == false || countriesFound == false || isValid == false)
    {
        continents.clear();
        countries.clear();
        return false;
    }

    else
    {

        return true;
    }
}


// territories part Territory,int1,int2,Continent,neighbours.....
// delimiter is the , 
// use a vector, test the first case, is a string, second is int,
// third is int, fourth continent, the fifth and the rest are string 
//and ar ethe neighbours

/* COUNTIRES & BORDERS FOR CONQUEST FILE READER **************************************************  */

// stores all the components of the line for the isCountry method into the countriesArr vector
void ConquestFileReader::isCountryVector(vector<string>& countriesArr, string line, bool& isValid)
{

    // gets the string
    stringstream ssin(line);
    char delim = ',';
    string countriesArrStore[1];

    try
    {
        while (getline(ssin, line, delim))
        {
            // accepts spaces at the end of a line
            if (line.length() == 0)
            {
                continue;
            }

            //stores the string into the countriesArr
            countriesArrStore[0] = line;
            
            countriesArr.push_back(countriesArrStore[0]);

            // counts to make sure that the correct number of strings (5) is on a given line
          
        }
    }

    // if there happens to be an error, the file is no longer valid
    catch (std::exception& e)
    {
        isValid = false;
    }
}



// use a vector, test the first case, is a string, second is int,
// third is int, fourth continent, the fifth and the rest are string 
//and ar ethe neighbours
void ConquestFileReader::isCountryLineValidator(int& val0, int& val2, bool& hasFirstInt, bool& hasString, bool& hasSecondInt, bool& hasThirdInt, bool& hasFourthInt, vector<string>& countriesArr)
{

    // tests to see if the first entry is a str
    try
    {
        int val1 = std::stoi(countriesArr[0]);
        hasString = false;
    }
    catch (std::exception& e)
    {
        hasString = true;
    }

    // tests to see if the second entry is a int
    try
    {
        val0 = std::stoi(countriesArr[1]);
        hasFirstInt = true;
    }
    catch (std::exception& e)
    {
        hasFirstInt = false;
    }

    // tests to see if the third entry is a int
    try
    {
        int val2 = std::stoi(countriesArr[2]);
        hasSecondInt = true;
    }

    catch (std::exception& e)
    {
        hasSecondInt = false;
    }

    // tests to see if the fourth entry is a str // and is a continent 
    try
    {
        int val3 = std::stoi(countriesArr[3]);
        hasString = false;
    }
    catch (std::exception& e)
    {
        hasString = true;
    }

    // MAY AHVE TO COME BACK TO HERE 
    for (int i = 4; i < countriesArr.size(); i++) {
    
        // need to do a for loop to test if its a string of borders. 
        // tests to see if the fifth entry is a int
        try
        {
            int val4 = std::stoi(countriesArr[i]);
            hasString = false;
        }

        catch (std::exception& e)
        {

            hasString = true;
        }
    }

  
}


// territories part Territory,int1,int2,Continent,neighbours.....
// delimiter is the ,
// use a vector, test the first case, is a string, second is int,
// third is int, fourth continent, the fifth and the rest are string 
//and ar ethe neighbours
bool ConquestFileReader::isCountryAddToMap(map::Map& test, bool& hasFirstInt, bool& hasString, bool& hasSecondInt, vector<string>& countriesArr, int& val0, int& val2, int& territoryID)
{
    // if all the strings follow the proper sequence of a country add it to the map
    if (hasFirstInt == true && hasString == true && hasSecondInt == true)
    {
        // store all territories in the countries array 
        countries.push_back(countriesArr[0]);

        // if the territory already exsits 
        if (test.getTerritory(countriesArr[0])!= NULL) {
           
            // then set the continent to the correct one through the string 
            test.getTerritory(countriesArr[0])->setContinent(test.getContinent(countriesArr[3]));

            // create the territories of the neighbouring territories
            for (int q = 4; q < countriesArr.size(); q++)
            {
                // if the territory already exsits 
                if (test.getTerritory(countriesArr[q]) != NULL) {
                    // adds the territories as neighbours
                    test.getTerritory(countriesArr[0])->addNeighbour(test.getTerritory(countriesArr[q]));
                    // connects the two territories
                    test.connectTerritories(test.getTerritory(countriesArr[0]), test.getTerritory(countriesArr[q]));
                    continue;
                }

                // stores the territory into the proper class
                map::Territory* territory = new map::Territory(territoryID, countriesArr[q], *test.getContinent((countriesArr[3])));
                test.addTerritory(territory);
                // increment the terriotry ID
                territoryID++;

                // adds the territories as neighbours
                test.getTerritory(countriesArr[0])->addNeighbour(test.getTerritory(countriesArr[q]));
                // connects the two territories
                test.connectTerritories(test.getTerritory(countriesArr[0]), test.getTerritory(countriesArr[q]));
                // deallocate the pointers
                territory = NULL;
                delete territory;
            }
        }

        // the territory doesnt exist 
        else {
        
            // stores the territory into the proper class
            map::Territory* territory = new map::Territory(territoryID, countriesArr[0], *test.getContinent((countriesArr[3])));
            test.addTerritory(territory);

            // increment the territory ID
            territoryID++;

            // create the territories of the neighbouring territories
            for (int q = 4; q < countriesArr.size(); q++)
            {

                // if the territory already exsits 
                if (test.getTerritory(countriesArr[q]) != NULL) {
                    // adds the territories as neighbours
                    test.getTerritory(countriesArr[0])->addNeighbour(test.getTerritory(countriesArr[q]));
                    // connects the two territories
                    test.connectTerritories(test.getTerritory(countriesArr[0]), test.getTerritory(countriesArr[q]));
                    continue;
                }


                // stores the territory into the proper class
                map::Territory* territory = new map::Territory(territoryID, countriesArr[q], *test.getContinent((countriesArr[3])));
                test.addTerritory(territory);
                // increment the terriotry ID
                territoryID++;

                // adds the territories as neighbours
                test.getTerritory(countriesArr[0])->addNeighbour(test.getTerritory(countriesArr[q]));
                // connects the two territories
                test.connectTerritories(test.getTerritory(countriesArr[0]), test.getTerritory(countriesArr[q]));

                // deallocate the pointers
                territory = NULL;
                delete territory;

            }
        }
        // is a vaid map file
        return true;
    }

    else
    {
        // invalid map file
        return false;
    }
}

// isCountry method whcih checks if the string is a Country to be added to the map by using the territory class
bool ConquestFileReader::isCountry(string line, map::Map& test, bool& isValid, int& territoryID)
{
    // stores the string into the elements in the array
    string countriesArrStore[1];
    vector<string> countriesArr; // contains all the eleemnts of the line
    int val0 = 0;
    int val2 = 0;
    // booleans to determine if the line is a countries
    bool hasFirstInt;  // check for int
    bool hasString;    // check for str
    bool hasSecondInt; // check for int
    bool hasThirdInt;  // check for int
    bool hasFourthInt; // check for int

    // stores all the components of a line into the vector
    isCountryVector(countriesArr, line, isValid);

    // checks to see if the line follows correct syntax
    isCountryLineValidator(val0, val2, hasFirstInt, hasString, hasSecondInt, hasThirdInt, hasFourthInt, countriesArr);

    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false
    if (isValid == false)
    {
        hasFirstInt = false;
        return false;
    }

    // returns true if the line was proper and was able to be added to the map
    return isCountryAddToMap(test, hasFirstInt, hasString, hasSecondInt, countriesArr, val0, val2, territoryID);
};


/* COUNTIRES & BORDERS FILE READER **************************************************  */



/* CONTINENTS FOR CONQUEST FILE READER **************************************************  */

// continents part -> contienet=#bonus delimiter is the = 
// store in array with an index of 2, first element is the name
// the second is the bonus 
// the string should have a max size of 2

// stores all the components of the line for the isCountry method into the countriesArr vector
void ConquestFileReader::isContinentArray(string* continentsArr, string line, int& i)
{

    // gets the string
    stringstream ssin(line);
    char delim = '=';

    while (getline(ssin, line, delim))
    {
        // accepts spaces at the end of file
        if (line.length() == 0)
        {
            continue;
        }
        continentsArr[i] = line;
        i++;
    }
    // parses all the contents of the line
    while (ssin.good())
    {
        // may have to add another variable to read it 
        ssin >> continentsArr[i];
        ++i;
    }
}

//validates the line used for the continent part
void ConquestFileReader::isContinentLineValidator(bool& hasFirstString, bool& hasInt, int& val1, string* continentsArr)
{
    // checks to see if this array index is a string
    try
    {
        int val0 = std::stoi(continentsArr[0]);
        hasFirstString = false;
    }
    catch (std::exception& e)
    {
        // put the bool variable to true
        hasFirstString = true;
    }

    // tests to see if the second entry is a int
    try
    {
        // put the bool to true at the end
        val1 = std::stoi(continentsArr[1]);
        hasInt = true;
    }

    catch (std::exception& e)
    {
        hasInt = false;
    }
}

// adds the validated line to the map
bool ConquestFileReader::isContinentAddToMap(string line, map::Map& test, bool& hasFirstString, bool& hasInt, int& i, int& val1, string* continentsArr, vector<string>& continents, int& continentID)
{
    // if it follows the string int string comvination then we store into the map
    if (hasFirstString == true && hasInt == true && i == 2)
    {
        i = 0;                      // resets counter variable
        continents.push_back(line); // store into a map

        // creates a continent object to be added to the list of continents
        map::Continent* continent = new map::Continent(continentID, continentsArr[0], val1);
        continentID++;
        test.addContinent(continent);

        // deallocate the pointers
        continent = NULL;
        delete continent;

        // is a valid file
        return true;
    }

    else
    {
        // resets counter variable
        i = 0;
        // is an invalid file
        return false;
    }
}

// isContinent method which checks to see if the string is a continent and adds it to the map by creating a continent obj
bool ConquestFileReader::isContinent(string line, map::Map& test, bool& isValid, int& continentID)
{
    // a continent ID which follows standard syntax OF ALL FILES, which is to start at 1, and goes up to N continents

    // arrays to store the contents of the line seperated by a space delimiter **** SHOULD ONLY BE 2 
    string continentsArr[2];
    // booleans to determine if the line is a continent
    bool hasFirstString;  // checks for str
    bool hasInt;          // checks for int


    int i = 0; //a variable to set our pointer to determine the number of strings
    int val1 = 0;

    // stores the content of the continentsArr
    isContinentArray(continentsArr, line, i);

    // validate sthe continent line
    isContinentLineValidator(hasFirstString, hasInt, val1, continentsArr);

    if (isValid == false)
    {
        i = 0;

        return false;
    }

    // returns true if we can add the Continent to the map
    return isContinentAddToMap(line, test, hasFirstString, hasInt, i, val1, continentsArr, continents, continentID);
};

/*

*****************************************************************************************/

/*CONQUEST FILE READER


*/
// methods that takes the string as a filepath a map, continent, and territory objects
bool ConquestFileReaderAdapter::loadFile(string mapName, map::Map& test)
{
    // testing 
    map::Map* test2 = new map::Map();
    ConquestFileReader* conquestFileReader = new ConquestFileReader();

   bool ret = conquestFileReader->loadFile(mapName, test);
   return ret;

};






/*CONQUEST FILE READER END ....


*/

// go to the bottom of this file, then where the part that says its false, return the result of the the conquest map object and pass the same line, and test map.

// methods that takes the string as a filepath a map, continent, and territory objects
 bool MapLoader::loadFile(string mapName, map::Map& test)
{
    int continentID = 1;
    bool continentsFound = false;
    bool countriesFound = false;
    bool bordersFound = false;
    bool isValid = true;
    string line;

    //THIS IS THE PATH TO THE FOLDER THAT WE WANT
    string folderPath;
#ifdef __linux__
    folderPath = "exampleMaps/" + mapName;
#else
    folderPath = "exampleMaps\\" + mapName;
#endif

    //opens the file
    ifstream mapFile(folderPath);

    string continentsHeader = "[continents]";
    string bordersHeader = "[borders]";
    string countriesHeader = "[countries]";

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
            if (strstr(line.c_str(), continentsHeader.c_str()))
            {
                continentsFound = true;

                while (getline(mapFile, line))
                {
                    // end of the file stop the loop
                    if (line.length() == 0)
                    {
                        break;
                    }

                    else
                    {
                        // checks to see if the line is a Continent
                        if (isContinent(line, test, isValid, continentID) == false)
                        {
                            isValid = false;
                        }
                    }
                }
            }

            // checks to see if there is the border banner and checks and stores them all
            if (strstr(line.c_str(), bordersHeader.c_str()))
            {
                bordersFound = true;
                while (getline(mapFile, line))
                {
                    // end of the file stop the loop
                    if (line.length() == 0)
                    {
                        break;
                    }

                    else
                    {
                        // checks to see if the line is a Continent

                        if (isBorder(line, test, isValid) == false)
                        {
                            isValid = false;
                        }
                    }
                }
            }

            // checks to see if there is the countries banner and checks and stores them all
            if (strstr(line.c_str(), countriesHeader.c_str()))
            {
                countriesFound = true;
                while (getline(mapFile, line))
                {
                    // end of the file stop the loop
                    if (line.length() == 0)
                    {
                        break;
                    }

                    else
                    {
                        // checks to see if the line is a Continent
                        if (isCountry(line, test, isValid) == false)
                        {
                            isValid = false;
                        }
                    }
                }
            }
        }
        mapFile.close();
    }
    else
    {
        return false;
    }
    if (continentsFound == false || countriesFound == false || bordersFound == false || isValid == false)
    {

            ConquestFileReaderAdapter conquestFileReader;  
           bool adap =  conquestFileReader.loadFile(mapName, test);

           if (adap == true) {
               return true;
           }

           else {
               continents.clear();
               countries.clear();
               borders.clear();
               return false;
           }

        
    }
    else
    {

        return true;
    }
};

void MapLoader::isBorderArray(vector<string>& bordersArrReborn, string line, bool& isValid)
{
    // gets the string
    stringstream ssin(line);
    char delim = ' ';
    string bordersArr[1];
    try
    {
        while (getline(ssin, line, delim))
        {
            // stores the variable into the borderArr
            // end of the file stop the loop

            bordersArr[0] = line;
            if (bordersArr[0].length() == 0)
            {
                continue;
            }
            bordersArrReborn.push_back(bordersArr[0]);
        }
    }

    // is an invalid file
    catch (std::exception& e)
    {
        isValid = false;
    }
}

void MapLoader::isBorderLineValidator(vector<string>& bordersArrReborn, string line, bool& foundBorders)
{
    // run a for loop ad basically have 1 variable if it is not a int then have it
    // go to an if statement where the boolean is false and then break the loop
    // because in the borders section you can only have ints on each line
    for (int q = 0; q < bordersArrReborn.size(); q++)
    {
        try
        {
            // end of the file stop the loop
            if (bordersArrReborn[q].length() == 0)
            {
                break;
            }

            int val0 = std::stoi(bordersArrReborn[q]);
            foundBorders = true;
        }
        catch (std::exception& e)
        {
            foundBorders = false;
            break;
        }
    }
}

bool MapLoader::isBorderAddToMap(string line, map::Map& test, vector<string>& bordersArrReborn, bool& foundBorders, vector<string>& borders)
{
    //look hhere so the size of each aray correclty corresponds to the amount fo borders, soemthign may be flawed with territories,

    // if only ints were found then we add it to the map
    if (foundBorders == true)
    {
        borders.push_back(line);
        for (int q = 0; q < bordersArrReborn.size() - 1; q++)
        {
            // stores the territories into the variables
            int land1 = std::stoi(bordersArrReborn[0]);
            int land2 = std::stoi(bordersArrReborn[q + 1]);

            // adds the territories as neighbours
            test.getTerritory(land1)->addNeighbour(test.getTerritory(land2));
            // connects the two territories
            test.connectTerritories(land1, land2);
        }
        // is a valid file
        return true;
    }

    else
    {
        // is an invalid file
        return false;
    }
}

// method that stores the contents of the given line should it be a Border (all ints)
bool MapLoader::isBorder(string line, map::Map& test, bool& isValid)
{
    // stores the parsed string into array elements
    vector<string> bordersArrReborn;

    string bordersArr[1];
    // boolean to determine if the line is a border checks if theyre all ints
    bool foundBorders;

    int x = 0; //a variable to set our pointer to determine the number of strings

    isBorderArray(bordersArrReborn, line, isValid);

    isBorderLineValidator(bordersArrReborn, line, foundBorders);

    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false
    if (isValid == false)
    {
        foundBorders = true;
        return false;
    }

    return isBorderAddToMap(line, test, bordersArrReborn, foundBorders, borders);
};

// stores all the components of the line for the isCountry method into the countriesArr vector
void MapLoader::isContinentArray(string* continentsArr, string line, int& i)
{

    // gets the string
    stringstream ssin(line);
    char delim = ' ';

    while (getline(ssin, line, delim))
    {
        // accepts spaces at the end of file
        if (line.length() == 0)
        {
            continue;
        }
        continentsArr[i] = line;
        i++;
    }
    // parses all the contents of the line
    while (ssin.good())
    {
        ssin >> continentsArr[i];
        ++i;
    }
}

//validates the line used for the continent part
void MapLoader::isContinentLineValidator(bool& hasFirstString, bool& hasInt, bool& hasSecondString, int& val1, string* continentsArr)
{
    // checks to see if this array index is a string
    try
    {
        int val0 = std::stoi(continentsArr[0]);
        hasFirstString = false;
    }
    catch (std::exception& e)
    {
        // put the bool variable to true
        hasFirstString = true;
    }

    // tests to see if the second entry is a int
    try
    {
        // put the bool to true at the end
        val1 = std::stoi(continentsArr[1]);
        hasInt = true;
    }

    catch (std::exception& e)
    {
        hasInt = false;
    }

    // checks to see if the array index is a string
    try
    {
        int val2 = std::stoi(continentsArr[2]);
        hasSecondString = false;
    }
    catch (std::exception& e)
    {
        hasSecondString = true;
    }
}

// adds the validated line to the map
bool MapLoader::isContinentAddToMap(string line, map::Map& test, bool& hasFirstString, bool& hasInt, bool& hasSecondString, int& i, int& val1, string* continentsArr, vector<string>& continents, int& continentID)
{
    // if it follows the string int string comvination then we store into the map
    if (hasFirstString == true && hasInt == true && hasSecondString == true && i == 3)
    {
        i = 0;                      // resets counter variable
        continents.push_back(line); // store into a map

        // creates a continent object to be added to the list of continents
        map::Continent* continent = new map::Continent(continentID, continentsArr[0], val1);
        continentID++;
        test.addContinent(continent);

        // deallocate the pointers
        continent = NULL;
        delete continent;

        // is a valid file
        return true;
    }

    else
    {
        // resets counter variable
        i = 0;
        // is an invalid file
        return false;
    }
}

// isContinent method which checks to see if the string is a continent and adds it to the map by creating a continent obj
bool MapLoader::isContinent(string line, map::Map& test, bool& isValid, int& continentID)
{
    // a continent ID which follows standard syntax OF ALL FILES, which is to start at 1, and goes up to N continents

    // arrays to store the contents of the line seperated by a space delimiter
    string continentsArr[99];
    // booleans to determine if the line is a continent
    bool hasFirstString;  // checks for str
    bool hasInt;          // checks for int
    bool hasSecondString; // checks for str

    int i = 0; //a variable to set our pointer to determine the number of strings
    int val1 = 0;

    // stores the content of the continentsArr
    isContinentArray(continentsArr, line, i);

    // validate sthe continent line
    isContinentLineValidator(hasFirstString, hasInt, hasSecondString, val1, continentsArr);

    if (isValid == false)
    {
        i = 0;
        hasSecondString = false;
        return false;
    }

    // returns true if we can add the Continent to the map
    return isContinentAddToMap(line, test, hasFirstString, hasInt, hasSecondString, i, val1, continentsArr, continents, continentID);
};

// stores all the components of the line for the isCountry method into the countriesArr vector
void MapLoader::isCountryVector(vector<string>& countriesArr, string line, bool& isValid, int& y)
{

    // gets the string
    stringstream ssin(line);
    char delim = ' ';
    string countriesArrStore[1];

    try
    {
        while (getline(ssin, line, delim))
        {
            // accepts spaces at the end of a line
            if (line.length() == 0)
            {
                continue;
            }

            //stores the string into the countriesArr
            countriesArrStore[0] = line;
            countriesArr.push_back(countriesArrStore[0]);

            // counts to make sure that the correct number of strings (5) is on a given line
            y++;
        }
    }

    // if there happens to be an error, the file is no longer valid
    catch (std::exception& e)
    {
        isValid = false;
    }
}

void MapLoader::isCountryLineValidator(int& val0, int& val2, bool& hasFirstInt, bool& hasString, bool& hasSecondInt, bool& hasThirdInt, bool& hasFourthInt, vector<string>& countriesArr)
{

    // tests to see if the first entry is a int
    try
    {
        val0 = std::stoi(countriesArr[0]);
        hasFirstInt = true;
    }
    catch (std::exception& e)
    {
        hasFirstInt = false;
    }

    // tests to see if the second entry is a str
    try
    {
        int val1 = std::stoi(countriesArr[1]);
        hasString = false;
    }
    catch (std::exception& e)
    {
        hasString = true;
    }

    // tests to see if the third entry is a int
    try
    {
        int val2 = std::stoi(countriesArr[2]);
        hasSecondInt = true;
    }

    catch (std::exception& e)
    {
        hasSecondInt = false;
    }

    // tests to see if the fourth entry is a int
    try
    {
        int val3 = std::stoi(countriesArr[3]);
        hasThirdInt = true;
    }

    catch (std::exception& e)
    {
        hasThirdInt = false;
    }

    // tests to see if the fifth entry is a int
    try
    {
        int val4 = std::stoi(countriesArr[4]);
        hasFourthInt = true;
    }

    catch (std::exception& e)
    {

        hasFourthInt = false;
    }
}

bool MapLoader::isCountryAddToMap(map::Map& test, bool& hasFirstInt, bool& hasString, bool& hasSecondInt, bool& hasThirdInt, bool& hasFourthInt, vector<string>& countriesArr, int& y, int& val0)
{

    // if all the strings follow the proper sequence of a country add it to the map
    if (hasFirstInt == true && hasString == true && hasSecondInt == true && hasThirdInt == true && hasFourthInt == true && y == 5)
    {
        // stores the territory into the proper class
        map::Territory* territory = new map::Territory(val0, countriesArr[1], *test.getContinent(std::stoi(countriesArr[2])));
        test.addTerritory(territory);

        // deallocate the pointers
        territory = NULL;
        delete territory;

        // sets the string counter to 0
        y = 0;
        // is a vaid map file
        return true;
    }

    else
    {
        // resets the counter to 0
        y = 0;
        // invalid map file
        return false;
    }
}

// isCountry method whcih checks if the string is a Country to be added to the map by using the territory class
bool MapLoader::isCountry(string line, map::Map& test, bool& isValid)
{
    // stores the string into the elements in the array
    string countriesArrStore[1];
    vector<string> countriesArr;
    int val0 = 0;
    int val2 = 0;
    // booleans to determine if the line is a countries
    bool hasFirstInt;  // check for int
    bool hasString;    // check for str
    bool hasSecondInt; // check for int
    bool hasThirdInt;  // check for int
    bool hasFourthInt; // check for int

    int y = 0; //a variable to set our pointer to determine the number of strings

    // stores all the components of a line into the vector
    isCountryVector(countriesArr, line, isValid, y);

    // checks to see if the line follows correct syntax
    isCountryLineValidator(val0, val2, hasFirstInt, hasString, hasSecondInt, hasThirdInt, hasFourthInt, countriesArr);

    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false
    if (isValid == false)
    {
        y = 0;
        hasFirstInt = false;
        return false;
    }

    // returns true if the line was proper and was able to be added to the map
    return isCountryAddToMap(test, hasFirstInt, hasString, hasSecondInt, hasThirdInt, hasFourthInt, countriesArr, y, val0);
};

// finds the list of mapfiles in the selected folder and returns a vector of strings of the names of the map files
vector<string> MapLoader::findMapFiles()
{
    std::vector<std::string> listOfFiles;
    // change char set to multibyte by going into the properties and go into advanced and change the char set
    // have to change the filepath to the correct one

    for (auto& p : fs::directory_iterator("exampleMaps"))
    {
        listOfFiles.push_back(p.path().filename().string());
    }
    return listOfFiles;
}

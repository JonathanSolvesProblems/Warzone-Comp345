#include "MapLoader.h"

// allows you to have multiple maps files

/*NOTE BEFORE USING THIS FILE CHANGE YOUR CHARSET TO MULTYBITE. IF YOU ARE USING VISUAL STUDIO ENTERPRISE, DO THIS BY GOING INTO PROJECT->PROPERTIES->ADVANCED
THEN CHANGE YOUR CHARSET FROM UNI TO MULTYBYTE
*/

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

using std::stringstream;
using std::ifstream;

std::wstring string_to_convert;

// default constructor
MapLoader::MapLoader()
{
}

//copy constructor
MapLoader::MapLoader(const MapLoader &copyto)
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
ostream &operator<<(ostream &out, const MapLoader &o)
{

    for (int s = 0; s < o.continents.size(); s++)
    {
        out << o.continents[s] << endl;
    }
    return out;
}
// took from continent clas to make cglobal

//assignment operator
//MapLoader& MapLoader::operator=(const MapLoader& o){}

// change to take in th eselected fil an dto have the filepath already in here
//THIS IS THE PATH TO THE FOLDER THAT WE WANT

// methods that takes the string as a filepath a map, continent, and territory objects
bool MapLoader::loadFile(string mapName, map::Map &test)
{


    int continentID = 1;
    bool continentsFound;
    bool countriesFound;
    bool bordersFound;
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
                        if (isContinent(line, test, isValid,continentID) == false)
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
        continents.clear();
        countries.clear();
        borders.clear();
        
        return false;
    }
    else
    {

        return true;
    }
};

// method that stores the contents of the given line should it be a Border (all ints)
bool MapLoader::isBorder(string line, map::Map &test, bool &isValid)
{
    // stores the parsed string into array elements
    vector<string> bordersArrReborn;

    string bordersArr[1];
    // boolean to determine if the line is a border checks if theyre all ints
    bool foundBorders;

    int x = 0; //a variable to set our pointer to determine the number of strings

    // gets the string
    stringstream ssin(line);
    char delim = ' ';

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
    catch (std::exception &e)
    {
        isValid = false;
    }

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
        catch (std::exception &e)
        {
            foundBorders = false;
            break;
        }
    }

    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false
    if (isValid == false)
    {
        foundBorders = true;
        return false;
    }

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
};

// isContinent method which checks to see if the string is a continent and adds it to the map by creating a continent obj
bool MapLoader::isContinent(string line, map::Map &test, bool &isValid, int &continentID)
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

    // checks to see if this array index is a string
    try
    {
        int val0 = std::stoi(continentsArr[0]);
        hasFirstString = false;
    }
    catch (std::exception &e)
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

    catch (std::exception &e)
    {
        hasInt = false;
    }

    // checks to see if the array index is a string
    try
    {
        int val2 = std::stoi(continentsArr[2]);
        hasSecondString = false;
    }
    catch (std::exception &e)
    {
        hasSecondString = true;
    }

    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false
    if (isValid == false)
    {
        i = 0;
        hasSecondString = false;
        return false;
    }

    // if it follows the string int string comvination then we store into the map
    if (hasFirstString == true && hasInt == true && hasSecondString == true && i == 3)
    {
        i = 0;                      // resets counter variable
        continents.push_back(line); // store into a map

        // creates a continent object to be added to the list of continents
        map::Continent *continent = new map::Continent(continentID, continentsArr[0], val1);
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
};

// isCountry method whcih checks if the string is a Country to be added to the map by using the territory class
bool MapLoader::isCountry(string line, map::Map &test, bool &isValid)
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

    // gets the string
    stringstream ssin(line);
    char delim = ' ';

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
    catch (std::exception &e)
    {
        isValid = false;
    }

    // tests to see if the first entry is a int
    try
    {
        val0 = std::stoi(countriesArr[0]);
        hasFirstInt = true;
    }
    catch (std::exception &e)
    {
        hasFirstInt = false;
    }

    // tests to see if the second entry is a str
    try
    {
        int val1 = std::stoi(countriesArr[1]);
        hasString = false;
    }
    catch (std::exception &e)
    {
        hasString = true;
    }

    // tests to see if the third entry is a int
    try
    {
        int val2 = std::stoi(countriesArr[2]);
        hasSecondInt = true;
    }

    catch (std::exception &e)
    {
        hasSecondInt = false;
    }

    // tests to see if the fourth entry is a int
    try
    {
        int val3 = std::stoi(countriesArr[3]);
        hasThirdInt = true;
    }

    catch (std::exception &e)
    {
        hasThirdInt = false;
    }

    // tests to see if the fifth entry is a int
    try
    {
        int val4 = std::stoi(countriesArr[4]);
        hasFourthInt = true;
    }

    catch (std::exception &e)
    {

        hasFourthInt = false;
    }

    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false
    if (isValid == false)
    {
        y = 0;
        hasFirstInt = false;
        return false;
    }

    // if all the strings follow the proper sequence of a country add it to the map
    if (hasFirstInt == true && hasString == true && hasSecondInt == true && hasThirdInt == true && hasFourthInt == true && y == 5)
    {
        // stores the territory into the proper class
        map::Territory *territory = new map::Territory(val0, countriesArr[1], *test.getContinent(std::stoi(countriesArr[2])));
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

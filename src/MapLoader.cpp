#include "MapLoader.h"


/*NOTE BEFORE USING THIS FILE CHANGE YOUR CHARSET TO MULTYBITE. IF YOU ARE USING VISUAL STUDIO ENTERPRISE, DO THIS BY GOING INTO PROJECT->PROPERTIES->ADVANCED
THEN CHANGE YOUR CHARSET FROM UNI TO MULTYBYTE
*/

/* ******************************* THIS IS THE OFFICIAL MAPLOADER FILE ************************************* */
// ths map loader detects the files by headers (with correct formating or not) and reads the the file whether if it is valid or not.
// the methods all return boolean values where if they return true, its because the file follows correct syntax and format. 

#include <fstream>
#include <iostream>
#include <filesystem>
#include <locale>
#include <codecvt>

std::wstring string_to_convert;

namespace fs = std::filesystem;
using namespace std;




// default constructor
MapLoader::MapLoader() {
}

//copy constructor
MapLoader::MapLoader(const MapLoader& copyto) {
    this->continents = copyto.continents;
    this->borders = copyto.borders;
    this->countries = copyto.countries;
}

// destructor
MapLoader::~MapLoader() {
}

// OS stream operator 
ostream& operator<<(ostream& out, const MapLoader& o) {

    for (int s = 0; s < o.continents.size(); s++) {
        out << o.continents[s] << endl;

    }
    return out;

}
// took from continent clas to make cglobal 
static int continentID = 1;
//assignment operator 
//MapLoader& MapLoader::operator=(const MapLoader& o){}


// change to take in th eselected fil an dto have the filepath already in here 
 //THIS IS THE PATH TO THE FOLDER THAT WE WANT
  

// methods that takes the string as a filepath a map, continent, and territory objects 
bool MapLoader::loadFile(string mapName, map::Map& test)
{

    bool continentsFound;
    bool countriesFound;
    bool bordersFound;
    static bool isValid = true;
    string line;

     //THIS IS THE PATH TO THE FOLDER THAT WE WANT
    string folderPath = "C:\\Users\\b1hom\\Desktop\\GameEngine\\MapFiles\\" + mapName;

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

                    else {
                        // checks to see if the line is a Continent
                        if (isContinent(line, test, isValid) == false) {
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

                    else {
                        // checks to see if the line is a Continent

                        if (isBorder(line, test, isValid) == false) {
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

                    else {
                        // checks to see if the line is a Continent
                        if (isCountry(line, test, isValid) == false) {
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
bool MapLoader::isBorder(string line, map::Map& test, bool& isValid)
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

    try {
        while (getline(ssin, line, delim)) {
            // stores the variable into the borderArr
            ssin >> bordersArr[0];
            bordersArrReborn.push_back(bordersArr[0]);
        }
    }

    // is an invalid file 
    catch (std::exception& e)
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

        for (int q = 0; q < bordersArrReborn.size() - 1; q++)
        {
            // stores the territories into the variables
            int land1 = std::stoi(bordersArrReborn[0]);
            int land2 = std::stoi(bordersArrReborn[q + 1]);

            // connects the two territories
            test.connectTerritories(land1, land2);

            // adds the territories as neighbours
            test.getTerritory(land1)->addNeighbour(test.getTerritory(land2));
        }
        // is a valid file 
        return true;
    }

    else {
        // is an invalid file 
        return false;
    }
};

// isContinent method which checks to see if the string is a continent and adds it to the map by creating a continent obj 
bool MapLoader::isContinent(string line, map::Map& test, bool& isValid)
{
    // a continent ID which follows standard syntax OF ALL FILES, which is to start at 1, and goes up to N continents
    static int continentID = 1;
    // arrays to store the contents of the line seperated by a space delimiter
    string continentsArr[99];
    // booleans to determine if the line is a continent
    bool hasFirstString; // checks for str
    bool hasInt; // checks for int
    bool hasSecondString; // checks for str

    int i = 0; //a variable to set our pointer to determine the number of strings
    int val1 = 0;

    // gets the string
    stringstream ssin(line);
    char delim = ' ';

    while (getline(ssin, line, delim)) {

        // accepts spaces at the end of file
        if (line.length() == 0) {

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


    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false 
    if (isValid == false) {
        i = 0;
        hasSecondString = false;
        return false;
    }

    // if it follows the string int string comvination then we store into the map
    if (hasFirstString == true && hasInt == true && hasSecondString == true && i == 3)
    {
        i = 0; // resets counter variable
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

    else {
        // resets counter variable
        i = 0;
        // is an invalid file
        return false;
    }
};

// isCountry method whcih checks if the string is a Country to be added to the map by using the territory class
bool MapLoader::isCountry(string line, map::Map& test, bool& isValid)
{
    // stores the string into the elements in the array 
    string countriesArrStore[1];
    vector<string> countriesArr;
    int val0 = 0;
    int val2 = 0;
    // booleans to determine if the line is a countries
    bool hasFirstInt; // check for int
    bool hasString; // check for str
    bool hasSecondInt; // check for int
    bool hasThirdInt; // check for int
    bool hasFourthInt; // check for int

    int y = 0; //a variable to set our pointer to determine the number of strings

    // gets the string
    stringstream ssin(line);
    char delim = ' ';

    try {

        while (getline(ssin, line, delim)) {

            // accepts spaces at the end of a line 
            if (line.length() == 0) {
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

    // if its an invalid file just exit the this method right away by setting the y variable to 0 and all the booleands to false 
    if (isValid == false) {
        y = 0;
        hasFirstInt = false;
        return false;
    }

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

    else {
        // resets the counter to 0
        y = 0;
        // invalid map file
        return false;
    }

};


// create a map list that returns a list of maps to choose from and the selected map is tested rigth away.
// if it isnt valid the user is prompted to select antoher map file. 

bool MapLoader::mapSelection(map::Map& test) {
    // stores the option for the selected  map file 
    int option;
    bool testMap = false;

    // dislays all map files in the directory
    vector<string> listOfFiles;

    do {
        try {
            // stores the map files in a vector 
            listOfFiles = findMapFiles();

            // prints out the map files
            cout << "We have " << listOfFiles.size() << " map files in our system..." << endl;
            for (int i = 0; i < listOfFiles.size(); i++)
                cout << "Type in " << i << " to play on the " << listOfFiles[i] << " file." << endl;

            // stores the option for the selected  map file 
            cout << "Please Type in the number that corresponds to the map of your choice:" << endl;

            cin >> option;

            if (!cin || option > listOfFiles.size() - 1 || option < 0) // or if(cin.fail())
            {
                // user didn't input a number
                cin.clear(); // reset failbit
                cin.ignore(); //skip bad input
                // prompts the while loop to go to the next iteration
                option = -1;
                cout << "Please Type in a valid number!" << endl;
            }

            else {
                //stores the filepath in the string 
                string& map = listOfFiles[option];
                string file = "C:\\Users\\b1hom\\Desktop\\GameEngine\\MapFiles\\" + map;

                // checks to see if it is a valid map, will have to see if theres a way to keep the loop going
                // seems that once its set, the continenets variabel never wants to be updated again. 
                if (mapValidator(file) == false) {
                    testMap = false;
                }

                else {
                    testMap = true;
                    loadFile(file, test);
                }

                if (testMap == false) {
                    cout << "Sadly we can't play on this map because of an error, please choose another number!" << endl;
                }

                else {
                    cout << "Map Generated!" << endl;
                }
            }
        }

        catch (exception e) {
            cout << "Please Type in a valid number!" << endl;
            option = -1;
        }

    } while (testMap == false || option > listOfFiles.size() - 1 || option < 0);

    // returns whether if its a valid map or not 
    return testMap;
};


// finds the list of mapfiles in the selected folder and returns a vector of strings of the names of the map files
vector<string> MapLoader::findMapFiles() {
    std::vector<std::string> listOfFiles;
    // change char set to multibyte by going into the properties and go into advanced and change the char set
    // have to change the filepath to the correct one

    for (auto& p : fs::directory_iterator("C:\\Users\\b1hom\\Desktop\\GameEngine\\MapFiles\\")) {
        std::wstring wide(p.path().filename());
        std::string str(wide.begin(), wide.end());
        listOfFiles.push_back(str);
    }
    return listOfFiles;
}


// VALIDATOR FUNCTIONS 

// ************************************************************************************************************************************************************
// ************************************************************************************************************************************************************
// ************************************************************************************************************************************************************




// method that checks a map file, its headers and sees if it follows correct syntax to determine whether if its valid or not.

// takes in a string and has a dedicated filepath so all we have to do is attach the name of that string with the file extension
// for instance if the folder is located in  "C:\\Users\\b1hom\\Desktop\\GameEngine\\MapFiles\\ then the map file seleced could be Solar.map and we
// would just add it to the string. 
bool MapLoader::mapValidator(string mapName)
{
    bool continentsFound = false;
    bool countriesFound = false;
    bool bordersFound = false;
    bool validFile = true;

    string line;
    
    //THIS IS THE PATH TO THE FOLDER THAT WE WANT
    string folderPath = "C:\\Users\\b1hom\\Desktop\\GameEngine\\MapFiles\\" + mapName;

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

                    else {
                        // checks to see if the line is a Continent
                        if (continentValidator(line) == false) {
                            validFile = false;
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

                    else {
                        // checks to see if the line is a Continent

                        if (borderValidator(line) == false) {
                            
                            validFile = false;
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
                    else {
                        // checks to see if the line is a Continent
                        if (countryValidator(line) == false) {
                            validFile = false;
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
    if (continentsFound == false || validFile == false || countriesFound == false) {
        return false;
    }
    else
        return true;
};


// validator for border
bool MapLoader::borderValidator(string line)
{
    bool isValid = true;
    // stores the parsed string into array elements
    vector<string> bordersArrReborn;
    string bordersArr[1];
    // boolean to determine if the line is a border checks if theyre all ints
    bool foundBorders = true;

    int x = 0; //a variable to set our pointer to determine the number of strings

       // gets the string
    stringstream ssin(line);
    char delim = ' ';

    try {
        while (getline(ssin, line, delim)) {
            // stores the variable into the borderArr
            ssin >> bordersArr[0];
            bordersArrReborn.push_back(bordersArr[0]);
        }
    }

    // is an invalid file 
    catch (std::exception& e)
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
            int val0 = std::stoi(bordersArrReborn[q]);
            foundBorders = true;
        }
        catch (std::exception& e)
        {
            // end of the file stop the loop
            if (bordersArrReborn[q].length() == 0)
            {
                break;
            }

            foundBorders = false;
            break;
        }
    }

    // if only ints were found then we add it to the map
    if (foundBorders == true) {
        for (int q = 0; q < bordersArrReborn.size() - 1; q++)
        {
            // stores the territories into the variables
            int land1 = std::stoi(bordersArrReborn[0]);
            int land2 = std::stoi(bordersArrReborn[q + 1]);
        }
        // is a valid file 
        return true;
    }

    else {
        // is an invalid file 
        return false;
    }
};

// isContinent method which checks to see if the string is a continent and adds it to the map by creating a continent obj 
bool MapLoader::continentValidator(string line)
{
    // a continent ID which follows standard syntax OF ALL FILES, which is to start at 1, and goes up to N continents

    // arrays to store the contents of the line seperated by a space delimiter
    string continentsArr[99];
    // booleans to determine if the line is a continent
    bool hasFirstString; // checks for str
    bool hasInt; // checks for int
    bool hasSecondString; // checks for str

    int i = 0; //a variable to set our pointer to determine the number of strings
    int val1 = 0;

    // gets the string
    stringstream ssin(line);
    char delim = ' ';

    while (getline(ssin, line, delim)) {

        // accepts spaces at the end of file
        if (line.length() == 0) {

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

    // if it follows the string int string comvination then we store into the map
    if (hasFirstString == true && hasInt == true && hasSecondString == true && i == 3)
    {
        i = 0; // resets counter variable
        // is a valid file 
        return true;
    }

    else {
        // resets counter variable
        i = 0;
        // is an invalid file
        return false;
    }
};

// validates the country lines 
bool MapLoader::countryValidator(string line)
{
    // stores the string into the elements in the array 
    string countriesArrStore[1];
    vector<string> countriesArr;
    int val0 = 0;
    int val2 = 0;
    // booleans to determine if the line is a countries
    bool hasFirstInt; // check for int
    bool hasString; // check for str
    bool hasSecondInt; // check for int
    bool hasThirdInt; // check for int
    bool hasFourthInt; // check for int

    int y = 0; //a variable to set our pointer to determine the number of strings

    // gets the string
    stringstream ssin(line);
    char delim = ' ';

    try {

        while (getline(ssin, line, delim)) {

            // accepts spaces at the end of a line 
            if (line.length() == 0) {
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
        return false;
    }

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

    // if all the strings follow the proper sequence of a country add it to the map
    if (hasFirstInt == true && hasString == true && hasSecondInt == true && hasThirdInt == true && hasFourthInt == true && y == 5)
    {

        // sets the string counter to 0
        y = 0;
        // is a vaid map file 
        return true;

    }

    else {
        // resets the counter to 0
        y = 0;
        // invalid map file
        return false;
    }

};
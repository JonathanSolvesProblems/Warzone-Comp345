#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<string.h>

using namespace std;

// a valid map has certain brakcets so if the file doesnt have the proper brakcet headers then 
/// we can essentially say that its an invalid map 

// Need to ask whta are the invalid map requirements and what do the contents of the map 
// mean like Scandinavia 5 #fff780 or in countries 1 s1 1 66 28 or 9 7 8 135 127 in borders 


// INVALID MAP REQUIREMENTS: 
// a invaid map is a map where the territory has no neighbours = unreachable 
// a invalid map doesnt not have all the required ID components of a territory 
// for instance its name, the terriotiry id, the continent Id, 
// under the countries banner = must have 5 elements 
// under contiennts banner = must have 3 elements 
// border cannot have 1 element only 
// border must have the same amount of lines as the number of countries 
// all countries in the border must be initialized 


// have 3 if statements once it reads [files] we store the parses to the files array
// once it reads [continents] we store the parses into the 

// d an if statement that does a while loop until cout !="" empty string 

int main () {
  vector<string> continents; 
  vector<string> borders;    
  vector<string> countries;       
  string line;
  
  // check to see if the files is a proper file and contains these 3 headers to read
  // if not we have to copy the whole file by first identifying if
  // if its a string then we can say that the whole line belongs to a territory 
  // so we would store all the contents of that file into a terriotory 
  // if its a number we have to parse it and see if the second entry is a number
  // if its a number it belongs to the border
  // else it belongs to the Countries 

  bool foundContinents;
  bool foundBorders;
  bool foundCountries;

  ifstream mapFile ("hello.txt");
  if (mapFile.is_open())
  {
    while ( getline (mapFile,line) )
    {
     
        // if it's a normal formated file it collects all the continents
if(line == "[continents]"){
 while ( getline(mapFile,line))
    {
        // this could be another segment but have to figure out how to get this to work for empty space 
        if(line.length() ==0){
            break;
        }

        else{
            continents.push_back(line);
        }

    }

}


// if its a normal formated file it collects all the Countries
if(line == "[countries]"){
 while ( getline(mapFile,line))
    {
        // this could be another segment but have to figure out how to get this to work for empty space 
        if(line.length() ==0){
            break;
        }

        else{
            countries.push_back(line);
        }

    }

}



// if its a normal formated file it collects all the Borders
if(line == "[borders]"){
 while ( getline(mapFile,line))
    {
        // this could be another segment but have to figure out how to get this to work for empty space 
        if(line.length() ==0){
            break;
        }

        else{
            borders.push_back(line);
        }

    }

}



    }
    mapFile.close();
  }

  else cout << "Unable to open file"; 

cout<< "Contientss" << continents.size()<< "\n";
  for (int i=0; i<continents.size();i++){
    cout << continents[i] << endl;
  }

cout<< "borders" <<  borders.size()<< "\n";
  for (int i=0; i<borders.size();i++){
    cout << borders[i] << endl;
  }

  cout<< "countries" << countries.size()<< "\n";
  for (int i=0; i<countries.size();i++){
    cout << countries[i] << endl;
  }



  return 0;
}
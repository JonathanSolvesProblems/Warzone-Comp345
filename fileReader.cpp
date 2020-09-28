#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale>         


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

  // invalid vectors
  vector<string> continents_I; 
  vector<string> borders_I;    
  vector<string> countries_I;  
  // use the test to identify the length of the vector and to see 
  // what positions give you what data type 
  vector<string> test_I;  

  // check to see if the files is a proper file and contains these 3 headers to read
  // if not we have to copy the whole file by first identifying if
  // if its a string then we can say that the whole line belongs to a territory 
  // so we would store all the contents of that file into a terriotory 
  // if its a number we have to parse it and see if the second entry is a number
  // if its a number it belongs to the border
  // else it belongs to the Countries 

// have to implement these under each 
  bool foundContinents1; // checks for str
  bool foundContinents2; // checks for int
  bool foundContinents3; // checks for str

// example output of countries 1 s1 1 66 28
  bool foundCountries1; // check for int
  bool foundCountries2; // check for str
  bool foundCountries3; // check for int
  bool foundCountries4; // check for int
  bool foundCountries5; // check for int

  bool foundBorders;
  

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


    string continentsArr[3];
    string countriesArr[5];
    string bordersArr[999];       

    int i = 0;
    int y = 0;
    int x = 0;
    int z = 0;

  ifstream mapFile ("hello.txt");
  if (mapFile.is_open())
  {
    while ( getline (mapFile,line) )
    {
     
        // if it's a normal formated file it collects all the continents
if(line == "[continents]"){
  // set the continent bool to true 
 while ( getline(mapFile,line))
    {
        // this could be another segment but have to figure out how to get this to work for empty space 
        if(line.length() ==0){
            break;
        }

        else{
          // gets he string 
        stringstream ssin(line);
        // need str int str 
            while (ssin.good() && i < 3){
                ssin >> continentsArr[i];
                ++i;
            }
            i=0;
// tests to see if the first entry is a string
              try
              {

                 int val0 = std::stoi(continentsArr[0]);
                foundContinents1 = false;

                // put the bool to false at the end
                
              }
              catch (std::exception& e)
              {
                foundContinents1 = true;
                // put the bool variable to true
                cout << "This value is a string (what we want). Not an Int. " << '\n';
              }



// tests to see if the second entry is a int 
// for some reason it accepts values like 5tyhuu as a int,
// what we could do is simply  skip the entries who do not have a proper int 

// for invalid articles = if the dont have a header we have to check the length of the 
// vector if the vector length = 3 then its for continents, if the vecor is all ints 
// then its for the borders etc.. 
             try
              {
                // put the bool to true at the end
                int val1 = std::stoi(continentsArr[1]);
                cout << val1<< "this is the value sedfghujkmg";
                foundContinents2 = true;

              }
              catch (std::exception& e)
              {
                // put the bool variable to false
                cout << " Not an Int this is not what we want  " << '\n';
                foundContinents2 = false;

              }

               try
              {
                int val2 = std::stoi(continentsArr[2]);
                foundContinents3 = false;

                
              }
              catch (std::exception& e)
              {
                cout << " Not an Int this is not what we want  " << '\n';
                foundContinents3 = true;

              }

            if(foundContinents1 == true && foundContinents2 == true && foundContinents3 == true){
            continents.push_back(line);
            }
        }

    }

}

//***************************************************************************************************************************
//***************************************************************************************************************************
//***************************************************************************************************************************
//***************************************************************************************************************************


// if its a normal formated file it collects all the Countries
if(line == "[countries]"){
 while ( getline(mapFile,line))
    {
        // this could be another segment but have to figure out how to get this to work for empty space 
        if(line.length() ==0){
            break;
        }

        else{
// test input  1 s1 1 66 28

         // gets he string 
        stringstream ssin(line);
        // need str int str 
            while (ssin.good() && y < 5){
                ssin >> countriesArr[y];
                ++y;
            }
            y=0;
// tests to see if the first entry is a int
              try
              {
              
                 int val0 = std::stoi(countriesArr[0]);
                foundCountries1 = true;
                cout << "good1" << '\n';

                
              }
              catch (std::exception& e)
              {
                foundCountries1 = false;
                cout << "111This value is a string (what we want). Not an Int. " << '\n';
              }


// tests to see if the second entry is a str 
              try
              {

                int val0 = std::stoi(countriesArr[1]);
                foundCountries2 = false;

                
              }
              catch (std::exception& e)
              {
                foundCountries2 = true;
                cout << "good " << '\n';
              }



// tests to see if the third entry is a int 
              try
              {

                int val0 = std::stoi(countriesArr[2]);
                foundCountries3 = true;
                cout << "good " << '\n';

                
              }
              catch (std::exception& e)
              {
                foundCountries3  = false;
                cout << "This value is a string (what we want). Not an Int. " << '\n';
              }


// tests to see if the fourth entry is a int 
              try
              {

                int val0 = std::stoi(countriesArr[3]);
                foundCountries4 = true;
                cout << "good " << '\n';

                
              }
              catch (std::exception& e)
              {
                foundCountries4 = false;
                cout << "This value is a string (what we want). Not an Int. " << '\n';
              }


// tests to see if the fifth entry is a int 
              try
              {

                int val0 = std::stoi(countriesArr[4]);
                foundCountries5 = true;
                cout << "good " << '\n';

                
              }
              catch (std::exception& e)
              {
                foundCountries5 = false;
                cout << "This value is a string (what we want). Not an Int. " << '\n';
              }





            if(foundCountries1 == true && foundCountries2 == true && foundCountries3 == true && foundCountries4 == true && foundCountries5 == true)
            countries.push_back(line);
        }

    }

}


//***************************************************************************************************************************
//***************************************************************************************************************************
//***************************************************************************************************************************
//***************************************************************************************************************************



// if its a normal formated file it collects all the Borders
if(line == "[borders]"){
 while ( getline(mapFile,line))
    {
        // this could be another segment but have to figure out how to get this to work for empty space 
        if(line.length() ==0){
            break;
        }

        else{

    // gets he string 
        stringstream ssin(line);
        // need str int str 
            while (ssin.good()){
                ssin >> bordersArr[x];
                ++x;
            }
           
cout << "size of x tfgyhjkmjhgf "<< x;
            // set the size of the array to check the strs
            string bordersArr2[5];
            x=0;
 // puts the string in appropriate array
    
        // need str int str 

          for (int r = 0; r < sizeof(bordersArr2)/sizeof(bordersArr2[0]) ; r++) {
              bordersArr2[r] = bordersArr[r];
          }
          
            cout << "size of  borderarr "<< sizeof(bordersArr2)/sizeof(bordersArr2[0]);
            cout << "FIRST EELE "<< bordersArr2[0];


            // run a for loop ad basically have 1 variable if it is not a int then have it 
            // go to an if statement where the boolean is false and then break the loop
            for (int q = 0; q < sizeof(bordersArr2)/sizeof(bordersArr2[0]) ; q++) {
            

              try
              {

                int val0 = std::stoi(bordersArr2[q]);
                foundBorders = true;
                cout << "good " << '\n';

                
              }
              catch (std::exception& e)
              {
                foundBorders = false;
                cout << "We dont add these set of borders to our map" << '\n';
                break;
              }


          }
          

            if(foundBorders == true)
            borders.push_back(line);

        }

    }

}



    }
    mapFile.close();
  }

  else cout << "Unable to open file"; 

cout<< "Contients  " << continents.size()<< " \n";
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
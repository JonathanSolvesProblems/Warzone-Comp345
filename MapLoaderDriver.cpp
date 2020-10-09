#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <locale>
#include "MapLoader.h"




// THIS IS THE MAP LOADER DRIVER FILE THAT TAKES A MAP AND DISPLAYS THE CONTINENTS, THE TERRITORIES, THE AMOUNT OF NEIGHBOURS A TERRITORY HAS ALONG WITH THE CONTINENT 
// IT BELONGS TO WITH THE TERITORY BONUS.

// NOTE** IF THE MAP FILE HAS SPACES AFTER THE LAST ELEMENT OF A LINE IT WONT ACCEPT IT AS FOLLOWING PROTOCOL AND WILL REJECT THE FILE 


int main()
{

    map::Map test;
    mapLoader map;


    if (map.loadFile("france.map", test) == true) {


        // remove 2 at the end
        cout << "Continents  " << map.continents.size() << " \n";
        for (int s = 0; s < map.continents.size(); s++)
        {
            cout << *test.getContinent(s + 1) << endl;
        }


        cout << "Countries and Neighbours: " << "\n";
        for (int s = 0; s < map.borders.size(); s++)
        {
            cout << "Country: " << *test.getTerritory(s + 1) << " Has " << test.getTerritory(s + 1)->getNeighbourCount() << " Neighbouring Countries And Belongs to " << test.getTerritory(s + 1)->getContinent() << endl;
        }

        // have to run a forloop through all the neighbors to display if it is a neighbour 


        // try to figure out how to send a vector containign the IDs of the neighborung territoryeis 


    }


    else {

        cout << "Invalid file... System terminating";
    }



    return 0;
}
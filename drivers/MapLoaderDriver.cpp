#include "MapLoader.h"
#include <sstream>
#include <string>       // std::string
#include <iostream>
using std::cout;
using std::endl;

/*THE VALIDATE SEMI WORKS, IT SAYS AND DISPLAYS THAT IT TRAVERSES TEH WHOLE GRAPH, BUT THE THEN RETURNS FALSE FOR CERTSAIN SUB GRAPHS, BUT IN THE DISPLAY OF
ALL TH ETERIRTORES AND COTNIENTS IT SHOWS ALL ITS NEIGTHBORS AND EVRTHING PROPERLY.
*/
// THIS IS THE MAP LOADER DRIVER FILE THAT TAKES A MAP AND DISPLAYS THE CONTINENTS, THE TERRITORIES, THE AMOUNT OF NEIGHBOURS A TERRITORY HAS ALONG WITH THE CONTINENT
// IT BELONGS TO WITH THE TERITORY BONUS.

// NOTE** IF THE MAP FILE HAS SPACES AFTER THE LAST ELEMENT OF A LINE IT WONT ACCEPT IT AS FOLLOWING PROTOCOL AND WILL REJECT THE FILE


// make it return a map object and 

int main()
{

    // issue is the conquestfiledreader works, but i cant seem to get it to call the proper function and class inside the conquestfilereaderAdapter class. this is not reading anythignfor some reaosn 
    map::Map* test = new map::Map();
    MapLoader map;



    if (map.loadFile("conquest.map", *test)) {
     
        int count = 1;
        while (test->getContinent(count)) {
            cout << *test->getContinent(count) << endl;
            count++;
        }
       
        int count2 = 1;
        while (test->getTerritory(count2)) {
            cout << "Country: " << *test->getTerritory(count2) << " Has " << test->getTerritory(count2)->getNeighbourCount() << " Neighbouring Countries And Belongs to " << *(test->getTerritory(count2)->getContinent()) << endl;
            count2++;
        }

        if (test->validate()) {
            cout << "true";
        }
        else
            cout << "false";

    }

    else {
    
        cout << "not a valid file";
    
    }


    return 0;
}
#include "MapLoader.h"

using std::cout;
using std::endl;

/*THE VALIDATE SEMI WORKS, IT SAYS AND DISPLAYS THAT IT TRAVERSES TEH WHOLE GRAPH, BUT THE THEN RETURNS FALSE FOR CERTSAIN SUB GRAPHS, BUT IN THE DISPLAY OF 
ALL TH ETERIRTORES AND COTNIENTS IT SHOWS ALL ITS NEIGTHBORS AND EVRTHING PROPERLY. 
*/
// THIS IS THE MAP LOADER DRIVER FILE THAT TAKES A MAP AND DISPLAYS THE CONTINENTS, THE TERRITORIES, THE AMOUNT OF NEIGHBOURS A TERRITORY HAS ALONG WITH THE CONTINENT
// IT BELONGS TO WITH THE TERITORY BONUS.

// NOTE** IF THE MAP FILE HAS SPACES AFTER THE LAST ELEMENT OF A LINE IT WONT ACCEPT IT AS FOLLOWING PROTOCOL AND WILL REJECT THE FILE

int main()
{

    map::Map* test = new map::Map();
    ConquestFileReader map;

        if(map.loadFile("conquest.map",*test)){
        cout << "Continents  " << map.getNumOfContinents() << " \n";

            cout << "Country: " << *test->getTerritory(0 + 1) << " Has " << test->getTerritory(0 + 1)->getNeighbourCount() << " Neighbouring Countries And Belongs to " << *(test->getTerritory(0 + 1)->getContinent()) << endl;
        }
    

    return 0;
}
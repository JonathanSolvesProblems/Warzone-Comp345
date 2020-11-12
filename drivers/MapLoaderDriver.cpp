#include "MapLoader.h"

int main()
{

    // map and map laoder objects for test purposes;
    map::Map test22;
    MapLoader map22;



    // tests the map selection, gracefully rejects invalid map files, and if a valid file is selected it generates the map
    map22.mapSelection(test22);
    cout << endl;

    // generates a map and tests it out after a user selects a correct mapFile
    cout << map22.borders.size();

    for (int s = 0; s < map22.continents.size(); s++)
    {
        cout << *test22.getContinent(s + 1) << endl;
    }

    cout << "Countries and Neighbours: " << "\n";
    for (int s = 0; s < map22.borders.size(); s++)
    {
        cout << "Country: " << *test22.getTerritory(s + 1) << " Has " << test22.getTerritory(s + 1)->getNeighbourCount() << " Neighbouring Countries And Belongs to " << *(test22.getTerritory(s + 1)->getContinent()) << endl;
    }


    return 0;
}
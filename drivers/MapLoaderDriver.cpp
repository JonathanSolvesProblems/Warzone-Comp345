#include "MapLoader.h"


// THIS IS THE MAP LOADER DRIVER FILE THAT TAKES A MAP AND DISPLAYS THE CONTINENTS, THE TERRITORIES, THE AMOUNT OF NEIGHBOURS A TERRITORY HAS ALONG WITH THE CONTINENT 
// IT BELONGS TO WITH THE TERITORY BONUS.

// NOTE** IF THE MAP FILE HAS SPACES AFTER THE LAST ELEMENT OF A LINE IT WONT ACCEPT IT AS FOLLOWING PROTOCOL AND WILL REJECT THE FILE 


int main()
{

    map::Map test;
    MapLoader map;


    if (map.loadFile("exampleMaps/france.map", test) == true) {


        // remove 2 at the end
        cout << "Continents  " << map.continents.size() << " \n";
        for (int s = 0; s < map.continents.size(); s++)
        {
            cout << *test.getContinent(s + 1) << endl;
        }


        cout << "Countries and Neighbours: " << "\n";
        for (int s = 0; s < map.borders.size(); s++)
        {
            cout << "Country: " << *test.getTerritory(s + 1) << " Has " << test.getTerritory(s + 1)->getNeighbourCount() << " Neighbouring Countries And Belongs to " << *(test.getTerritory(s + 1)->getContinent()) << endl;
        }


        // have to run a forloop through all the neighbors to display if it is a neighbour 


        // try to figure out how to send a vector containign the IDs of the neighborung territoryeis 

        // to get the list of neighbours you can do a forloop  with a try catch and store the successful ones in the vector
        // so the sequence is we display the terrotories the user has 
        // then we prompt for them to select a territory to make a decision on
        // then we run this for loop to get the list of places you could attack  
       // cout<< *test.getTerritory(1)->getNeighbour(39);

    }


    else {

        cout << "Invalid file... System terminating" << endl;
    }


    // invalid Map
    map::Map invalid;
    MapLoader map3;


    // testing invalid file 

    cout << endl << endl << endl << "Testing invalid hello file"<< endl;


    if (map3.loadFile("exampleMaps/hello.txt", invalid) == true) {


        // remove 2 at the end
        cout << "Continents  " << map3.continents.size() << " \n";
        for (int s = 0; s < map3.continents.size(); s++)
        {
            cout << *test.getContinent(s + 1) << endl;
        }


        cout << "Countries and Neighbours: " << "\n";
        for (int s = 0; s < map3.borders.size(); s++)
        {
            cout << "Country: " << *invalid.getTerritory(s + 1) << " Has " << invalid.getTerritory(s + 1)->getNeighbourCount() << " Neighbouring Countries And Belongs to " << *(invalid.getTerritory(s + 1)->getContinent()) << endl;
        }

      
    }


    else {

        cout << "Invalid file... System terminating" << endl;
    }




// copy constructor 
  MapLoader* map4 =  new MapLoader(map);

cout<< "copy Constructor " << map4->continents[0] << endl << endl;
cout<< map4->continents.size() << endl;
cout<< map4->borders.size() << endl;
cout<< map4->countries.size() << endl;
cout<< map.borders[1];
cout<< endl<< endl<<" this is the OS operator" <<endl;
// out put 
cout << *map4;


    return 0;
}
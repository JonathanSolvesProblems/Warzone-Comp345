#include "Orders.h"
#include <string>
using std::cout;

// Setup players
Player anthony = Player("Anthony", 12345678);
Player steve = Player("Steve", 12345670);
// Setup continents
map::Continent canada = map::Continent(0, "Canada", 3);
map::Continent freedomland = map::Continent(1, "Freedomland", 99);
// Setup territories
map::Territory quebec = map::Territory(0, "Quebec", canada);
map::Territory newJersey = map::Territory(1, "New Jersey", freedomland);
map::Territory texas = map::Territory(2, "Texas", freedomland);


void deployOrderTest() {
	// Set neighbors
	newJersey.addNeighbour(&texas);
	newJersey.addNeighbour(&quebec);
	
	//Set owners
	quebec.setOwner(&steve);
	newJersey.setOwner(&anthony);
	texas.setOwner(&anthony);

	anthony.setArmees(30);
	steve.setArmees(3);

	// Set armies
	newJersey.setArmees(6);
	quebec.setArmees(10);
	texas.setArmees(3);

	DeployOrder* deployValid = new DeployOrder(anthony,texas,2);
	cout << "----------------------------------------------------" << endl;
	cout << "Deploy Anthony's 2 armies to Texas " << endl;
	cout << "Texas armies BEFORE deploy: " << texas.getArmees() << endl;
	deployValid->execute();
	cout << "Texas armies AFTER deploy: " << texas.getArmees() << endl;

	DeployOrder* deployInvalid = new DeployOrder(steve,quebec,5);
	cout << "----------------------------------------------------" << endl;
	cout << "Deploy Anthony's 2 armies to Quebec BUT steve only has 1 army " << endl;
	cout << "Quebec armies BEFORE deploy: " << quebec.getArmees() << endl;
	deployInvalid->execute();
	cout << "Quebec armies AFTER deploy (should not change): " << quebec.getArmees() << endl;

	DeployOrder* deployInvalid2 = new DeployOrder(steve,newJersey,2);
	cout << "----------------------------------------------------" << endl;
	cout << "Try to deploy Steve's armies to a territory he doesn't own(invalid)" << endl;
	cout << "New Jersey armies BEFORE deploy: " << newJersey.getArmees() << endl;
	deployInvalid2->execute();
	cout << "New Jersey armies AFTER deploy (should not change): " << newJersey.getArmees() << endl;



	//TODO DELETE
}

void advanceOrderTest() {
	// Set neighbors
	newJersey.addNeighbour(&texas);
	newJersey.addNeighbour(&quebec);
	
	//Set owners
	quebec.setOwner(&steve);
	newJersey.setOwner(&anthony);
	texas.setOwner(&anthony);

	// Set armies
	newJersey.setArmees(6);
	quebec.setArmees(10);
	texas.setArmees(3);

	cout << "----------------------------------------------------" << endl;
	cout << "Advance with two territories that are not neighbors." << endl;
	cout << "Before: " << texas << endl << quebec << endl;
	AdvanceOrder* advanceDiff = new AdvanceOrder(anthony, texas, quebec, 2);
	advanceDiff->execute();
	cout << "After: " << texas << endl << quebec << endl;
	cout << "----------------------------------------------------" << endl;
	cout << "Advance with two territories owned by the same player." << endl;
	cout << "Before: " << newJersey << endl << texas << endl;
	AdvanceOrder* advanceSame = new AdvanceOrder(anthony, newJersey, texas, 1);
	advanceSame->execute();
	cout << "After: " << newJersey << endl << texas << endl;
	cout << "----------------------------------------------------" << endl;
	cout << "Advance with two territories owned by different players." << endl;
	cout << "Defender wins:" << endl;
	cout << "Before: " << newJersey << endl << quebec << endl;
	delete advanceDiff;
	advanceDiff = new AdvanceOrder(anthony, newJersey, quebec, 4);
	advanceDiff->execute();
	cout << "After: " << newJersey << endl << quebec << endl;
	cout << "-------------------------" << endl;
	newJersey.setArmees(20);
	quebec.setArmees(4);

	cout << "Hand BEFORE conquer: ";
	anthony.hand->show();
	cout << "Attacker wins:" << endl;
	cout << "Before: " << newJersey << endl << quebec << endl;
	delete advanceDiff;
	advanceDiff = new AdvanceOrder(anthony, newJersey, quebec, 20);
	advanceDiff->execute();
	cout << "After: " << newJersey << endl << quebec << endl;
	cout << "Hand AFTER conquer: ";
	anthony.hand->show();
	cout << "----------------------------------------------------" << endl;
	delete advanceSame;
	delete advanceDiff;
}

void airliftOrderTest() {
	//Set owners
	quebec.setOwner(&steve);
	newJersey.setOwner(&anthony);
	texas.setOwner(&anthony);

	// Set armies
	newJersey.setArmees(15);
	quebec.setArmees(10);
	texas.setArmees(5);

	cout << "Airlift with two territories owned by the same player." << endl;
	cout << "Before: " << texas << endl << newJersey << endl;
	AirliftOrder* airliftSame = new AirliftOrder(anthony, texas, newJersey, 4);
	airliftSame->execute();
	cout << "After: " << texas << endl << newJersey << endl;
	cout << "----------------------------------------------------" << endl;
	cout << "Airlift with two territories owned by different players." << endl;
	cout << "Defender wins:" << endl;
	cout << "Before: " << newJersey << endl << quebec << endl;
	AirliftOrder* airliftDiff = new AirliftOrder(anthony, newJersey, quebec, 4);
	airliftDiff->execute();
	cout << "After: " << newJersey << endl << quebec << endl;
	cout << "-------------------------" << endl;
	newJersey.setArmees(20);
	quebec.setArmees(4);
	cout << "Attacker wins:" << endl;
	cout << "Before: " << newJersey << endl << quebec << endl;
	delete airliftDiff;
	airliftDiff = new AirliftOrder(anthony, newJersey, quebec, 20);
	airliftDiff->execute();
	cout << "After: " << newJersey << endl << quebec << endl;
	cout << "----------------------------------------------------" << endl;
	delete airliftSame;
	delete airliftDiff;
}

void bombOrderTest() {
	//Set owners
	quebec.setOwner(&steve);
	newJersey.setOwner(&anthony);
	texas.setOwner(&anthony);

	// Set armies
	newJersey.setArmees(15);
	quebec.setArmees(11);
	texas.setArmees(5);

	// Anthony bombs Steve
	BombOrder* validBomb = new BombOrder(anthony,steve,quebec);

	cout << "Number of armies on quebec before anthony bombs it: " << quebec.getArmees() << endl;
	validBomb->execute();
	cout << "Number of armies on quebec AFTER anthony bombs it: "<< quebec.getArmees() << endl;

	cout << "Making sure you can't bomb yourself" << endl;
	BombOrder* invalidBomb = new BombOrder(anthony,anthony,newJersey);
	bool successBomb = invalidBomb->execute();
	cout << "Bomb shouldn't be successful (zero here): " << successBomb << endl;
	delete validBomb;
	delete invalidBomb;
}

void blockadeOrderTest() {
	//Set owners
	quebec.setOwner(&steve);
	newJersey.setOwner(&anthony);
	texas.setOwner(&anthony);

	// Set armies
	newJersey.setArmees(15);
	quebec.setArmees(11);
	texas.setArmees(5);

	// Anthony blockades New Jersey
	BlockadeOrder* validBlockade = new BlockadeOrder(anthony,newJersey);
	cout << "Number of armies BEFORE blockade in New Jersey: " << newJersey.getArmees() << endl;
	cout << "Owner BEFORE blockade: " << *newJersey.getOwner() << endl;
	validBlockade->execute();
	cout << "Number of armies AFTER blockade in New Jersey (should be double): " << newJersey.getArmees() << endl;
	cout << "Owner AFTER blockade (should be 0 for neutral): " << newJersey.getOwner() << endl;

	// Anthony blockades Quebec (bad)
	cout << "Player executes blockade on enemy territory(invalid)" << endl;
	BlockadeOrder* invalidBlockade = new BlockadeOrder(anthony,quebec);
	cout << "Zero shown here if blockade didn't occur: " << invalidBlockade->execute() << endl;

	delete validBlockade;
	delete invalidBlockade;
}

void negotiateOrderTest() {
	//Set owners
	quebec.setOwner(&steve);
	newJersey.setOwner(&anthony);
	texas.setOwner(&anthony);

	// Set armies
	newJersey.setArmees(15);
	quebec.setArmees(11);
	texas.setArmees(5);

	cout << "Testing if negotiate cancels a bomb:" << endl;
	NegotiateOrder* validNegotiate = new NegotiateOrder(anthony,steve);
	validNegotiate->execute();
	cout << "Troops on quebec BEFORE bomb: " << quebec.getArmees() << endl;
	BombOrder* blockedBomb = new BombOrder(anthony,steve,quebec);
	bool hasBombed = blockedBomb->execute();
	cout << "Troops on quebec AFTER bomb of player with truce:  " << quebec.getArmees() << endl;

	delete validNegotiate;
	delete blockedBomb;
}

int main() {
	deployOrderTest();
	advanceOrderTest();
	airliftOrderTest();
	bombOrderTest();
	blockadeOrderTest();
	negotiateOrderTest();

	return 0;
}

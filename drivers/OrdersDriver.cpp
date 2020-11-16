#include "Orders.h"
#include <string>

template<typename T>
void orderTest() {
	T orderTest;
	cout << "--- " << orderTest << " Constructors ---" << endl;
	T orderTest2(orderTest);
	// debug point here
	cout << "--- " << orderTest << " Constructors test complete ---" << endl;
	cout << "--- Service Methods ---" << endl;
	cout << "validate() should be true for both objects (1 means true)." << endl;
	cout << "orderTest: " << orderTest.validate() << endl;
	cout << "orderTest2: " << orderTest2.validate() << endl;
	cout << "execute() will output the effect of each order. It will also return whether the order was executed or not." << endl;
	cout << "orderTest: " << orderTest.execute() << endl;
	cout << "orderTest2: " << orderTest2.execute() << endl;
	cout << "Stream insertion operator overloading test:" << endl;
	cout << orderTest << endl;
	cout << orderTest2 << endl;
	cout << "Assignment operator test: " << endl;
	T orderTest3;
	orderTest3 = orderTest2;
	// debug point here
	cout << "--- Service Methods test complete ---" << endl;
}

void ordersListTest() {
	cout << "--- Adding Every Kind of Order ---" << endl;
	OrdersList test;
	Order* test2 = new AdvanceOrder();
	test.add(test2);
	cout << test;
	Order* test3 = new AirliftOrder();
	test.add(test3);
	cout << test;
	Order* test4 = new BlockadeOrder();
	test.add(test4);
	cout << test;
	Order* test5 = new BombOrder();
	test.add(test5);
	cout << test;
	Order* test6 = new DeployOrder();
	test.add(test6);
	cout << test;
	Order* test7 = new NegotiateOrder();
	test.add(test7);
	cout << test;
	cout << "--- Testing Removing the Fourth Order ---" << endl;
	test.remove(3);
	cout << test;
	cout << "--- Testing Moving the First Order to the Third Position ---" << endl;
	test.move(0, 3);
	cout << test;
	cout << "--- Testing Moving the Fourth Order to the First Position ---" << endl;
	test.move(3, 0);
	cout << test;
	cout << "--- Testing Assignment Operator and Copy Constructor ---" << endl;
	OrdersList newTest;
	newTest = test;
	// debug point here
	OrdersList* newTest2 = new OrdersList(newTest);
	cout << test;
}

void ordersValidationTest() {
	//TODO: Re-use these for the same tests?
	Player anthony = Player("Anthony", 12345678);
	Player steve = Player("Steve", 12345670);
	map::Continent test = map::Continent(0, "Canada", 3);
	map::Continent test2 = map::Continent(1, "Freedomland", 99);
	map::Territory stevesTerr = map::Territory(0, "Quebec", test);
	map::Territory anthonysTerr = map::Territory(1, "New Jersey", test2);
	map::Territory anthonysOtherTerr = map::Territory(2, "Texas", test2);
	//Set owners
	//TODO: set without pointer?
	stevesTerr.setOwner(&steve);
	anthonysTerr.setOwner(&anthony);
	anthonysOtherTerr.setOwner(&anthony);

	AdvanceOrder* advanceGood = new AdvanceOrder(anthony, anthonysTerr, anthonysOtherTerr, 3);
	AirliftOrder* airliftGood = new AirliftOrder(anthony, anthonysTerr, anthonysTerr, 3);
	BombOrder* bombGood = new BombOrder(anthony, steve, stevesTerr);
	BlockadeOrder* blockadeGood = new BlockadeOrder(anthony, anthonysTerr);
	DeployOrder* deployGood = new DeployOrder(anthony, anthonysTerr, 19);
	NegotiateOrder* negotiateGood = new NegotiateOrder(anthony, steve);

	cout << "Validating Advance order, should show 1 as true: " << advanceGood->validate() << "\n";
	cout << "Validating Airlift order, should show 1 as true: " << airliftGood->validate() << "\n";
	cout << "Validating Blockade order, should show 1 as true: " << blockadeGood->validate() << "\n";
	cout << "Validating Bomb order, should show 1 as true: " << bombGood->validate() << "\n";
	cout << "Validating Deploy order, should show 1 as true: " << deployGood->validate() << "\n";
	cout << "Validating Negotiate order, should show 1 as true: " << negotiateGood->validate() << "\n";

	delete advanceGood;
	delete airliftGood;
	delete blockadeGood;
	delete bombGood;
	delete deployGood;
	delete negotiateGood;
}

void ordersExecutionTest() {
	// Setup players
	Player anthony = Player("Anthony", 12345678);
	Player steve = Player("Steve", 12345670);

	// Setup continents
	map::Continent test = map::Continent(0, "Canada", 3);
	map::Continent test2 = map::Continent(1, "Freedomland", 99);

	// Setup territories
	map::Territory stevesTerr = map::Territory(0, "Quebec", test);
	map::Territory anthonysTerr = map::Territory(1, "New Jersey", test2);
	map::Territory anthonysOtherTerr = map::Territory(2, "Texas", test2);
	anthonysTerr.addNeighbour(&anthonysOtherTerr);
	anthonysTerr.setArmees(2);
	anthonysOtherTerr.setArmees(3);

	//Set owners
	//TODO: set without pointer?
	stevesTerr.setOwner(&steve);
	anthonysTerr.setOwner(&anthony);
	anthonysOtherTerr.setOwner(&anthony);

	// Advance with two territories owned by the same player
	cout << "Before: " << anthonysTerr << endl << anthonysOtherTerr << endl;
	AdvanceOrder* advanceGood = new AdvanceOrder(anthony, anthonysTerr, anthonysOtherTerr, 100);
	advanceGood->execute();
	cout << "After: " << anthonysTerr << endl << anthonysOtherTerr << endl;
	
	AirliftOrder* airliftGood = new AirliftOrder(anthony, anthonysTerr, anthonysTerr, 3);
	BombOrder* bombGood = new BombOrder(anthony, steve, stevesTerr);
	BlockadeOrder* blockadeGood = new BlockadeOrder(anthony, anthonysTerr);
	DeployOrder* deployGood = new DeployOrder(anthony, anthonysTerr, 19);
	NegotiateOrder* negotiateGood = new NegotiateOrder(anthony, steve);


	// Player anthony = Player("Anthony", 12345678);
	// Player steve = Player("Steve", 12345670);
	// map::Continent test = map::Continent(0, "Canada", 3);
	// map::Continent test2 = map::Continent(1, "Freedomland", 99);
	// map::Territory terTest = map::Territory(0, "Quebec", test);
	// map::Territory terTest2 = map::Territory(1, "New Jersey", test2);
	
	// AirliftOrder* airlift = new AirliftOrder(anthony, terTest, anthony, terTest2, 8);
	airliftGood->execute();
	// BlockadeOrder* blockade = new BlockadeOrder(anthony, terTest);
	blockadeGood->execute();
	// BombOrder* bomb = new BombOrder(anthony, terTest);
	bombGood->execute();
	// DeployOrder* deploy = new DeployOrder(anthony, terTest, 19);
	deployGood->execute();
	// NegotiateOrder* negotiate = new NegotiateOrder(anthony, steve);
	negotiateGood->execute();
	delete advanceGood;
	delete airliftGood;
	delete blockadeGood;
	delete bombGood;
	delete deployGood;
	delete negotiateGood;
}

int main() {
	// orderTest<AdvanceOrder>();
	// cout << endl;
	// orderTest<AirliftOrder>();
	// cout << endl;
	// orderTest<BlockadeOrder>();
	// cout << endl;
	// orderTest<BombOrder>();
	// cout << endl;
	// orderTest<DeployOrder>();
	// cout << endl;
	// orderTest<NegotiateOrder>();
	// cout << endl;
	// ordersListTest();
	ordersExecutionTest();
	//ordersValidationTest();
	return 0;
}

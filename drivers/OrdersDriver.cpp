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

void ordersExecutionTest() {
	map::Continent test = map::Continent(0, "Canada", 3);
	map::Continent test2 = map::Continent(1, "Freedomland", 99);
	map::Territory terTest = map::Territory(0, "Quebec", test);
	map::Territory terTest2 = map::Territory(1, "New Jersey", test2);
	AdvanceOrder* advance = new AdvanceOrder(terTest, terTest2, 3);
	advance->execute();
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
	return 0;
}

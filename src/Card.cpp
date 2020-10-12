#include "Card.h"

Card::Card() {}

Card::Card(const Card& cardCopy) {}

Spy::Spy() {}
Bomb::Bomb() {}
Reinforcement::Reinforcement() {}
Blockage::Blockage() {}
Airlift::Airlift() {}
Diplomacy::Diplomacy() {}

Card::~Card() {}
Spy::~Spy() {}
Bomb::~Bomb() {}
Reinforcement::~Reinforcement() {}
Blockage::~Blockage() {}
Airlift::~Airlift() {}
Diplomacy::~Diplomacy() {}

ostream& Card::operator<<(ostream& os)
{
	return os;
}

Deck::~Deck() {
	for (auto deck : _deck)
	{
		delete deck;
	}
	_deck.clear();
}

const Card& Card::operator=(const Card& cards) {
	// Not necessary for now.
	// compare private data members if any necessary.
	return cards;
}



void Spy::play() {
	cout << "Spy::play()" << endl;
	// create special orders
}

void Bomb::play() {
	cout << "Bomb::play()" << endl;
	// create special orders
}

void Reinforcement::play() {
	cout << "Reinforcement::play()" << endl;
	// create special orders
}

void Blockage::play() {
	cout << "Blockage::play()" << endl;
	// create special orders
}

void Airlift::play() {
	cout << "Airlift::play()" << endl;
	// create special orders
}

void Diplomacy::play() {
	cout << "Diplomacy::play()" << endl;
	// create special orders
}

// shuffles deck of cards
void Deck::shuffle(vector<Card*>& deck) {

	srand(time(nullptr)); // reset random seed

	for (int s1 = 0; s1 < deck.size() - 1; s1++) {

		int s2 = s1 + rand() % (deck.size() - s1);

		// swapping cards
		Card* temp = deck.at(s1);
		deck.at(s1) = deck.at(s2);
		deck.at(s2) = temp;
	}
}

Deck::Deck() {
	// test cases
	_deck = { new Spy(), new Bomb(), new Reinforcement(), new Blockage(), new Airlift(), new Diplomacy() }; // to add more cards, testing with the 6 different types.

	shuffle(_deck);

	cout << "deck created...\n\n";
}

Card* Deck::draw() {

	// storing card to be drawn
	Card* drawn = _deck.at(_deck.size() - 1);

	// erasing drawn card from deck
	_deck.erase(_deck.begin() + (_deck.size() - 1));

	// returning drawn card
	return drawn;
}

Hand::Hand() {}

void Hand::showHand() {

	cout << "The hand contains: " << "\n\n";

	for (auto elem : _hand)
		elem->play();

	cout << "\n\n";
}

void Hand::addHand(Card* drawn) {

	cout << "card drew successfully...\n";

	_hand.push_back(drawn);
		
}

Hand::~Hand() {
	for (auto deck : _hand)
	{
		delete deck;
	}
	_hand.clear();
}
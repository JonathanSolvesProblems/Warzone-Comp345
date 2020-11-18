#include "Card.h"

// Default Constructers
Card::Card() {}
Spy::Spy() {} 
Bomb::Bomb() {}
Reinforcement::Reinforcement() {}
Blockage::Blockage() {} 
Airlift::Airlift() {}
Diplomacy::Diplomacy() {} 
Hand::Hand() {}

Deck::Deck() {
	// Hardcoding test cases to add for the deck.
	_deck = { new Spy(), new Bomb(), new Reinforcement(), new Blockage(), new Airlift(), new Diplomacy(), 
	new Spy(), new Bomb(), new Reinforcement(), new Blockage(), new Airlift(), new Diplomacy(),
	new Spy(), new Bomb(), new Reinforcement(), new Blockage(), new Airlift(), new Diplomacy(),
	new Spy(), new Bomb(), new Reinforcement(), new Blockage(), new Airlift(), new Diplomacy() };

	shuffle(_deck);
}

// Copy Constructers
Card::Card(const Card& cardCopy) {}

// equals operator overloading

Hand& Hand::operator=(const Hand& hand) { 
	return *this;
}

// Destructers
Card::~Card() {} 
Spy::~Spy() {} 
Bomb::~Bomb() {} 
Reinforcement::~Reinforcement() {}
Blockage::~Blockage() {} 
Airlift::~Airlift() {} 
Diplomacy::~Diplomacy() {}

// Memory management for vector of pointers in destructor. 
Deck::~Deck() {
	for (auto card : _deck)
	{
		delete card;
	}
	_deck.clear();
}

Hand::~Hand() {
	for (auto deck : cards)
	{
		delete deck;
	}
	cards.clear();
}

// Overriding play methods, depending on card type
void Spy::play() {
	cout << "Spy Card\n";
}

void Bomb::play() {
	cout << "Bomb Card\n";
}

void Reinforcement::play() {
	cout << "Reinforcement Card\n";
}

void Blockage::play() {
	cout << "Blockage Card\n";
}

void Airlift::play() {
	cout << "Airlift Card\n";
}

void Diplomacy::play() {
	cout << "Diplomacy Card\n";
}

// shuffles deck of cards
void Deck::shuffle(vector<Card*>& deck) {

	srand(time(nullptr)); // reset random seed to not get same random # of cards each shuffle.

	// swap cards
	for (int s1 = 0; s1 < deck.size() - 1; s1++) {

		int s2 = s1 + rand() % (deck.size() - s1);

		// swapping cards
		Card* temp = deck.at(s1);
		deck.at(s1) = deck.at(s2);
		deck.at(s2) = temp;
	}
}

// draw from deck. Return card that was drawn.
Card* Deck::draw() {

	// storing card to be drawn
	Card* drawn = _deck.at(_deck.size() - 1);

	// erasing drawn card from deck
	_deck.erase(_deck.begin() + (_deck.size() - 1));

	return drawn;
}

void Hand::show() { // display's contents of the hand.
#ifdef DEBUG
	cout << "The hand contains: " << "\n\n";
#endif
	for (auto elem : cards)
		elem->play();

	cout << "\n\n";
}

void Hand::add(Card* drawn) { // adds card to hand, based on card that was drew.
#ifdef DEBUG
	cout << "card drew successfully...\n";
#endif
	cards.push_back(drawn);
		
}

// insertion operator overloading

ostream& operator<<(ostream& os, const Deck& deck) {
	os << "(Deck " + std::to_string(deck._deck.size()) + ")" << std::endl;
	return os;
}

ostream& operator<<(ostream& os, const Hand& hand) {
	os << "(Hand " + std::to_string(hand.cards.size()) + ")" << std::endl;
	return os;
}

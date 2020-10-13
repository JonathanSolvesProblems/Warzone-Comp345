#include "Card.h"

int main() {
	// creates deck of cards of all different kinds.
	cout << "creating deck...\n\n";
	Deck deck;

	// creates a hand object that is filled by drawing cards from the deck.
	cout << "Drawing 9 cards from the deck...\n\n";
	Hand hand;

	// drawing cards from deck and adding them to the hand.
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());
	hand.addHand(deck.draw());

	// calls play method of all cards in the hand.
	cout << "\nRevealing hand...\n\n";
	hand.showHand();

	return 0;
}
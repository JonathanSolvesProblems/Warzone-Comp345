#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <memory>

using std::vector;
using std::ostream;
using std::cout;
// Card is an abstract class that will be the parent of all the card types.
class Card {
public:
	Card(); 
	virtual std::string play() = 0;
	virtual ~Card(); 
	Card(const Card& cardCopy);
};

// Card Type Spy
class Spy : public Card { 
public:
	Spy();
	virtual std::string play() override final;
	virtual ~Spy();
};

// Card Type Bomb
class Bomb : public Card { 
public:
	Bomb(); 
	virtual std::string play() override final; 
	virtual ~Bomb();
};

 // Card Type Reinforcement
class Reinforcement : public Card {
public:
	Reinforcement();
	virtual std::string play() override final;
	virtual ~Reinforcement();
};

// Card Type Blockage
class Blockage : public Card { 
public:
	Blockage(); 
	virtual std::string play() override final; 
	virtual ~Blockage(); 

};

// Card Type Airlift
class Airlift : public Card { 
public:
	Airlift(); 
	virtual std::string play() override final; 
	virtual ~Airlift();
};

// Card Type Diplomacy
class Diplomacy : public Card { 
public:
	Diplomacy();
	virtual std::string play() override final;
	virtual ~Diplomacy();
};

// Hand, cards that are drawn by the player will be stored here.
class Hand {

	vector<Card*> cards;
public:
	// default constructor
	Hand(); 
	// destructor
	~Hand();

	// equals operator.
	Hand& operator=(const Hand& hand);

	// adds card to deck.
	void add(Card* drawn);
	Card* playCard();
	int size();
	void show(); // displays contents of hand

	friend ostream& operator<<(ostream& os, const Hand& hand);
};

// The Deck class stores the contents of the cards within a deck
class Deck {
private:
	vector<Card*> _deck;
	Deck(); 
public:
	static std::shared_ptr<Deck> instance();
	Deck (const Deck  &) = delete;
	~Deck(); 
	// Plays card
	void cardPlay(); 
	// draws a card. Meant to be passed into the deck.
	Card* draw(); 
	// shuffles the deck
	void shuffle(vector<Card*>& deck);
	// overloading stream operator
	friend ostream& operator<<(ostream& os, const Deck& deck);
};
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
	virtual std::string getType() = 0;
};

// Card Type Spy
class Spy : public Card { 
public:
	Spy();
	virtual std::string play() override final;
	virtual ~Spy();
	virtual std::string getType() { return "spy"; };
};

// Card Type Bomb
class Bomb : public Card { 
public:
	Bomb(); 
	virtual std::string play() override final; 
	virtual ~Bomb();
	virtual std::string getType() { return "bomb"; };
};

 // Card Type Reinforcement
class Reinforcement : public Card {
public:
	Reinforcement();
	virtual std::string play() override final;
	virtual ~Reinforcement();

	virtual std::string getType() { return "deploy"; };
};

// Card Type Blockage
class Blockage : public Card { 
public:
	Blockage(); 
	virtual std::string play() override final; 
	virtual ~Blockage();

	virtual std::string getType() { return "blockade"; };
};

// Card Type Airlift
class Airlift : public Card { 
public:
	Airlift(); 
	virtual std::string play() override final; 
	virtual ~Airlift();

	virtual std::string getType() { return "airlift"; };
};

// Card Type Diplomacy
class Diplomacy : public Card { 
public:
	Diplomacy();
	virtual std::string play() override final;
	virtual ~Diplomacy();

	virtual std::string getType() { return "negotiate"; };
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

	Card* removeCard(std::string findCard);

	// adds card to deck.
	void add(Card* drawn);
	Card* playCard();
	int size();
	void show(); // displays contents of hand

	int countCardsOfType(std::string t);

	friend ostream& operator<<(ostream& os, const Hand& hand);
};

// The Deck class stores the contents of the cards within a deck
class Deck {
private:
	vector<Card*> _deck;
	Deck(); 
	void shuffle(vector<Card*>& deck);
public:
	static std::shared_ptr<Deck> instance();
	Deck (const Deck  &) = delete;
	~Deck(); 
	// Plays card
	void cardPlay(); 
	// draws a card. Meant to be passed into the deck.
	Card* draw(); 
	void add(Card* card);
	// shuffles the deck
	// overloading stream operator
	friend ostream& operator<<(ostream& os, const Deck& deck);
};
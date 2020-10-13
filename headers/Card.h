#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

// Card is an abstract class that will be the parent of all the card types.
class Card {
public:
	Card(); 
	virtual void play() = 0;
	virtual ~Card(); 
	Card(const Card& cardCopy);
};

// Card Type Spy
class Spy : public Card { 
public:
	Spy();
	virtual void play() override final;
	virtual ~Spy();
};

// Card Type Bomb
class Bomb : public Card { 
public:
	Bomb(); 
	virtual void play() override final; 
	virtual ~Bomb();
};

 // Card Type Reinforcement
class Reinforcement : public Card {
public:
	Reinforcement();
	virtual void play() override final;
	virtual ~Reinforcement();
};

// Card Type Blockage
class Blockage : public Card { 
public:
	Blockage(); 
	virtual void play() override final; 
	virtual ~Blockage(); 

};

// Card Type Airlift
class Airlift : public Card { 
public:
	Airlift(); 
	virtual void play() override final; 
	virtual ~Airlift();
};

// Card Type Diplomacy
class Diplomacy : public Card { 
public:
	Diplomacy();
	virtual void play() override final;
	virtual ~Diplomacy();
};

// Hand, cards that are drawn by the player will be stored here.
class Hand {

	vector<Card*> _hand;
public:

	Hand(); 
	~Hand();

	Hand& operator=(const Hand& hand);

	void addHand(Card* drawn); 
	void showHand(); // displays contents of hand

	friend ostream& operator<<(ostream& os, const Hand& hand);
};

// The Deck class stores the contents of the cards within a deck
class Deck {
private:
	vector<Card*> _deck;
public:
	Deck(); 
	~Deck(); 
	void cardPlay(); 
	Card* draw(); 
	void shuffle(vector<Card*>& deck);
	friend ostream& operator<<(ostream& os, const Deck& deck);
};
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>

using std::string;
using std::ostream;
using std::vector;
using std::cout;
using std::endl;


class Card { // abstract class
private:
public:
	Card();
	Card(const Card& cardCopy);
	friend ostream& operator<<(ostream& out, const Card& card);
	virtual void play() = 0;
	virtual ~Card();
	const Card& operator=(const Card& cards);
	ostream& operator<<(ostream& os);
};

class Spy : public Card {
public:
	Spy();
	virtual void play() override final;
	virtual ~Spy();
};

class Bomb : public Card {
public:
	Bomb();
	virtual void play() override final;
	virtual ~Bomb();
};

class Reinforcement : public Card {
public:
	Reinforcement();
	virtual void play() override final;
	virtual ~Reinforcement();
};

class Blockage : public Card {
public:
	Blockage();
	virtual void play() override final;
	virtual ~Blockage();
};

class Airlift : public Card {
public:
	Airlift();
	virtual void play() override final;
	virtual ~Airlift();
};

class Diplomacy : public Card {
public:
	Diplomacy();
	virtual void play() override final;
	virtual ~Diplomacy();
};

class Hand final {

	vector<Card*> _hand;
public:

	Hand();
	~Hand();

	void addHand(Card* drawn);
	void showHand();

	Hand(const Hand& orderToCopy);
	friend ostream& operator<<(ostream& out, const Hand& o);
	Hand& operator=(const Hand& o);
};

class Deck final {
private:
	vector<Card*> _deck;

public:
	Deck();
	~Deck();
	void cardPlay();
	Card* draw();
	void shuffle(vector<Card*>& deck);
};
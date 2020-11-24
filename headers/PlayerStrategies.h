#pragma once
#include <vector>
#include "Map.h"
#include "Orders.h"

using std::vector;

// TODO
class Strategy {
public:
    virtual Order* issueOrder() = 0;
    virtual const vector<map::Territory*> toAttack() = 0;
    virtual const vector<map::Territory*> toDefend() = 0;
};

// (1) human player that requires user interaction to make decisions
class HumanPlayerStrategy : public Strategy {
public:
    Order* issueOrder();
    const vector<map::Territory*> toAttack();
    const vector<map::Territory*> toDefend();
};

/* an aggressive computer player that focuses
on attack (deploys or advances armies on its strongest country, then always advances to enemy territories until it
cannot do so anymore)
*/
class AggressivePlayerStrategy : public Strategy {
public:
    Order* issueOrder();
    const vector<map::Territory*> toAttack();
    const vector<map::Territory*> toDefend();
};

/*
 (3) a benevolent computer player that focuses on protecting its weak countries (deploys
or advances armies on its weakest countries, never advances to enemy territories),
*/
class BenevolentPlayerStrategy : public Strategy {
public:
    Order* issueOrder();
    const vector<map::Territory*> toAttack();
    const std::vector<map::Territory*> toDefend();

};

//  (4) a neutral player that never issues any order
class NeutralPlayerStrategy : public Strategy {
public:
    Order* issueOrder();
    const vector<map::Territory*> toAttack();
    const vector<map::Territory*> toDefend();
};

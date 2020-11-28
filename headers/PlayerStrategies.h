#pragma once

class PlayerStrategy;

#include <vector>
#include "GameModels.h"
#include "Orders.h"

#include "curses.h"

using std::vector;

// TODO
class PlayerStrategy {
public:
    virtual Order* issueOrder(GameModel* gm) = 0;
    virtual const vector<map::Territory *> toAttack(GameModel *gm) = 0;
    virtual const vector<map::Territory *> toDefend(GameModel *gm) = 0;
};

// (1) human player that requires user interaction to make decisions
class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy() {};
    ~HumanPlayerStrategy() {};
    virtual Order *issueOrder(GameModel *gm);
    virtual const vector<map::Territory *> toAttack(GameModel *gm);
    virtual const vector<map::Territory *> toDefend(GameModel *gm);
private:
    int choose_order_type(GameModel *gm);
    Order *deploy_controller(GameModel *gm);
    Order *advance_controller(GameModel *gm);
    Order *blockade_controller(GameModel *gm);
    Order *airlift_controller(GameModel *gm);
    Order *bomb_controller(GameModel *gm);
    Order *negotiate_controller(GameModel *gm);
};

/* an aggressive computer player that focuses
on attack (deploys or advances armies on its strongest country, then always advances to enemy territories until it
cannot do so anymore)
*/
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    AggressivePlayerStrategy() {};
    ~AggressivePlayerStrategy() {};
    virtual Order *issueOrder(GameModel *gm);
    virtual const vector<map::Territory *> toAttack(GameModel *gm);
    virtual const vector<map::Territory *> toDefend(GameModel *gm);
};

/*
 (3) a benevolent computer player that focuses on protecting its weak countries (deploys
or advances armies on its weakest countries, never advances to enemy territories),
*/
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    BenevolentPlayerStrategy() {};
    ~BenevolentPlayerStrategy() {};
    virtual Order *issueOrder(GameModel *gm);
    virtual const vector<map::Territory *> toAttack(GameModel *gm);
    virtual const std::vector<map::Territory *> toDefend(GameModel *gm);
};

//  (4) a neutral player that never issues any order
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    NeutralPlayerStrategy() {};
    ~NeutralPlayerStrategy() {};
    virtual Order *issueOrder(GameModel *gm);
    virtual const vector<map::Territory *> toAttack(GameModel *gm);
    virtual const vector<map::Territory *> toDefend(GameModel *gm);
};

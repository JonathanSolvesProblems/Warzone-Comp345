#pragma once

class PlayerStrategy;

#include <vector>
#include "GameModels.h"
#include "Orders.h"
#include "Application.h"

using std::vector;

// TODO
class PlayerStrategy {
public:
    virtual void beginRound(Player *player, GameModel *gm) = 0;
    virtual Order *issueOrder(Player *player, GameModel *gm) = 0;
    virtual const vector<map::Territory *> toAttack(Player *player,GameModel *gm) = 0;
    virtual const vector<map::Territory *> toDefend(Player *player, GameModel *gm) = 0;
};

// (1) human player that requires user interaction to make decisions
class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy() {};
    ~HumanPlayerStrategy() {};

    virtual void beginRound(Player *player, GameModel *gm);
    virtual void endRound(Player *player, GameModel *gm);
    virtual Order *issueOrder(Player *player, GameModel *gm);
    virtual const vector<map::Territory *> toAttack(Player *player, GameModel *gm);
    virtual const vector<map::Territory *> toDefend(Player *player, GameModel *gm);

private:
    vector<map::Territory*> territories_to_defend;
    int index_of_next_territory_to_defend = 0;
    int choose_order_type(GameModel *gm );
    Order *deploy_controller(Player *player, GameModel *gm);
    Order *advance_controller(Player *player, GameModel *gm);
    Order *blockade_controller(Player *player, GameModel *gm);
    Order *airlift_controller(Player *player, GameModel *gm);
    Order *bomb_controller(Player *player, GameModel *gm);
    Order *negotiate_controller(Player *player, GameModel *gm);
};

/* an aggressive computer player that focuses
on attack (deploys or advances armies on its strongest country, then always advances to enemy territories until it
cannot do so anymore)
*/
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    AggressivePlayerStrategy() {};
    ~AggressivePlayerStrategy() {};
    virtual Order *issueOrder(Player *player, GameModel *gm);
    virtual const vector<map::Territory *> toAttack(Player *player, GameModel *gm);
    virtual const vector<map::Territory *> toDefend(Player *player, GameModel *gm);
private:
    std::vector<map::Territory *> sortTerritoryList(std::vector<map::Territory *> toSort);
};

/*
 (3) a benevolent computer player that focuses on protecting its weak countries (deploys
or advances armies on its weakest countries, never advances to enemy territories),
*/
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    BenevolentPlayerStrategy() {};
    ~BenevolentPlayerStrategy() {};
    virtual Order *issueOrder(Player *player, GameModel *gm);
    virtual void beginRound(Player *player, GameModel *gm);
    virtual const vector<map::Territory *> toAttack(Player *player, GameModel *gm);
    virtual const std::vector<map::Territory *> toDefend(Player *player, GameModel *gm);
private:
    vector<int> numberArmiesToDeploy;
    vector<int> indicesToDeployAt;
    vector<int> indicesToAdvanceTo;
    vector<map::Territory*> playersTerritoriesSorted;
    int current_player_armies = 0;
    Order* issueBenevolentDeploy(Player *player);
    Order* issueBenevolentAdvance(Player *player);
    std::vector<map::Territory *> sortTerritoryList(std::vector<map::Territory *> toSort);
    vector<map::Territory*> ownedNeighboursOfGivenTerritory(Player *player,map::Territory highestOccupied);
};

//  (4) a neutral player that never issues any order
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    NeutralPlayerStrategy() {};
    ~NeutralPlayerStrategy() {};
    virtual Order *issueOrder(Player *player, GameModel *gm);
    virtual void beginRound(Player *player, GameModel *gm);
    virtual const vector<map::Territory *> toAttack(Player *player, GameModel *gm);
    virtual const vector<map::Territory *> toDefend(Player *player, GameModel *gm);
};

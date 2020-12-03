#pragma once

class PlayerStrategy;

#include <vector>
#include "GameModels.h"
#include "Orders.h"
#include "Application.h"

using std::vector;


class PlayerStrategy {
public:
    // begins the round
    virtual void beginRound(Player *player, GameModel *gm) = 0;
    // issueOrder depending on one called.
    virtual Order *issueOrder(Player *player, GameModel *gm) = 0;
    // sets up sorted by max.
    virtual const vector<map::Territory *> toAttack(Player *player,GameModel *gm) = 0;
    // sets up sorted by lowest.
    virtual const vector<map::Territory *> toDefend(Player *player, GameModel *gm) = 0;
};

// (1) human player that requires user interaction to make decisions
class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy();
    ~HumanPlayerStrategy();

    // begins round.
    virtual void beginRound(Player *player, GameModel *gm);
    // ends round.
    virtual void endRound(Player *player, GameModel *gm);
    // issueOrder depending on one called.
    virtual Order *issueOrder(Player *player, GameModel *gm);
    // sets up sorted by max.
    virtual const vector<map::Territory *> toAttack(Player *player, GameModel *gm);
    // sets up sorted by lowest.
    virtual const vector<map::Territory *> toDefend(Player *player, GameModel *gm);

private:
    // stores territories to defend.
    vector<map::Territory*> territories_to_defend;
    // variable for tracking the index of the territory.
    int index_of_next_territory_to_defend = 0;
    // method to choose order type.
    int choose_order_type(GameModel *gm );
    
    // controllers for orders (mvc architecture)
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
    AggressivePlayerStrategy();
    ~AggressivePlayerStrategy();
    // begins the round
    virtual void beginRound(Player *player, GameModel *gm);
    // issueOrder depending on one called.
    virtual Order *issueOrder(Player *player, GameModel *gm);
    // sets up sorted by max.
    virtual const vector<map::Territory *> toAttack(Player *player, GameModel *gm);
    // sets up sorted by lowest.
    virtual const vector<map::Territory *> toDefend(Player *player, GameModel *gm);
private:
    vector<map::Territory*> findEnemies(Player *player, map::Territory *origin, vector<map::Territory*> known, vector<map::Territory*> path);
    bool contains(vector<map::Territory*> &list, map::Territory *subject);
    std::vector<map::Territory *> sortTerritoryList(std::vector<map::Territory *> toSort);
    vector<map::Territory*> playersTerritoriesSorted;
    Order* issueAggressiveDeploy(Player *player);
    Order* issueAggressiveAdvance(Player *player,GameModel *gm);
    Order* issueAggressiveAirlift(Player *player);
    // vector for neighborsToAttack
    vector<map::Territory*> neighborsToAttack;
    // vector for pathToEnemy
    vector<map::Territory*> pathToEnemy;
    // vector for backupTargetFriendlies
    map::Territory *backupTarget{nullptr};
    // vector for backupTargetFriendlies
    vector<map::Territory *> backupTargetFriendlies;
    // variable for current_player_armies
    int current_player_armies = 0;
    // armiesToAdvance variable for tracking
    int armiesToAdvance = 0;
    // variable for tracking advancedToFriendly.
    bool advancedToFriendly = false;
};

/*
 (3) a benevolent computer player that focuses on protecting its weak countries (deploys
or advances armies on its weakest countries, never advances to enemy territories),
*/
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    BenevolentPlayerStrategy();
    ~BenevolentPlayerStrategy();
    // issueOrder depending on one called.
    virtual Order *issueOrder(Player *player, GameModel *gm);
    // begins round.
    virtual void beginRound(Player *player, GameModel *gm);
    // sets up sorted by max.
    virtual const vector<map::Territory *> toAttack(Player *player, GameModel *gm);
    // sets up sorted by lowest.
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
    // issueOrder depending on one called.
    virtual Order *issueOrder(Player *player, GameModel *gm);
    // begins round.
    virtual void beginRound(Player *player, GameModel *gm);
    // sets up sorted by max.
    virtual const vector<map::Territory *> toAttack(Player *player, GameModel *gm);
    // sets up sorted by lowest.
    virtual const vector<map::Territory *> toDefend(Player *player, GameModel *gm);
};

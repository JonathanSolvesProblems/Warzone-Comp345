#include "PlayerStrategies.h"

// TODO

Order *HumanPlayerStrategy::issueOrder(Player *player, GameModel *gm)
{
  gm->current_step->set(0);

  nodelay(stdscr, FALSE);

  Order* order = nullptr;

  OrderType order_type = (OrderType) choose_order_type(gm);
  if (order_type != PASS) {
    gm->current_order_type->set(order_type);
    gm->current_step->set(1);

    if (order_type == DEPLOY) {
      order = deploy_controller(gm);
    } else if (order_type == ADVANCE) {
      order = advance_controller(gm);
    } else if (order_type == BLOCKADE) {
      order = blockade_controller(gm);
    } else if (order_type == AIRLIFT) {
      order = airlift_controller(gm);
    } else if (order_type == BOMB) {
      order = bomb_controller(gm);
    } else if (order_type == NEGOTIATE) {
      order = negotiate_controller(gm);
    }
  }

  nodelay(stdscr, TRUE);

  return order;
};

int HumanPlayerStrategy::choose_order_type(GameModel* gm) {
  int k;

  int must_deploy = gm->current_player->get()->getArmees();
  int blockade_cards = gm->current_player->get()->countCardsOfType("blockade");
  int airlift_cards = gm->current_player->get()->countCardsOfType("airlift");
  int bomb_cards = gm->current_player->get()->countCardsOfType("bomb");
  int negotiate_cards = gm->current_player->get()->countCardsOfType("negotiate");

  while(k = getch()) {
    gm->error_message->set("");
    if (k == ' ') {
      return PASS;
    } else if (k == 'd' && must_deploy) {
      return DEPLOY;
    } else if (k == 'a' && !must_deploy) {
      return ADVANCE;
    } else if (k == 's' && !must_deploy && blockade_cards > 0) {
      return BLOCKADE;
    } else if (k == 'f' && !must_deploy && airlift_cards > 0) {
      return AIRLIFT;
    } else if (k == 'b' && !must_deploy && bomb_cards > 0) {
      return BOMB;
    } else if (k == 'n' && !must_deploy && negotiate_cards > 0) {
      return NEGOTIATE;
    }
    
    gm->error_message->set("Invalid selection");
  }
}

Order *HumanPlayerStrategy::deploy_controller(GameModel *gm){};
Order *HumanPlayerStrategy::advance_controller(GameModel *gm){};
Order *HumanPlayerStrategy::blockade_controller(GameModel *gm){};
Order *HumanPlayerStrategy::airlift_controller(GameModel *gm){};
Order *HumanPlayerStrategy::bomb_controller(GameModel *gm){};
Order *HumanPlayerStrategy::negotiate_controller(GameModel *gm){};

const vector<map::Territory *> HumanPlayerStrategy::toAttack(Player *player, GameModel *gm){};
const std::vector<map::Territory *> HumanPlayerStrategy::toDefend(Player *player, GameModel *gm)
{
  bool finished = false;
  while (!finished) {
    continue;
  }
};

Order *AggressivePlayerStrategy::issueOrder(Player *player, GameModel *gm) { return nullptr; };
const vector<map::Territory *> AggressivePlayerStrategy::toAttack(Player *player, GameModel *gm){};
const std::vector<map::Territory *> AggressivePlayerStrategy::toDefend(Player *player, GameModel *gm){};

void BenevolentPlayerStrategy::beginRound(Player *player, GameModel *gm){
    playersTerritoriesSorted = toDefend(player,gm);
    current_player_armies = 0;
}

Order *BenevolentPlayerStrategy::issueOrder(Player *player, GameModel *gm) { 

    if(current_player_armies > 0) {
      return issueBenevolentDeploy(player);
    } else {
      return issueBenevolentAdvance(player);
    }
    return nullptr;
};

Order* BenevolentPlayerStrategy::issueBenevolentDeploy(Player *player){

    int armies_to_deploy = numberArmiesToDeploy.at(0);
    int indexToDeploy = indicesToDeployAt.at(0);

    numberArmiesToDeploy.erase(numberArmiesToDeploy.begin());
    indicesToDeployAt.erase(indicesToDeployAt.begin());

    current_player_armies += armies_to_deploy;

    if(current_player_armies < player->getArmees()) {
       return new DeployOrder(*(player),*(playersTerritoriesSorted.at(indexToDeploy)),armies_to_deploy - 0);
    }
    //Else, deploy rest of armies
    current_player_armies = 0;
    return new DeployOrder(*(player),*(playersTerritoriesSorted.at(indexToDeploy)),current_player_armies - player->getArmees());
}


Order* BenevolentPlayerStrategy::issueBenevolentAdvance(Player *player) {
        //Get highest occupied territory
    map::Territory highestOccupied = *(playersTerritoriesSorted.at(playersTerritoriesSorted.size() - 1));

    //Get its number of troops
    int highestOccupiedNumber = highestOccupied.getArmees();

    vector<map::Territory*> ownedNeighbours = ownedNeighboursOfGivenTerritory(player,highestOccupied);

    //Find smallest and second smallest neighbour
    ownedNeighbours = sortTerritoryList(ownedNeighbours);
    map::Territory lowestOccupiedNeighbour = *(ownedNeighbours.at(0));
    map::Territory secondLowestOccupiedNeighbour = *(ownedNeighbours.at(1));

    int lowestOccupiedNumber = lowestOccupiedNeighbour.getArmees();
    int secondLowestOccupiedNumber = secondLowestOccupiedNeighbour.getArmees();
    
    //Advance troops from highest occupied territory to lowest occupied one
    return new AdvanceOrder(*(player),highestOccupied,lowestOccupiedNeighbour,lowestOccupiedNumber - secondLowestOccupiedNumber);
}

//Finds all neighbours of a territory that are also owned by the player
vector<map::Territory*> BenevolentPlayerStrategy::ownedNeighboursOfGivenTerritory(Player *player,map::Territory highestOccupied) {

  vector<map::Territory*> allNeighbours = highestOccupied.getNeighbours();

  //Get owned neighbours from allNeighbours
  vector<map::Territory* > ownedNeighbours;
  for(int i = 0; i < allNeighbours.size(); i++){

    //Filter out territories not owned by the player
    if(!(player->isOwner(allNeighbours.at(i))) ){
      continue;
    }
    ownedNeighbours.push_back(allNeighbours.at(i));
  }
  return ownedNeighbours;
}

//Sorts given territories from least armies to most
std::vector<map::Territory *> BenevolentPlayerStrategy::sortTerritoryList(std::vector<map::Territory *> toSort){
    
    std::sort(toSort.begin(), toSort.end(),
    [](map::Territory * T1, map::Territory * T2)
    {
        return T1->getArmees() < T2->getArmees();
    });

    return toSort;
};

const vector<map::Territory *> BenevolentPlayerStrategy::toAttack(Player *player, GameModel *gm){
    return vector<map::Territory*>();
};

//Returns territories from least armies to most
const std::vector<map::Territory *> BenevolentPlayerStrategy::toDefend(Player *player, GameModel *gm){

    std::vector<map::Territory *> sortedTerritories = player->owned_territories;
    
    std::sort(sortedTerritories.begin(), sortedTerritories.end(),
    [](map::Territory * T1, map::Territory * T2)
    {
        return T1->getArmees() < T2->getArmees();
    });

  // potentially goes out of bounce.
      for(int i = 0 ; i < sortedTerritories.size(); i++){
      if(i == sortedTerritories.size())
        break;
      int initialArmies = sortedTerritories.at(i)->getArmees();
      int nextArmies = sortedTerritories.at(i + i)->getArmees();
      int totalToDistribute = nextArmies - initialArmies;
      for(int j = 0; j < i; j++){
        numberArmiesToDeploy.push_back(totalToDistribute);
      }
    }

    for(int i = 0; i < sortedTerritories.size(); i++) {
      for(int j = 0; j < i; j++){
        indicesToDeployAt.push_back(j);
      }
    }

    return sortedTerritories;
};

Order *NeutralPlayerStrategy::issueOrder(Player *player, GameModel *gm) { return nullptr; };
const vector<map::Territory *> NeutralPlayerStrategy::toAttack(Player *player, GameModel *gm){
  return std::vector<map::Territory *>();
};
const std::vector<map::Territory *> NeutralPlayerStrategy::toDefend(Player *player, GameModel *gm){
  return std::vector<map::Territory *>();
};
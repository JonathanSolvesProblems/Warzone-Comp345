#include "PlayerStrategies.h"

// TODO

void HumanPlayerStrategy::beginRound(Player *player, GameModel *gm) {
  territories_to_defend = toDefend(player, gm);
  index_of_next_territory_to_defend = 0;
}

void HumanPlayerStrategy::endRound(Player *player, GameModel *gm){
    territories_to_defend.clear();
    index_of_next_territory_to_defend = -1;
}

Order *HumanPlayerStrategy::issueOrder(Player *player, GameModel *gm)
{
  Order* order = nullptr;

  OrderType order_type = (OrderType) choose_order_type(gm);

  if (order_type != PASS) {
    gm->current_order_type->set(order_type);

    if (order_type == DEPLOY) {
      order = deploy_controller(player, gm);
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

  if (order == nullptr) {
    endRound(player, gm);
  }

  gm->current_step->set(-1);
  return order;
};

int HumanPlayerStrategy::choose_order_type(GameModel* gm) {
  int k;

  int must_deploy = index_of_next_territory_to_defend < territories_to_defend.size();

  int blockade_cards = gm->current_player->get()->countCardsOfType("blockade");
  int airlift_cards = gm->current_player->get()->countCardsOfType("airlift");
  int bomb_cards = gm->current_player->get()->countCardsOfType("bomb");
  int negotiate_cards = gm->current_player->get()->countCardsOfType("negotiate");

  if (must_deploy) return DEPLOY;

  gm->current_step->set(0);

  do {
    k = Application::instance()->get_key(true);
    gm->error_message->set("");
    if (k == ' ') {
      return PASS;
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
  } while (true);
}

Order *HumanPlayerStrategy::deploy_controller(Player *player, GameModel *gm){
  if (index_of_next_territory_to_defend == -1) {

    gm->current_step->set(-1);
  }

  if (index_of_next_territory_to_defend < territories_to_defend.size())
  {
    map::Territory *next_territory = territories_to_defend.at(index_of_next_territory_to_defend++);
    return new DeployOrder(*player, *next_territory, 1);
  }
};

Order *HumanPlayerStrategy::advance_controller(GameModel *gm){};
Order *HumanPlayerStrategy::blockade_controller(GameModel *gm){};
Order *HumanPlayerStrategy::airlift_controller(GameModel *gm){};
Order *HumanPlayerStrategy::bomb_controller(GameModel *gm){};
Order *HumanPlayerStrategy::negotiate_controller(GameModel *gm){};

const vector<map::Territory *> HumanPlayerStrategy::toAttack(Player *player, GameModel *gm){};
const std::vector<map::Territory *> HumanPlayerStrategy::toDefend(Player *player, GameModel *gm)
{
  int key;
  int remaining = gm->current_player->get()->getArmees();


  for (map::Territory *territory : player->owned_territories)
  {
    gm->territory_list_items->push_back(
      new ConcreteObservable<std::pair<map::Territory*, int>>(std::make_pair(territory, 0)));
  }
  const std::vector<ConcreteObservable<std::pair<map::Territory *, int>>*> owned_territories_with_numbers = gm->territory_list_items->get();
  gm->current_step->set(1);
  gm->current_order_type->set(DEPLOY);

  while ((key = Application::instance()->get_key(true)) != ' ' || remaining > 0)
  {
    gm->error_message->set("");
    if (key == ' ') {
      gm->error_message->set("You must deploy all of your armies!");
      continue;
    }
    int current_index = gm->selected_index->get();
    if (key == KEY_DOWN) {
      if (current_index < owned_territories_with_numbers.size() - 1)
        gm->selected_index->set(++current_index);
    } else if (key == KEY_UP) {
      if (current_index > 0)
        gm->selected_index->set(--current_index);
    } else if (key == KEY_LEFT) {
      auto obs = owned_territories_with_numbers.at(current_index);
      auto pair = obs->get();
      if (pair.second > 0) {
        obs->set(std::make_pair(pair.first, pair.second - 1));
        remaining += 1;
      }
    } else if (key == KEY_RIGHT) {
      auto obs = owned_territories_with_numbers.at(current_index);
      auto pair = obs->get();
      if (remaining > 0)
      {
        obs->set(std::make_pair(pair.first, pair.second + 1));
        remaining -= 1;
      }
    }
    continue;
  }

  std::vector<map::Territory*> result;
  
  for (auto obs : owned_territories_with_numbers) {
    std::pair<map::Territory *, int> pair = obs->get();
    for (int i = 0; i < pair.second; i++) {
      result.push_back(pair.first);
    }
  }

  return result;
};

Order *AggressivePlayerStrategy::issueOrder(Player *player, GameModel *gm) { return nullptr; };
const vector<map::Territory *> AggressivePlayerStrategy::toAttack(Player *player, GameModel *gm){};
const std::vector<map::Territory *> AggressivePlayerStrategy::toDefend(Player *player, GameModel *gm){};

void BenevolentPlayerStrategy::beginRound(Player *player, GameModel *gm){
    playersTerritoriesSorted = toDefend(player, gm);
    current_player_armies = player->getArmees();
    int averageArmies = 0;
    int totalTerritories = playersTerritoriesSorted.size() >= 3 ? 3 : playersTerritoriesSorted.size();
    // takes at most the first 3 territories with the lowest number of armies and sums up their armies
    for (int i = 0; i < totalTerritories; ++i) {
      averageArmies += playersTerritoriesSorted.at(i)->getArmees();
    }
    // Calculates average number of armies for those 3 territories
    averageArmies += player->getArmees();
    averageArmies /= totalTerritories;
    // Calculates number of armies to deploy for each of those 3 territories
    for (int i = 0; i < totalTerritories; ++i) {
      numberArmiesToDeploy.push_back(averageArmies - playersTerritoriesSorted.at(i)->getArmees());
      indicesToDeployAt.push_back(i);
      indicesToAdvanceTo.push_back(i);
    }
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

  int armies_to_deploy = 0;
  int indexToDeploy = 0;
  // If armies are left to deploy after the lists are exhausted, deploy to the territory with the smallest number of countries
  if (numberArmiesToDeploy.size() == 0 || indicesToDeployAt.size() == 0) {
    armies_to_deploy = current_player_armies;
    indexToDeploy = 0;
  }
  // Else continue like normal
  else {
    armies_to_deploy = numberArmiesToDeploy.at(0);
    indexToDeploy = indicesToDeployAt.at(0);
    numberArmiesToDeploy.erase(numberArmiesToDeploy.begin());
    indicesToDeployAt.erase(indicesToDeployAt.begin());
  }

  current_player_armies -= armies_to_deploy;

  return new DeployOrder(*(player),*(playersTerritoriesSorted.at(indexToDeploy)), armies_to_deploy);
}


Order* BenevolentPlayerStrategy::issueBenevolentAdvance(Player *player) {
  // cout << "BenevolentAdvance" << endl;
  // Return if no more territories need armies from neighbors
  if (indicesToAdvanceTo.size() == 0) {
    return nullptr;
  }

  int totalTerritories = playersTerritoriesSorted.size() >= 3 ? 3 : playersTerritoriesSorted.size();
  int advanceIndex = indicesToAdvanceTo.front();
  indicesToAdvanceTo.erase(indicesToAdvanceTo.begin());
  // balances the number armies with a friendly neighbor
  // cout << "BenevolentAdvance::Friendlies" << endl;
  vector<map::Territory *> neighbors;
  neighbors = playersTerritoriesSorted.at(advanceIndex)->getNeighbours();
  map::Territory *neighbor{nullptr};

  // cout << "Neighbors size" << endl;
  // cout << neighbors.size() << endl;
  for (int i = 0; i < neighbors.size(); ++i) {
    // cout << "Neighbors index" << endl;
    neighbor = neighbors.at(i);
    // cout << *neighbor << endl;
    if (neighbor->getOwner() == player) {
      // cout << "BenevolentAdvance::Balancing" << endl;
      int averageArmies = (playersTerritoriesSorted.at(advanceIndex)->getArmees() + neighbor->getArmees()) / 2;
      if (neighbor->getArmees() > averageArmies) {
        // cout << "BenevolentAdvance::BalancingOneWay" << endl;
        return new AdvanceOrder(*(player), *(neighbor), *(playersTerritoriesSorted.at(advanceIndex)), (neighbor->getArmees() - averageArmies) / 2);
      }
      else if (neighbor->getArmees() < averageArmies) {
        // cout << "BenevolentAdvance::BalancingOtherWay" << endl;
        return new AdvanceOrder(*(player), *(playersTerritoriesSorted.at(advanceIndex)), *(neighbor), (playersTerritoriesSorted.at(advanceIndex)->getArmees() - averageArmies) / 2);
      }
      else {
        return nullptr;
      }
    }
  }
  return nullptr;
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

std::vector<map::Territory *> AggressivePlayerStrategy::sortTerritoryList(std::vector<map::Territory *> toSort){
    
    std::sort(toSort.begin(), toSort.end(),
    [](map::Territory * T1, map::Territory * T2)
    {
        return T1->getArmees() > T2->getArmees();
    });

    return toSort;
};

const vector<map::Territory *> BenevolentPlayerStrategy::toAttack(Player *player, GameModel *gm){
    return vector<map::Territory*>();
};

// Then check neighbors and if there is a friendly, balance out the armies between the two with Advance.
const std::vector<map::Territory *> BenevolentPlayerStrategy::toDefend(Player *player, GameModel *gm){

    // Returns territories sorted from least armies to most
    return sortTerritoryList(player->owned_territories);
};

Order *NeutralPlayerStrategy::issueOrder(Player *player, GameModel *gm) { return nullptr; };
void NeutralPlayerStrategy::beginRound(Player *player, GameModel *gm){};
const vector<map::Territory *> NeutralPlayerStrategy::toAttack(Player *player, GameModel *gm){
  return std::vector<map::Territory *>();
};
const std::vector<map::Territory *> NeutralPlayerStrategy::toDefend(Player *player, GameModel *gm){
  return std::vector<map::Territory *>();
};
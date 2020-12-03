#include "PlayerStrategies.h"

// TODO

Order *HumanPlayerStrategy::issueOrder(Player *player, GameModel *gm)
{
  gm->current_step->set(0);

  nodelay(stdscr, FALSE);

  Order *order = nullptr;

  OrderType order_type = (OrderType)choose_order_type(gm);
  if (order_type != PASS)
  {
    gm->current_order_type->set(order_type);
    gm->current_step->set(1);

    if (order_type == DEPLOY)
    {
      order = deploy_controller(gm);
    }
    else if (order_type == ADVANCE)
    {
      order = advance_controller(gm);
    }
    else if (order_type == BLOCKADE)
    {
      order = blockade_controller(gm);
    }
    else if (order_type == AIRLIFT)
    {
      order = airlift_controller(gm);
    }
    else if (order_type == BOMB)
    {
      order = bomb_controller(gm);
    }
    else if (order_type == NEGOTIATE)
    {
      order = negotiate_controller(gm);
    }
  }

  nodelay(stdscr, TRUE);

  return order;
};

int HumanPlayerStrategy::choose_order_type(GameModel *gm)
{
  int k;

  int must_deploy = gm->current_player->get()->getArmees();
  int blockade_cards = gm->current_player->get()->countCardsOfType("blockade");
  int airlift_cards = gm->current_player->get()->countCardsOfType("airlift");
  int bomb_cards = gm->current_player->get()->countCardsOfType("bomb");
  int negotiate_cards = gm->current_player->get()->countCardsOfType("negotiate");

  while (k = getch())
  {
    gm->error_message->set("");
    if (k == ' ')
    {
      return PASS;
    }
    else if (k == 'd' && must_deploy)
    {
      return DEPLOY;
    }
    else if (k == 'a' && !must_deploy)
    {
      return ADVANCE;
    }
    else if (k == 's' && !must_deploy && blockade_cards > 0)
    {
      return BLOCKADE;
    }
    else if (k == 'f' && !must_deploy && airlift_cards > 0)
    {
      return AIRLIFT;
    }
    else if (k == 'b' && !must_deploy && bomb_cards > 0)
    {
      return BOMB;
    }
    else if (k == 'n' && !must_deploy && negotiate_cards > 0)
    {
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
  while (!finished)
  {
    continue;
  }
};



Order *AggressivePlayerStrategy::issueOrder(Player *player, GameModel *gm)
{
  
  if (current_player_armies > 0)
  {
    return issueAggressiveDeploy(player);
  }
  else if(player->hand->removeCard("airlift") != nullptr)
  {
    return issueAggressiveAirlift(player);
  }
  else if (neighborsToAttack.size() > 0)
  {
    return issueAggressiveAdvance(player,gm);
  }
  // Final case is take an enemy territory, and attack it with all the neighbor/airlift/bomb friendlies that you have.
  else if (!advancedToFriendly)
  {
    advancedToFriendly = true;
    map::Territory* m = playersTerritoriesSorted.at(0)->getNeighbours().at(rand() % playersTerritoriesSorted.at(0)->getNeighbours().size());
    return new AdvanceOrder(*player, *(playersTerritoriesSorted.at(0)), *m, playersTerritoriesSorted.at(0)->getArmees());
  }

  // else if (pathToEnemy.size() > 0 && !advancedToFriendly)
  // {
  //   map::Territory *next = pathToEnemy.at(0);
  //   pathToEnemy.erase(pathToEnemy.begin());
  //   advancedToFriendly = true;
  //   return new AdvanceOrder(*player, *(playersTerritoriesSorted.at(0)), *next, playersTerritoriesSorted.at(0)->getArmees());
  // }
  // else if (pathToEnemy.size() == 0)
  // {
  //   pathToEnemy = findEnemies(player, playersTerritoriesSorted.at(0), vector<map::Territory*>(), vector<map::Territory*>());
  // }
  return nullptr;
};

Order *AggressivePlayerStrategy::issueAggressiveAirlift(Player *player)
{
  return new AirliftOrder(*player, *(playersTerritoriesSorted.at(0)), *backupTarget, playersTerritoriesSorted.at(0)->getArmees());
}

Order *AggressivePlayerStrategy::issueAggressiveAdvance(Player *player, GameModel* gm)
{
  map::Territory *max = playersTerritoriesSorted.at(0);
  map::Territory *neighbor = neighborsToAttack.at(0);
  neighborsToAttack.erase(neighborsToAttack.begin());

  int enemyArmies = neighbor->getArmees();

  if (neighbor->getArmees() == 0)
  {
    gm->log->append("ARMIES IS ZERO--------------------------------------------------------------------------------");
    //armiesToAdvance -= 1;
    gm->log->append("NEIGHBOR: " +  neighbor->getName() + std::to_string(neighbor->getArmees()) +  "----------------------------------------------------------------------" );
    gm->log->append("MAX: " +  max->getName() + "    " + std::to_string(max->getArmees()) + "----------------------------------------------------------------------" );
    return new AdvanceOrder(*(player), *max, *neighbor, 1);
  }

  else if ((enemyArmies * 2) < max->getArmees())
  {
    gm->log->append("NOT ZERO ARMIES--------------------------------------------------------------------------------");
    gm->log->append("NEIGHBOR: " +  neighbor->getName() + "----------------------------------------------------------------------" );
    gm->log->append("MAX: " + max->getName() + "----------------------------------------------------------------------" );
    armiesToAdvance -= neighbor->getArmees() * 2;
    return new AdvanceOrder(*(player), *max, *neighbor, neighbor->getArmees() * 2);
  }

  return nullptr;
}

const vector<map::Territory *> AggressivePlayerStrategy::toAttack(Player *player, GameModel *gm)
{
 unordered_set<map::Territory *> territories_to_attack = unordered_set<map::Territory *>();
  
  for (map::Territory *owned_territory : player->owned_territories)
  {
    // float test = 1.0f / owned_territory->getNeighbours().size();
    for (map::Territory *neighbour : owned_territory->getNeighbours())
    {
      if (territories_to_attack.find(neighbour) == territories_to_attack.end() && neighbour->getOwner() != player)
      {
        territories_to_attack.insert(neighbour);
      }
    }
  }
  
  vector<map::Territory *> result = vector<map::Territory *>();
  result.assign(territories_to_attack.begin(), territories_to_attack.end());

  return result;
};

const std::vector<map::Territory *> AggressivePlayerStrategy::toDefend(Player *player, GameModel *gm)
{
  return sortTerritoryList(player->owned_territories);
};

void AggressivePlayerStrategy::beginRound(Player *player, GameModel *gm)
{
  playersTerritoriesSorted = toDefend(player, gm);
  current_player_armies = player->getArmees();
  advancedToFriendly = false;
  backupTarget = toAttack(player, gm).at(rand() % toAttack(player, gm).size());
  for (auto neighbor : backupTarget->getNeighbours())
  {
    if (neighbor->getOwner() == player)
    {
      backupTargetFriendlies.push_back(neighbor);
    }
  }
  vector<map::Territory *> neighbors = playersTerritoriesSorted.at(0)->getNeighbours();
  for (auto neighbor : neighbors)
  {
    if (neighbor->getOwner() != player)
    {
      neighborsToAttack.push_back(neighbor);
    }
  }
}

Order *AggressivePlayerStrategy::issueAggressiveDeploy(Player *player)
{
  map::Territory *max = playersTerritoriesSorted.at(0);
  current_player_armies = 0;
  armiesToAdvance = max->getArmees() + player->getArmees();
  return new DeployOrder(*player, *max, player->getArmees());
}

void BenevolentPlayerStrategy::beginRound(Player *player, GameModel *gm)
{
  playersTerritoriesSorted = toDefend(player, gm);
  current_player_armies = player->getArmees();
  int averageArmies = 0;
  int totalTerritories = playersTerritoriesSorted.size() >= 3 ? 3 : playersTerritoriesSorted.size();
  // takes at most the first 3 territories with the lowest number of armies and sums up their armies
  for (int i = 0; i < totalTerritories; ++i)
  {
    averageArmies += playersTerritoriesSorted.at(i)->getArmees();
  }
  // Calculates average number of armies for those 3 territories
  averageArmies += player->getArmees();
  averageArmies /= totalTerritories;
  // Calculates number of armies to deploy for each of those 3 territories
  for (int i = 0; i < totalTerritories; ++i)
  {
    numberArmiesToDeploy.push_back(averageArmies - playersTerritoriesSorted.at(i)->getArmees());
    indicesToDeployAt.push_back(i);
    indicesToAdvanceTo.push_back(i);
  }
}

vector<map::Territory*> AggressivePlayerStrategy::findEnemies(Player *player, map::Territory *origin, vector<map::Territory*> known, vector<map::Territory*> path)
{
  for (map::Territory *neighbor : origin->getNeighbours())
  {
    if (!contains(known, neighbor))
    {
      known.push_back(neighbor);
    }
    if (neighbor->getOwner() != player)
    {
      // path.push_back(neighbor);
      return path;
    }
  }
  for (map::Territory *neighbor : origin->getNeighbours())
  {
    path.push_back(neighbor);
    return findEnemies(player, neighbor, known, path);
  }
}
bool AggressivePlayerStrategy::contains(vector<map::Territory*> &list, map::Territory *subject)
{
  for (map::Territory *territory : list)
  {
    if (territory == subject)
    {
      return true;
    }
  }
  return false;
}

Order *BenevolentPlayerStrategy::issueOrder(Player *player, GameModel *gm)
{

  if (current_player_armies > 0)
  {
    return issueBenevolentDeploy(player);
  }
  else
  {
    return issueBenevolentAdvance(player);
  }
  return nullptr;
};

Order *BenevolentPlayerStrategy::issueBenevolentDeploy(Player *player)
{

  int armies_to_deploy = 0;
  int indexToDeploy = 0;
  // If armies are left to deploy after the lists are exhausted, deploy to the territory with the smallest number of countries
  if (numberArmiesToDeploy.size() == 0 || indicesToDeployAt.size() == 0)
  {
    armies_to_deploy = current_player_armies;
    indexToDeploy = 0;
  }
  // Else continue like normal
  else
  {
    armies_to_deploy = numberArmiesToDeploy.at(0);
    indexToDeploy = indicesToDeployAt.at(0);
    numberArmiesToDeploy.erase(numberArmiesToDeploy.begin());
    indicesToDeployAt.erase(indicesToDeployAt.begin());
  }

  current_player_armies -= armies_to_deploy;

  return new DeployOrder(*(player), *(playersTerritoriesSorted.at(indexToDeploy)), armies_to_deploy);
}

Order *BenevolentPlayerStrategy::issueBenevolentAdvance(Player *player)
{
  // Return if no more territories need armies from neighbors
  if (indicesToAdvanceTo.size() == 0)
  {
    return nullptr;
  }

  int totalTerritories = playersTerritoriesSorted.size() >= 3 ? 3 : playersTerritoriesSorted.size(); 
  int advanceIndex = indicesToAdvanceTo.front();
  indicesToAdvanceTo.erase(indicesToAdvanceTo.begin());
  // balances the number armies with a friendly neighbor
  vector<map::Territory *> neighbors;
  neighbors = playersTerritoriesSorted.at(advanceIndex)->getNeighbours();
  map::Territory *neighbor{nullptr};

  for (int i = 0; i < neighbors.size(); ++i)
  {
    neighbor = neighbors.at(i);
    if (neighbor->getOwner() == player)
    {
      int averageArmies = (playersTerritoriesSorted.at(advanceIndex)->getArmees() + neighbor->getArmees()) / 2;
      if (neighbor->getArmees() > averageArmies)
      {
        return new AdvanceOrder(*(player), *(neighbor), *(playersTerritoriesSorted.at(advanceIndex)), (neighbor->getArmees() - averageArmies) / 2);
      }
      else if (neighbor->getArmees() < averageArmies)
      {
        return new AdvanceOrder(*(player), *(playersTerritoriesSorted.at(advanceIndex)), *(neighbor), (playersTerritoriesSorted.at(advanceIndex)->getArmees() - averageArmies) / 2);
      }
      else
      {
        return nullptr;
      }
    }
  }
  return nullptr;
}

//Finds all neighbours of a territory that are also owned by the player
vector<map::Territory *> BenevolentPlayerStrategy::ownedNeighboursOfGivenTerritory(Player *player, map::Territory highestOccupied)
{

  vector<map::Territory *> allNeighbours = highestOccupied.getNeighbours();

  //Get owned neighbours from allNeighbours
  vector<map::Territory *> ownedNeighbours;
  for (int i = 0; i < allNeighbours.size(); i++)
  {

    //Filter out territories not owned by the player
    if (!(player->isOwner(allNeighbours.at(i))))
    {
      continue;
    }
    ownedNeighbours.push_back(allNeighbours.at(i));
  }
  return ownedNeighbours;
}

//Sorts given territories from least armies to most
std::vector<map::Territory *> BenevolentPlayerStrategy::sortTerritoryList(std::vector<map::Territory *> toSort)
{

  std::sort(toSort.begin(), toSort.end(),
            [](map::Territory *T1, map::Territory *T2) {
              return T1->getArmees() < T2->getArmees();
            });

  return toSort;
};

std::vector<map::Territory *> AggressivePlayerStrategy::sortTerritoryList(std::vector<map::Territory *> toSort)
{

  std::sort(toSort.begin(), toSort.end(),
            [](map::Territory *T1, map::Territory *T2) {
              return T1->getArmees() > T2->getArmees();
            });

  return toSort;
};

const vector<map::Territory *> BenevolentPlayerStrategy::toAttack(Player *player, GameModel *gm)
{
  return vector<map::Territory *>();
};

// Then check neighbors and if there is a friendly, balance out the armies between the two with Advance.
const std::vector<map::Territory *> BenevolentPlayerStrategy::toDefend(Player *player, GameModel *gm)
{

  // Returns territories sorted from least armies to most
  return sortTerritoryList(player->owned_territories);
};

Order *NeutralPlayerStrategy::issueOrder(Player *player, GameModel *gm) { return nullptr; };
void NeutralPlayerStrategy::beginRound(Player *player, GameModel *gm){};
const vector<map::Territory *> NeutralPlayerStrategy::toAttack(Player *player, GameModel *gm)
{
  return std::vector<map::Territory *>();
};
const std::vector<map::Territory *> NeutralPlayerStrategy::toDefend(Player *player, GameModel *gm)
{
  return std::vector<map::Territory *>();
};
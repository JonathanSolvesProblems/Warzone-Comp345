#include "PlayerStrategies.h"

// TODO

Order* HumanPlayerStrategy::issueOrder(GameModel* gm) {
  gm->current_step->set(0);
  nodelay(stdscr, FALSE);

  while(getch() != 's');

  nodelay(stdscr, TRUE);

  return nullptr;
};
const vector<map::Territory *> HumanPlayerStrategy::toAttack(GameModel *gm){

};
const std::vector<map::Territory *> HumanPlayerStrategy::toDefend(GameModel *gm){

};

Order *AggressivePlayerStrategy::issueOrder(GameModel *gm){ return nullptr; };
const vector<map::Territory *> AggressivePlayerStrategy::toAttack(GameModel *gm){};
const std::vector<map::Territory *> AggressivePlayerStrategy::toDefend(GameModel *gm){};

Order *BenevolentPlayerStrategy::issueOrder(GameModel *gm){ return nullptr; };
const vector<map::Territory *> BenevolentPlayerStrategy::toAttack(GameModel *gm){};
const std::vector<map::Territory *> BenevolentPlayerStrategy::toDefend(GameModel *gm){};

Order *NeutralPlayerStrategy::issueOrder(GameModel *gm){ return nullptr; };
const vector<map::Territory *> NeutralPlayerStrategy::toAttack(GameModel *gm){};
const std::vector<map::Territory *> NeutralPlayerStrategy::toDefend(GameModel *gm){};

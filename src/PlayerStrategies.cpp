#include "PlayerStrategies.h"

// TODO

Order *HumanPlayerStrategy::issueOrder(Player *player, GameModel *gm)
{
  Order* order = nullptr;

  OrderType order_type = (OrderType) choose_order_type(gm);
  if (order_type != PASS && order_type != DEPLOY)
    gm->current_step->set(0);

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
    territories_to_defend.clear();
    index_of_next_territory_to_defend = -1;
  }

  gm->current_step->set(-1);
  return order;
};

int HumanPlayerStrategy::choose_order_type(GameModel* gm) {
  int k;

  int must_deploy = gm->current_player->get()->getArmees() - territories_to_defend.size();
  int blockade_cards = gm->current_player->get()->countCardsOfType("blockade");
  int airlift_cards = gm->current_player->get()->countCardsOfType("airlift");
  int bomb_cards = gm->current_player->get()->countCardsOfType("bomb");
  int negotiate_cards = gm->current_player->get()->countCardsOfType("negotiate");

  if (must_deploy) return DEPLOY;

  while(k = Application::instance()->get_key(true)) {
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

Order *HumanPlayerStrategy::deploy_controller(Player *player, GameModel *gm){
  if (index_of_next_territory_to_defend == -1) {
    territories_to_defend = toDefend(player, gm);
    index_of_next_territory_to_defend = 0;
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

  while ((key = Application::instance()->get_key(true)) != ' ')
  {
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

Order *BenevolentPlayerStrategy::issueOrder(Player *player, GameModel *gm) { return nullptr; };
const vector<map::Territory *> BenevolentPlayerStrategy::toAttack(Player *player, GameModel *gm){};
const std::vector<map::Territory *> BenevolentPlayerStrategy::toDefend(Player *player, GameModel *gm){};

Order *NeutralPlayerStrategy::issueOrder(Player *player, GameModel *gm) { return nullptr; };
const vector<map::Territory *> NeutralPlayerStrategy::toAttack(Player *player, GameModel *gm){};
const std::vector<map::Territory *> NeutralPlayerStrategy::toDefend(Player *player, GameModel *gm){};

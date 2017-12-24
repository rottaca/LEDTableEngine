#ifndef _H_GAME_OF_LIFE_APP_
#define _H_GAME_OF_LIFE_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>
#include <cstdlib>
#include <complex>

#include "../engine/baseApplication.hpp"

class GameOfLife : public BaseApplication {
protected:

  std::array<Image, 2> m_gameField;
  size_t   m_currentFieldIdx;
  TimeUnit m_lastUpdateTime;
  TimeUnit m_lastChangeTime;
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_dist;

  struct Rules {
    std::vector<uint8_t>neighborsForBirth;
    std::vector<uint8_t>neighborsForLiving;
    float               initialLivingCellRatio;

    Rules() {
      initialLivingCellRatio = 0.4;
      neighborsForBirth      = { 3 };
      neighborsForLiving     = { 2, 3 };
    }

    Rules(std::vector<uint8_t>neighborsForBirth,
          std::vector<uint8_t>neighborsForLiving,
          float               initRatio) {
      this->neighborsForBirth  = neighborsForBirth;
      this->neighborsForLiving = neighborsForLiving;
      initialLivingCellRatio   = initRatio;
    }
  };

  Rules m_evolutionRules;

public:

  GameOfLife();
  virtual ~GameOfLife();

  void initialize(BaseController *ctrl);

  void processInput(const BaseInput::InputEvents& events,
                    TimeUnit                      deltaTime);
  void draw(Image& frame);
  void continueApp();

private:

  uint8_t checkNeighbours(Image& gameField,
                          Pointi p);
  void    randomInitField();
};


#endif // ifndef _H_GAME_OF_LIFE_APP_

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

class GameOfLife: public BaseApplication {
protected:
  std::array<Image,2> m_gameField;
  size_t m_currentFieldIdx;
  TimeUnit m_lastUpdateTime;
  std::default_random_engine m_generator;
  std::uniform_real_distribution<float> m_dist;

public:
  GameOfLife();
  virtual ~GameOfLife ();

  void initialize(BaseController * ctrl);

  void processInput(const BaseInput::InputEvents &events,
                    const BaseInput::InputEvents &eventsDebounced,
                    TimeUnit deltaTime);
  void draw(Image &frame);
  void continueApp();

private:
  uint8_t checkNeighbours(Image& gameField, Pointi p);
  void randomInitField();

};


#endif

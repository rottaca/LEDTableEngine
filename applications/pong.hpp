#ifndef _H_PONG_APP_
#define _H_PONG_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>
#include <list>

#include "../engine/baseApplication.hpp"

class Pong : public BaseApplication {
protected:

  std::default_random_engine m_generator;
  std::uniform_real_distribution<float> m_randFloat;

  Pointf m_ballSpeed;
  Pointf m_ballPos;
  int    m_playerHeight;
  std::array<int, 2>  m_playerPos;
  std::array<int, 2>  m_playerScore;
  std::array<bool, 2> m_playerWonRound;

  float m_speedIncrease;
  TimeUnit m_movementSpeed;

  TimeUnit m_lastUpdateTime;
  TimeUnit m_deathTime;
  bool     m_autoplay;

public:

  Pong(bool autoplay = true);
  virtual ~Pong();

  void initialize(BaseController *ctrl);

  void processInput(const BaseInput::InputEvents& events,
                    TimeUnit                      deltaTime);
  void draw(Image& frame);

private:

  void resetRound();
};


#endif // ifndef _H_PONG_APP_

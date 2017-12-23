#ifndef _H_SNAKE_APP_
#define _H_SNAKE_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>
#include <list>

#include "../engine/baseApplication.hpp"
#include "../engine/matrixScrollText.hpp"

class Snake : public BaseApplication {
protected:

  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_posDist;
  std::list<Pointi> m_snake;
  Pointi   m_moveDir;
  Pointi   m_foodPos;
  TimeUnit m_lastUpdateTime;

  Audio* m_soundCoin;
public:

  Snake();
  virtual ~Snake();

  void initialize(BaseController *ctrl);
  void deinitialize();

  void processInput(const BaseInput::InputEvents& events,
                    TimeUnit                      deltaTime);
  void draw(Image& frame);
};


#endif // ifndef _H_SNAKE_APP_

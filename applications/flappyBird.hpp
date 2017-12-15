#ifndef _H_FlappyBird_APP_
#define _H_FlappyBird_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>
#include <list>

#include "../engine/baseApplication.hpp"
#include "../engine/matrixScrollText.hpp"

class FlappyBird : public BaseApplication {
protected:

  struct Wall {
    ssize_t openingYMin;
    ssize_t openingYMax;
    float   posX;
  };

  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_posDist;

  float  m_birdY;
  size_t m_birdX;
  float  m_birdYSpeed;
  size_t m_score;
  std::list<Wall> m_walls;
  TimeUnit m_lastUpdateTime;

public:

  FlappyBird();
  virtual ~FlappyBird();

  void initialize(BaseController *ctrl);

  void processInput(const BaseInput::InputEvents& events,
                    TimeUnit                      deltaTime);
  void draw(Image& frame);
};


#endif // ifndef _H_FlappyBird_APP_

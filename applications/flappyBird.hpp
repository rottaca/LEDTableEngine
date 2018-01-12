#ifndef _H_FlappyBird_APP_
#define _H_FlappyBird_APP_

#include "../engine/baseApplication.hpp"
#include "../engine/matrixScrollText.hpp"

class FlappyBird : public led::BaseApplication {
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
  led::TimeUnit m_lastUpdateTime;

public:

  FlappyBird();
  virtual ~FlappyBird();

  void initialize(led::BaseController *ctrl);

  void processInput(const led::BaseInput::InputEvents& events,
                    led::TimeUnit                      deltaTime);
  void draw(led::Image& frame);
};


#endif // ifndef _H_FlappyBird_APP_

#ifndef _H_TETRIS_APP_
#define _H_TETRIS_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>
#include <list>

#include "../engine/baseApplication.hpp"
#include "../engine/matrixScrollText.hpp"

class Tetris : public BaseApplication {
protected:

  typedef std::vector<Pointi>Shape;
  std::vector<Shape> m_shapes;

  struct ShapeState {
    bool    isAlive;
    Shape   shape;
    uint8_t colorIdx;
    ShapeState() {
      isAlive  = false;
      colorIdx = 0;
    }

    void update(Shape shape, uint8_t colIdx) {
      this->shape    = shape;
      this->colorIdx = colIdx;
      isAlive        = true;
    }

    void translate(int dx, int dy) {
      for (Pointi& p : shape) {
        p.x += dx;
        p.y += dy;
      }
    }

    void rotate90() {
      Pointf c;

      for (Pointi& p : shape) {
        c.x += p.x;
        c.y += p.y;
      }

      for (Pointi& p : shape) {
        p.x = (p.y - c.y) + c.x;
        p.y = (p.x - c.x) + c.y;
      }
    }
  };

  ShapeState m_fallingShape;
  Image m_gameField;
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_posDist;
  size_t m_score;
  TimeUnit m_lastUpdateTimeFall;
  TimeUnit m_lastUpdateTimeMove;

public:

  Tetris();
  virtual ~Tetris();

  void initialize(BaseController *ctrl);

  void processInput(const BaseInput::InputEvents& events,
                    TimeUnit                      deltaTime);
  void draw(Image& frame);
};


#endif // ifndef _H_SNAKE_APP_

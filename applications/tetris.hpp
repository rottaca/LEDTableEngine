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
  typedef std::vector<Shape> ShapeDef;
  std::vector<ShapeDef> m_shapes;

  struct ShapeState {
    Pointi   pos;
    Pointi   size;
    bool     isAlive;
    ShapeDef shape;
    size_t   rotIdx;
    uint8_t  colorIdx;
    ShapeState() {
      isAlive  = false;
      colorIdx = 0;
      rotIdx   = 0;
      pos.x    = 0;
      pos.y    = 0;
    }

    void update(ShapeDef shape, uint8_t colIdx) {
      this->shape    = shape;
      this->colorIdx = colIdx;
      isAlive        = true;
      rotIdx         = 0;
      pos.x          = 0;
      pos.y          = 0;
    }

    Shape& currShape() {
      return shape[rotIdx];
    }

    void translate(int dx, int dy) {
      pos.x += dx;
      pos.y += dy;
    }

    void rotate() {
      rotIdx = (rotIdx + 1) % shape.size();
    }

    bool isIntersecting(int px, int py) {
      for (Pointi& p : shape[rotIdx]) {
        if ((p.x + pos.x == px) && (p.y + pos.y == py)) {
          return true;
        }
      }
      return false;
    }
  };

  ShapeState m_fallingShape;
  Image m_gameField;
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_posDist;
  size_t   m_score;
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

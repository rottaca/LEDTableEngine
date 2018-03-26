#ifndef _H_TETRIS_APP_
#define _H_TETRIS_APP_


#include <LEDTableEngine/core/baseApplication.hpp>

class Tetris : public led::BaseApplication {
public:

    typedef std::vector<led::Pointi>Shape;
    typedef std::vector<Shape>      ShapeDef;

protected:

    led::Audio *m_soundClick;
    led::Audio *m_soundCoin;

    struct ShapeState {
        led::Pointi pos;
        led::Pointi size;
        bool isAlive;
        ShapeDef shape;
        size_t rotIdx;
        uint8_t colorIdx;
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
            for (led::Pointi& p : shape[rotIdx]) {
                if ((p.x + pos.x == px) && (p.y + pos.y == py)) {
                    return true;
                }
            }
            return false;
        }
    };

    ShapeState m_fallingShape;
    led::Image m_gameField;
    std::default_random_engine m_generator;
    std::uniform_int_distribution<int> m_posDist;
    size_t m_score;
    led::TimeUnit m_lastUpdateTimeFall;
    led::TimeUnit m_lastUpdateTimeMove;

public:

    Tetris();
    virtual ~Tetris();

    void initialize(led::BaseController *ctrl);
    void deinitialize();

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);

    const std::string getName() const{
      return "Tetris";
    }
private:

    void newShape();
};


#endif // ifndef _H_SNAKE_APP_

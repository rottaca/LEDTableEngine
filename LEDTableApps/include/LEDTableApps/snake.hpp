#ifndef _H_SNAKE_APP_
#define _H_SNAKE_APP_

#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/core/matrixScrollText.hpp>

class Snake : public led::BaseApplication {
protected:

    std::default_random_engine m_generator;
    std::uniform_int_distribution<int> m_posDist;
    typedef std::list<led::Pointi>SnakeList;
    struct SnakeData {
        SnakeList snake;
        led::Pointi currMoveDir, nextMoveDir;
    };
    std::vector<SnakeData>   m_snakes;
    std::vector<led::Pointi> m_foodPos;
    led::TimeUnit m_lastUpdateTime;

    led::Audio *m_soundCoin;

public:

    Snake();
    virtual ~Snake();

    void initialize(led::BaseController *ctrl);
    void deinitialize();

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);
};


#endif // ifndef _H_SNAKE_APP_

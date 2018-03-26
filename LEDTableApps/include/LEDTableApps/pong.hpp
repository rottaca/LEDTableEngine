#ifndef _H_PONG_APP_
#define _H_PONG_APP_

#include <LEDTableEngine/core/baseApplication.hpp>

class Pong : public led::BaseApplication {
protected:

    std::default_random_engine m_generator;
    std::uniform_real_distribution<float> m_randFloat;

    led::Pointf m_ballSpeed;
    led::Pointf m_ballPos;
    int m_playerHeight;
    std::array<int, 2>  m_playerPos;
    std::array<int, 2>  m_playerScore;
    std::array<bool, 2> m_playerWonRound;

    float m_speedIncrease;
    led::TimeUnit m_movementSpeed;

    led::TimeUnit m_lastUpdateTime;
    led::TimeUnit m_deathTime;

public:

    Pong();
    virtual ~Pong();

    void initialize(led::BaseController *ctrl);

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);

    const std::string getName() const{
      return "Pong";
    }
private:

    void resetRound();
};


#endif // ifndef _H_PONG_APP_

#ifndef _H_CONNECTFOUR_
#define _H_CONNECTFOUR_

#include <LEDTableEngine/core/baseApplication.hpp>

namespace ledApps{
class ConnectFour : public led::BaseApplication {
public:

    ConnectFour();
    virtual ~ConnectFour();

    void initialize(led::BaseController *ctrl);
    void deinitialize();

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);

    void continueApp();
    void pauseApp();

    const std::string getName() const{
      return "Connect Four";
    }
protected:

    std::vector<led::Pointi>checkForWin(led::Pointi insertedPoint);
    std::vector<led::Pointi>checkForWin(led::Pointi insertedPoint,
                                        led::Pointi searchDir);

private:

    led::Image m_gameField;
    led::BaseInput::PlayerID m_currPlayerId;
    bool m_chipIsFalling;
    bool m_playerWon;
    led::Pointi m_currFallingChipPos;
    led::TimeUnit m_lastFallTime;
    std::vector<led::Pointi> m_winPoints;
};
}

#endif // ifndef _H_CONNECTFOUR_

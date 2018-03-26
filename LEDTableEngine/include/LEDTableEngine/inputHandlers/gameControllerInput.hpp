#ifndef _H_GAMECTRL_INPUT_
#define _H_GAMECTRL_INPUT_

#include <cstdlib>
#include <memory>
#include <vector>
#include <string>
#include <linux/i2c-dev.h>

#include <LEDTableEngine/core/baseInput.hpp>

namespace led{
class GameControllerInput : public BaseInput {
private:

    int m_deviceHandle;
    std::vector<int> m_controllerId;
    std::string m_i2cDevName;

public:

    GameControllerInput(std::string i2cDev);
    virtual ~GameControllerInput();

    bool                        initialize();
    BaseInput::InputEvents getInputEvents();
    size_t                 getSupportedPlayerCnt();
private:

    void detectConnectedControllers();
    bool connectToDevice(std::string device);
    void disconnect();
};
}

#endif // ifndef _H_GAMECTRL_INPUT_

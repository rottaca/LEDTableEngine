#ifndef _H_UPDATE_APP_
#define _H_UPDATE_APP_

#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/core/font.hpp>
#include <LEDTableEngine/core/matrixScrollText.hpp>

class UpdateApp : public led::BaseApplication {
protected:

    enum State {
        IDLE,
        DO_UPDATE,
        UPDATING,
        DONE,
        ERROR
    };

    led::MatrixScrollText m_scrollText;
    pid_t m_childPID;
    State m_state;
    int m_stdoutPipe[2];

public:

    UpdateApp();
    virtual ~UpdateApp();

    void initialize(led::BaseController *ctrl);

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);
};


#endif // ifndef _H_UPDATE_APP_

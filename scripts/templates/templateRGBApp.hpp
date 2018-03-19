#ifndef _H_TEMPLATE_APP_INC_GUARD_NAME_
#define _H_TEMPLATE_APP_INC_GUARD_NAME_

#include <LEDTableEngine/core/baseApplication.hpp>

class TEMPLATE_APP_NAME : public led::BaseApplication {
public:

    TEMPLATE_APP_NAME();
    virtual ~TEMPLATE_APP_NAME();

    void initialize(led::BaseController *ctrl);
    void deinitialize();

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);

    void continueApp();
    void pauseApp();
};


#endif // ifndef _H_TEMPLATE_APP_INC_GUARD_NAME_

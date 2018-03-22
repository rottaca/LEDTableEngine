#ifndef _H_SHUTDOWNAPP_
#define _H_SHUTDOWNAPP_

#include <LEDTableEngine/core/baseApplication.hpp>

namespace led{
  class YesNoDisplay;
}

class ShutdownApp : public led::BaseApplication {
public:

    ShutdownApp();
    virtual ~ShutdownApp();

    void initialize(led::BaseController *ctrl);
    void deinitialize();

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);

    void continueApp();
    void pauseApp();

private:
  std::shared_ptr<led::YesNoDisplay>  m_yesNoDisplay;

  enum State {
    IDLE,
    USER_RESPONSE,
    SHUTDOWN_STARTED,
    SHUTDOWN_ABORTED
  };
  State m_state;
};


#endif // ifndef _H_SHUTDOWNAPP_

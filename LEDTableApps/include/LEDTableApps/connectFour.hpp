#ifndef _H_CONNECTFOUR_
#define _H_CONNECTFOUR_

#include <LEDTableEngine/core/baseApplication.hpp>

class ConnectFour : public led::BaseApplication {
public:
  ConnectFour();
  virtual ~ConnectFour();

  void initialize(led::BaseController *ctrl);
  void deinitialize();

  void processInput(const led::BaseInput::InputEvents& events,
                    led::TimeUnit                      deltaTime);
  void draw(led::Image& frame);

  void continueApp();
  void pauseApp();
};


#endif

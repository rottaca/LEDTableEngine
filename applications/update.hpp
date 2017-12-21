#ifndef _H_UPDATE_APP_
#define _H_UPDATE_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>

#include "../engine/baseApplication.hpp"
#include "../engine/font.hpp"
#include "../engine/matrixScrollText.hpp"

class UpdateApp : public BaseApplication {
protected:
  enum State {
    IDLE,
    DO_UPDATE,
    UPDATING,
    DONE,
    ERROR
  };

  MatrixScrollText m_scrollText;
  pid_t m_childPID;
  State m_state;
public:

  UpdateApp();
  virtual ~UpdateApp();

  void initialize(BaseController *ctrl);

  void processInput(const BaseInput::InputEvents& events,
                    TimeUnit                      deltaTime);
  void draw(Image& frame);
};


#endif // ifndef _H_UPDATE_APP_

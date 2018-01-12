#ifndef _H_TEXT_DISPLAY_APP
#define _H_TEXT_DISPLAY_APP

#include "../engine/baseApplication.hpp"
#include "../engine/font.hpp"
#include "../engine/matrixScrollText.hpp"

namespace led {
class MenuEntryHandler;
class BaseController;
}

class TextDisplay : public led::BaseApplication {
public:

protected:

  led::MatrixScrollText m_scrollText;
  std::string m_text;

public:

  TextDisplay();
  virtual ~TextDisplay();

  void initialize(led::BaseController *ctrl);

  void processInput(const led::BaseInput::InputEvents& events,
                    led::TimeUnit                      deltaTime);
  void draw(led::Image& frame);
  void setText(std::string text);
};

#endif // ifndef _H_TEXT_DISPLAY_APP

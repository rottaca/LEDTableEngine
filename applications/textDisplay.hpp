#ifndef _H_TEXT_DISPLAY_APP
#define _H_TEXT_DISPLAY_APP

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>

#include "../engine/baseApplication.hpp"
#include "../engine/font.hpp"
#include "../engine/matrixScrollText.hpp"

class MenuEntryHandler;
class BaseController;

class TextDisplay: public BaseApplication {
public:


protected:
  MatrixScrollText m_scrollText;
  std::string m_text;
  
public:
  TextDisplay();
  virtual ~TextDisplay ();

  void initialize(BaseController * ctrl);

  void processInput(const BaseInput::InputEvents &events,
                    const BaseInput::InputEvents &eventsDebounced,
                    TimeUnit deltaTime);
  void draw(Image &frame);
  void setText(std::string text);
};

#endif

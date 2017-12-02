#ifndef _H_KEYBOARD_INPUT_
#define _H_KEYBOARD_INPUT_

#include <cstdlib>
#include <memory>
#include <vector>
#include <string>

#include "../LEDTableEngine/baseInput.hpp"
#include "keyboard.hpp"

class KeyboardInput :public BaseInput{

public:
  KeyboardInput ();
  virtual ~KeyboardInput ();

  KeyboardInput::InputEvents getInputEvents();

private:
  cKeyboard m_keyboard;
};


#endif

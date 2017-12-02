
#include "keyboardInput.hpp"

KeyboardInput::KeyboardInput (){

}
KeyboardInput::~KeyboardInput (){

}

KeyboardInput::InputEvents KeyboardInput::getInputEvents(){
  InputEvents ie;
  if(m_keyboard.getKeyState(KEY_A)){
    ie.push_back(InputEvent(BaseInput::InputEventState::KEY_PRESSED,
                            BaseInput::InputEventName::LEFT));
  }
  if(m_keyboard.getKeyState(KEY_D)){
    ie.push_back(InputEvent(BaseInput::InputEventState::KEY_PRESSED,
                            BaseInput::InputEventName::RIGHT));
  }
  if(m_keyboard.getKeyState(KEY_W)){
    ie.push_back(InputEvent(BaseInput::InputEventState::KEY_PRESSED,
                            BaseInput::InputEventName::UP));
  }
  if(m_keyboard.getKeyState(KEY_S)){
    ie.push_back(InputEvent(BaseInput::InputEventState::KEY_PRESSED,
                            BaseInput::InputEventName::DOWN));
  }
  if(m_keyboard.getKeyState(KEY_Q)){
    ie.push_back(InputEvent(BaseInput::InputEventState::KEY_PRESSED,
                            BaseInput::InputEventName::EXIT));
  }
  if(m_keyboard.getKeyState(KEY_E)){
    ie.push_back(InputEvent(BaseInput::InputEventState::KEY_PRESSED,
                            BaseInput::InputEventName::ENTER));
  }
  return ie;
}

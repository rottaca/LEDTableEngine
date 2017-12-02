#include "baseInput.hpp"
#include "baseController.hpp"

const char* BaseInput::inputEventState2Str[] = {"KEY_PRESSED", "KEY_RELEASED", "KEY_HOLD", "KEY_NONE"};
const char* BaseInput::inputEventName2Str[] = {"UP", "LEFT", "DOWN", "RIGHT", "ENTER", "EXIT"};

BaseInput::BaseInput(){

}
BaseInput::~BaseInput(){

}

void BaseInput::setController(BaseController* ctrl){
  m_ctrl = ctrl;
}

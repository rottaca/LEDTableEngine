#include "baseInput.hpp"
#include "baseController.hpp"

namespace led {

const char *BaseInput::inputEventState2Str[] =
{ "KEY_PRESSED", "KEY_RELEASED", "KEY_HOLD", "KEY_NONE" };
const char *BaseInput::inputEventName2Str[] =
{ "UP", "LEFT", "DOWN", "RIGHT", "ENTER", "EXIT" };

BaseInput::BaseInput() {}

BaseInput::~BaseInput() {}

void BaseInput::setController(BaseController *ctrl) {
  m_ctrl = ctrl;
}

bool BaseInput::initialize() {
  return true;
}
void BaseInput::deinitialize() {

}

bool BaseInput::isReleased(const InputEvents& events, InputEventName name) {
  for (auto e : events) {
    if ((e.name == name) &&
        (e.state == InputEventState::KEY_RELEASED)) return true;
  }
  return false;
}

bool BaseInput::isPressed(const InputEvents& events, InputEventName name) {
  for (auto e : events) {
    if ((e.name == name) &&
        (e.state == InputEventState::KEY_PRESSED)) return true;
  }
  return false;
}

bool BaseInput::isHold(const InputEvents& events, InputEventName name) {
  for (auto e : events) {
    if ((e.name == name) && (e.state == InputEventState::KEY_HOLD)) return true;
  }
  return false;
}
}

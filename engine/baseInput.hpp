#ifndef _H_BASE_INPUT_
#define _H_BASE_INPUT_

#include <cstdlib>
#include <memory>
#include <vector>
#include <string>

class BaseController;

class BaseInput {
public:
  enum InputEventState { KEY_PRESSED, KEY_RELEASED, KEY_HOLD, KEY_NONE, TYPE_CNT };
  static const char* inputEventState2Str[];
  enum InputEventName { UP, LEFT, DOWN, RIGHT, ENTER, EXIT, NAME_CNT  };
  static const char* inputEventName2Str[];

  struct InputEvent{
    InputEventState state;
    InputEventName name;
    InputEvent(InputEventState state, InputEventName name){
      this->name = name;
      this->state = state;
    }
  };
  typedef std::vector<InputEvent> InputEvents;

private:
  BaseController* m_ctrl;

public:
  BaseInput ();
  virtual ~BaseInput ();

  virtual void setController(BaseController* ctrl);
  virtual bool initialize();
  virtual BaseInput::InputEvents getInputEvents()=0;

  static bool isReleased(const InputEvents& events, InputEventName name);
  static bool isPressed(const InputEvents& events, InputEventName name);
  static bool isHold(const InputEvents& events, InputEventName name);
};


#endif

#ifndef _H_KEYBOARD_DEBOUNCER
#define _H_KEYBOARD_DEBOUNCER

#include "baseInput.hpp"

namespace led {
class KeyboardDebouncer {
private:

  BaseInput::InputEvents m_events;

public:

  KeyboardDebouncer();
  virtual ~KeyboardDebouncer();

  virtual void                          processInput(
    const BaseInput::InputEvents& events);

  virtual const BaseInput::InputEvents& getDebouncedEvents();
};

}

#endif // ifndef _H_KEYBOARD_DEBOUNCER

#ifndef _H_KEYBOARD_DEBOUNCER
#define _H_KEYBOARD_DEBOUNCER

#include <cstdlib>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "baseInput.hpp"

class KeyboardDebouncer {
private:
  BaseInput::InputEvents m_events;
  std::vector<bool> m_eventUpdated;
public:
  KeyboardDebouncer ();
  virtual ~KeyboardDebouncer ();

  virtual void processInput(const BaseInput::InputEvents &events);

  virtual const BaseInput::InputEvents &getDebouncedEvents();
};


#endif

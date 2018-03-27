#ifndef _H_KEYBOARD_DEBOUNCER
#define _H_KEYBOARD_DEBOUNCER

#include "baseInput.hpp"

namespace led {
/**
 * @brief Converts continuously hold keys from KEY_PRESSED to KEY_HOLD.
 */
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

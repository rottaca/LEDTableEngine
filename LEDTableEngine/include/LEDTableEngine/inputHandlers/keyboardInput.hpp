#ifndef _H_KEYBOARD_INPUT_
#define _H_KEYBOARD_INPUT_

#include <cstdlib>
#include <memory>
#include <vector>
#include <string>

#include <LEDTableEngine/core/baseInput.hpp>
#include <LEDTableEngine/inputHandlers/keyboard.hpp>

class KeyboardInput : public led::BaseInput {
public:

    KeyboardInput(std::string keyboardDev);
    virtual ~KeyboardInput();

    bool                        initialize();
    led::BaseInput::InputEvents getInputEvents();

private:

    cKeyboard m_keyboard;
    std::string m_keyboardDevName;
};


#endif // ifndef _H_KEYBOARD_INPUT_

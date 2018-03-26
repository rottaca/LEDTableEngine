#include <LEDTableEngine/inputHandlers/keyboardInput.hpp>
#include <LEDTableEngine/configuration.hpp>

namespace led{
KeyboardInput::KeyboardInput(std::string keyboardDev) {
    m_keyboardDevName = keyboardDev;
}

KeyboardInput::~KeyboardInput() {
}

bool KeyboardInput::initialize() {
    return m_keyboard.start(m_keyboardDevName);
}

BaseInput::InputEvents KeyboardInput::getInputEvents() {
    InputEvents ie;

    BaseInput::InputEvent e;
    e.state    = InputEventState::KEY_PRESSED;

    if (m_keyboard.getKeyState(KEYBOARD_CTR_0_KEYMAP_LEFT)) {
        e.name     = BaseInput::InputEventName::LEFT;
        e.playerId = PLAYER_0;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_0_KEYMAP_RIGHT)) {
        e.name     = BaseInput::InputEventName::RIGHT;
        e.playerId = PLAYER_0;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_0_KEYMAP_UP)) {
        e.name     = BaseInput::InputEventName::UP;
        e.playerId = PLAYER_0;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_0_KEYMAP_DOWN)) {
        e.name     = BaseInput::InputEventName::DOWN;
        e.playerId = PLAYER_0;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_0_KEYMAP_EXIT)) {
        e.name     = BaseInput::InputEventName::EXIT;
        e.playerId = PLAYER_0;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_0_KEYMAP_ENTER)) {
        e.name     = BaseInput::InputEventName::ENTER;
        e.playerId = PLAYER_0;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_0_KEYMAP_A)) {
        e.name     = BaseInput::InputEventName::A;
        e.playerId = PLAYER_0;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_0_KEYMAP_B)) {
        e.name     = BaseInput::InputEventName::B;
        e.playerId = PLAYER_0;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_1_KEYMAP_LEFT)) {
        e.name     = BaseInput::InputEventName::LEFT;
        e.playerId = PLAYER_1;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_1_KEYMAP_RIGHT)) {
        e.name     = BaseInput::InputEventName::RIGHT;
        e.playerId = PLAYER_1;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_1_KEYMAP_UP)) {
        e.name     = BaseInput::InputEventName::UP;
        e.playerId = PLAYER_1;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_1_KEYMAP_DOWN)) {
        e.name     = BaseInput::InputEventName::DOWN;
        e.playerId = PLAYER_1;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_1_KEYMAP_EXIT)) {
        e.name     = BaseInput::InputEventName::EXIT;
        e.playerId = PLAYER_1;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_1_KEYMAP_ENTER)) {
        e.name     = BaseInput::InputEventName::ENTER;
        e.playerId = PLAYER_1;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_1_KEYMAP_A)) {
        e.name     = BaseInput::InputEventName::A;
        e.playerId = PLAYER_1;
        ie.push_back(e);
    }

    if (m_keyboard.getKeyState(KEYBOARD_CTRL_1_KEYMAP_B)) {
        e.name     = BaseInput::InputEventName::B;
        e.playerId = PLAYER_1;
        ie.push_back(e);
    }
    return ie;
}
size_t KeyboardInput::getSupportedPlayerCnt(){
  return 2;
}
}

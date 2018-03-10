#include <LEDTableEngine/inputHandlers/keyboardInput.hpp>

KeyboardInput::KeyboardInput(std::string keyboardDev) {
  m_keyboardDevName = keyboardDev;
}

KeyboardInput::~KeyboardInput() {}

bool KeyboardInput::initialize() {
  return m_keyboard.start(m_keyboardDevName);
}

led::BaseInput::InputEvents KeyboardInput::getInputEvents() {
  InputEvents ie;
  InputEvent  e(BaseInput::InputEventState::KEY_PRESSED,
                BaseInput::InputEventName::LEFT, 0);

  if (m_keyboard.getKeyState(KEY_A)) {
    e.name     = BaseInput::InputEventName::LEFT;
    e.playerId = 0;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_D)) {
    e.name     = BaseInput::InputEventName::RIGHT;
    e.playerId = 0;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_W)) {
    e.name     = BaseInput::InputEventName::UP;
    e.playerId = 0;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_S)) {
    e.name     = BaseInput::InputEventName::DOWN;
    e.playerId = 0;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_Q)) {
    e.name     = BaseInput::InputEventName::EXIT;
    e.playerId = 0;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_E)) {
    e.name     = BaseInput::InputEventName::ENTER;
    e.playerId = 0;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_R)) {
    e.name     = BaseInput::InputEventName::A;
    e.playerId = 0;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_F)) {
    e.name     = BaseInput::InputEventName::B;
    e.playerId = 0;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_LEFT)) {
    e.name     = BaseInput::InputEventName::LEFT;
    e.playerId = 1;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_RIGHT)) {
    e.name     = BaseInput::InputEventName::RIGHT;
    e.playerId = 1;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_UP)) {
    e.name     = BaseInput::InputEventName::UP;
    e.playerId = 1;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_DOWN)) {
    e.name     = BaseInput::InputEventName::DOWN;
    e.playerId = 1;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_BACKSPACE)) {
    e.name     = BaseInput::InputEventName::EXIT;
    e.playerId = 1;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_ENTER)) {
    e.name     = BaseInput::InputEventName::ENTER;
    e.playerId = 1;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_O)) {
    e.name     = BaseInput::InputEventName::A;
    e.playerId = 1;
    ie.push_back(e);
  }

  if (m_keyboard.getKeyState(KEY_L)) {
    e.name     = BaseInput::InputEventName::B;
    e.playerId = 1;
    ie.push_back(e);
  }
  return ie;
}

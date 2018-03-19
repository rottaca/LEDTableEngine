#include <LEDTableApps/connectFour.hpp>
#include <LEDTableEngine/core/baseController.hpp>

using namespace led;

ConnectFour::ConnectFour() {
  // Do some initialize that should only happen once!
}

ConnectFour::~ConnectFour() {}

void ConnectFour::initialize(BaseController *ctrl) {
  // Initialize base class
  BaseApplication::initialize(ctrl);
  // Specify a RGB color palette
  // Only use the indices into that table
  // if you set color values in the image
  m_colorPalette = {
    {   0,   0,   0 }, // Background
    {   0,   0, 255 }, // Color 1
    {   0, 255,   0 }, // Color 2
    { 255,   0,   0 }, // Color 3
    { 255, 255,   0 }, // Color 4
    {   0, 255, 255 }, // Color 5
    { 255,   0, 255 }, // Color 6
  };
  // Fill all remaining indices with a default color
  fillPalette(m_colorPalette);

  // Initialize your member variables here!
}

void ConnectFour::deinitialize() {
  // Deinitialize something
}
void ConnectFour::continueApp() {
  // Continue your app (only necessary if you launched new apps by your own)
}
void ConnectFour::pauseApp() {
  // Pause your app (only necessary if you launched new apps by your own)
}

void ConnectFour::processInput(const BaseInput::InputEvents& events,
                          TimeUnit                      deltaTime) {
  // Process incoming events
  for (const auto& e : events) {
    // Only consider key pressed and ignore KEY_RELEASED and KEY_HOLD
    if(e.state != BaseInput::InputEventState::KEY_PRESSED)
      continue;
    // Check which key has been pressed
    // This could happen multiple times in the "events" structure if you
    // have multiple players. Have a look at e.playerId to check that.
    switch (e.name) {
    case BaseInput::InputEventName::UP :
      break;

    case BaseInput::InputEventName::LEFT:
      break;

    case BaseInput::InputEventName::DOWN:
      break;

    case BaseInput::InputEventName::RIGHT:
      break;

    case BaseInput::InputEventName::ENTER:
      break;

    case BaseInput::InputEventName::EXIT:
      m_hasFinished = true;
      return;
    }
  }
}

void ConnectFour::draw(Image& frame) {
  // Set the frame to palette index 0
  m_ctrl->clearFrame(0);

}

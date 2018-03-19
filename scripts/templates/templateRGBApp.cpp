#include <LEDTableApps/TEMPLATE_APP_INC_HEADER.hpp>
#include <LEDTableEngine/core/baseController.hpp>

using namespace led;

TEMPLATE_APP_NAME::TEMPLATE_APP_NAME() {
  // Do some initialize that should only happen once!
}

TEMPLATE_APP_NAME::~TEMPLATE_APP_NAME() {}

void TEMPLATE_APP_NAME::initialize(BaseController *ctrl) {
  // Initialize base class
  BaseApplication::initialize(ctrl);
  // Switch to RGB color mode
  // This will create a new image buffer with RGB values
  // instead of a palette
  m_bufferColorMode = BufferColorMode::RGB;
  // Initialize your member variables here!
}

void TEMPLATE_APP_NAME::deinitialize() {
  // Deinitialize something
}
void TEMPLATE_APP_NAME::continueApp() {
  // Continue your app (only necessary if you launched new apps by your own)
}
void TEMPLATE_APP_NAME::pauseApp() {
  // Pause your app (only necessary if you launched new apps by your own)
}

void TEMPLATE_APP_NAME::processInput(const BaseInput::InputEvents& events,
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

void TEMPLATE_APP_NAME::draw(Image& frame) {
  // Set the frame to (0,0,0)
  m_ctrl->clearFrame({0,0,0});

  // Draw you content but remember, that you have to
  // set all RGB components for a pixel.
  for (size_t y = 0; y < frame.h; y++) {
    for (size_t x = 0; x < frame.w; x++) {
      frame(y,x,0) = 255*i/m_ctrl->getWidth();
      frame(y,x,1) = 255*i/m_ctrl->getHeight();
      frame(y,x,2) = 255*i*i/m_ctrl->getSize();
    }
  }
}

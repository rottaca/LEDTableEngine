#include <LEDTableApps/TEMPLATE_APP_INC_HEADER.hpp>
#include <LEDTableEngine/core/baseController.hpp>

using namespace led;

TEMPLATE_APP_NAME::TEMPLATE_APP_NAME() {
    // Do some initialize that should only happen once!
}

TEMPLATE_APP_NAME::~TEMPLATE_APP_NAME() {
}

void TEMPLATE_APP_NAME::initialize(BaseController *ctrl) {
    // Initialize base class
    BaseApplication::initialize(ctrl);

    // Specify a RGB color palette
    // Only use the indices into that table
    // if you set color values in the image
    m_colorPalette = {
        {   0,   0,   0 },        // Background
        {   0,   0, 255 },        // Color 1
        {   0, 255,   0 },        // Color 2
        { 255,   0,   0 },        // Color 3
        { 255, 255,   0 },        // Color 4
        {   0, 255, 255 },        // Color 5
        { 255,   0, 255 },        // Color 6
    };

    // Fill all remaining indices with a default color
    fillPalette(m_colorPalette);

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
                                     TimeUnit deltaTime) {
    // Process incoming events
    for (const auto& e : events) {
        // Only consider key pressed and ignore KEY_RELEASED and KEY_HOLD
        if (e.state != BaseInput::InputEventState::KEY_PRESSED) continue;

        // Check which key has been pressed
        // This could happen multiple times in the "events" structure if you
        // have multiple players. Have a look at e.playerId to check that.
        switch (e.name) {
        case BaseInput::InputEventName::UP:
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
    // Set the frame to palette index 0
    m_ctrl->clearFrame(0);

    // Draw you content but remember, that you have only
    // a palette image (1 channel per pixel not 3!)
    for (size_t y = 0; y < frame.height; y++) {
        for (size_t x = 0; x < frame.width; x++) {
            frame(y, x, 0) = 1;
        }
    }
}

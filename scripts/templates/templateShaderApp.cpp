#include <LEDTableApps/TEMPLATE_APP_INC_HEADER.hpp>
#include <LEDTableEngine/core/baseController.hpp>

using namespace led;

TEMPLATE_APP_NAME::TEMPLATE_APP_NAME() {
    // Do some initialize that should only happen once!
}

TEMPLATE_APP_NAME::~TEMPLATE_APP_NAME() {
}

void TEMPLATE_APP_NAME::initialize(BaseController *ctrl) {
    // Initialize the base class and set the number of implemented animations
    // Set the second parameter accordingly, if you change the amount of
    // animations.
    ShaderApplication::initialize(ctrl, 1);

    // Initialize your member variables here!
}

void TEMPLATE_APP_NAME::renderPixel(led::TimeUnit tm,
                                    size_t x,
                                    size_t y,
                                    uint8_t     & r,
                                    uint8_t     & g,
                                    uint8_t     & b,
                                    size_t shaderId) {
    // Select the right animation effect
    switch (shaderId) {
    case 0:
        effect1(tm, x, y, r, g, b);
        break;
    }
}

void TEMPLATE_APP_NAME::effect1(TimeUnit tm, size_t x, size_t y, uint8_t& r, uint8_t& g,
                                uint8_t& b) {
    // Set the color for the current pixel
    r = 255 * x / m_ctrl->getWidth();
    g = 255 * y / m_ctrl->getHeight();
    b = 255 * x * y / m_ctrl->getSize();
}

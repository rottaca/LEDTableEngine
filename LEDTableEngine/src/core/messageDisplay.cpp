#include <LEDTableEngine/core/messageDisplay.hpp>
#include <LEDTableEngine/core/baseController.hpp>

namespace led{

MessageDisplay::MessageDisplay() {
}

MessageDisplay::~MessageDisplay() {
}

void MessageDisplay::initialize(BaseController *ctrl) {
    BaseApplication::initialize(ctrl);
    m_colorPalette = {
        {   0,   0,   0 },
        { 255, 255, 255 },
    };
    fillPalette(m_colorPalette);

    m_scrollText.init(m_ctrl->getHeight(),
                      m_ctrl->getWidth(), m_ctrl->getDefaultFont());
    m_scrollText.setColorPalette(1);
    m_scrollText.setText(m_text);
}

void MessageDisplay::setText(std::string text) {
    m_text = text;
}

void MessageDisplay::processInput(const BaseInput::InputEvents& events,
                               TimeUnit deltaTime) {
    TimeUnit t = m_ctrl->getTimeMs();

    m_requiresRedraw = m_scrollText.update(t);

    for (auto& e : events) {
        if (e.state != BaseInput::InputEventState::KEY_PRESSED) {
            continue;
        }

        switch (e.name) {
        case BaseInput::InputEventName::ENTER:
        case BaseInput::InputEventName::EXIT: {
            m_hasFinished = true;
            break;
        }
        }
    }
}

void MessageDisplay::draw(Image& frame) {
    m_requiresRedraw = false;
    m_ctrl->clearFrame(0);
    m_scrollText.draw(frame);
}
}

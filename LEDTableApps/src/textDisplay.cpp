#include <LEDTableApps/textDisplay.hpp>
#include <LEDTableEngine/core/baseController.hpp>

using namespace led;

TextDisplay::TextDisplay() {
}

TextDisplay::~TextDisplay() {
}

void TextDisplay::initialize(BaseController *ctrl) {
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

void TextDisplay::setText(std::string text) {
    m_text = text;
}

void TextDisplay::processInput(const BaseInput::InputEvents& events,
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

void TextDisplay::draw(Image& frame) {
    m_requiresRedraw = false;
    m_ctrl->clearFrame(0);
    m_scrollText.draw(frame);
}

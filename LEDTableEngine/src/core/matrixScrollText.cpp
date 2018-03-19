#include <LEDTableEngine/core/matrixScrollText.hpp>

namespace led {
MatrixScrollText::MatrixScrollText() {
}

MatrixScrollText::~MatrixScrollText() {
}

void MatrixScrollText::init(size_t height,
                            size_t width,
                            std::shared_ptr<bmfont::Font>font,
                            TimeUnit initialWaitTime,
                            TimeUnit warpTimePerChar) {
    m_width                   = width;
    m_height                  = height;
    m_font                    = font;
    m_timeTextWarpInitialWait = initialWaitTime;
    m_timeTextWarpPerChar     = warpTimePerChar;
    m_scrollXPixels           = 0;
    m_paddingX                = 3;
    resetScrollState();
}

void MatrixScrollText::setText(std::string text) {
    resetScrollState();
    m_text          = text;
    m_currTextSize  = m_font->getTextSize(text);
    m_scrollXPixels = m_currTextSize.x - m_width + 2 * m_paddingX;
}

void MatrixScrollText::setColorRGB(ColorRGB c) {
    m_color = { c[0], c[1], c[2] };
}

void MatrixScrollText::setColorPalette(uint8_t c) {
    m_color = { c };
}

void MatrixScrollText::resetScrollState() {
    m_timeLastCharWarp  = 0;
    m_currScrollXPixels = 0;
}

bool MatrixScrollText::update(TimeUnit currTime) {
    // If we don't have moved so far, set to current time
    // and draw initially once
    if (m_timeLastCharWarp == 0)
    {
        m_timeLastCharWarp = currTime;
        return true;
    }

    // Move the text if one of the following conditions is fulfilled
    // - 0 < m_currScrollXPixels < END and it is time to move to the next position
    // - m_currScrollXPixels = 0 or = END and initial wait time is expired
    if (((m_currScrollXPixels != 0) && (m_currScrollXPixels != m_scrollXPixels) &&
         (currTime - m_timeLastCharWarp >= m_timeTextWarpPerChar)) ||
        (((m_currScrollXPixels == 0) || (m_currScrollXPixels == m_scrollXPixels)) &&
         (currTime - m_timeLastCharWarp >= m_timeTextWarpPerChar +
          m_timeTextWarpInitialWait)))
    {
        m_timeLastCharWarp  = currTime;
        m_currScrollXPixels = (m_currScrollXPixels + 1) % (m_scrollXPixels + 1);

        return true;
    }
    return false;
}

void MatrixScrollText::draw(Image& img) {
    m_font->draw(img,
                 m_text,
                 m_paddingX - (float)m_currScrollXPixels,
                 img.height / 2.0 - m_currTextSize.y / 2.0,
                 m_color);
}
}

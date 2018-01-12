#include "matrixScrollText.hpp"

namespace led{

MatrixScrollText::MatrixScrollText() {}

MatrixScrollText::~MatrixScrollText() {}

void MatrixScrollText::init(size_t                       height,
                            size_t                       width,
                            std::shared_ptr<bmfont::Font>font) {
  m_width  = width;
  m_height = height;
  m_font   = font;
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
  m_lastStepUpdate          = 0;
  m_timeTextWarpInitialWait = 1000;
  m_timeTextWarpStart       = 0;
  m_timeTextWarpPerChar     = 100;
  m_scrollXPixels           = 0;
  m_currScrollXPixels       = 0;
  m_paddingX                = 3;
}

bool MatrixScrollText::update(TimeUnit currTime) {
  if (((m_currScrollXPixels != 0) && (m_currScrollXPixels != m_scrollXPixels) &&
       (currTime - m_timeTextWarpStart >= m_timeTextWarpPerChar)) ||
      (((m_currScrollXPixels == 0) ||
        (m_currScrollXPixels == m_scrollXPixels)) &&
       (currTime - m_timeTextWarpStart >= m_timeTextWarpPerChar +
        m_timeTextWarpInitialWait)))
  {
    m_timeTextWarpStart = currTime;
    m_currScrollXPixels++;

    if (m_currScrollXPixels > m_scrollXPixels) {
      m_currScrollXPixels = 0;
    }
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

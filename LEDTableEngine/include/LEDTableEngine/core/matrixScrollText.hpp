#ifndef _H_MATRIX_SCROLL_TEXT
#define _H_MATRIX_SCROLL_TEXT

#include "types.hpp"
#include "font.hpp"

namespace led {
class MatrixScrollText {
private:

  std::shared_ptr<bmfont::Font> m_font;
  TimeUnit m_lastStepUpdate;
  TimeUnit m_timeTextWarpInitialWait;
  TimeUnit m_timeTextWarpStart;
  TimeUnit m_timeTextWarpPerChar;

  Pointi m_currTextSize;
  size_t m_scrollXPixels;
  size_t m_currScrollXPixels;
  size_t m_paddingX;
  size_t m_width;
  size_t m_height;
  std::vector<uint8_t> m_color;

  std::string m_text;

public:

  MatrixScrollText();
  virtual ~MatrixScrollText();

  void init(size_t                       height,
            size_t                       width,
            std::shared_ptr<bmfont::Font>font);
  bool update(TimeUnit currTime);
  void draw(Image& img);
  void setText(std::string text);
  void setColorRGB(ColorRGB c);
  void setColorPalette(uint8_t c);
  void resetScrollState();
};
}
#endif // ifndef _H_MATRIX_SCROLL_TEXT

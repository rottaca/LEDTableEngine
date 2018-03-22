#ifndef _H_MATRIX_SCROLL_TEXT
#define _H_MATRIX_SCROLL_TEXT

#include "types.hpp"
#include "font.hpp"

namespace led {
  /**
   * This class provides a simple interface to display
   * a scrolling text on a matrix display.
   * Specify the used font and the movement speed of the text
   * in the init function.
   * Use the update function to update the text state.
   */
class MatrixScrollText {
private:

    std::shared_ptr<bmfont::Font> m_font;
    TimeUnit m_timeTextWarpInitialWait;
    TimeUnit m_timeLastCharWarp;
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

/**
 * Initialize the matrix scroll text
 * @param height          Height of the display
 * @param width           Width of the display
 * @param font            The font to be used
 * @param initialWaitTime Initial time, to wait before the text moves
 * @param warpTimePerChar The time between subsequent moves (per char)
 */
    void init(size_t height,
              size_t width,
              std::shared_ptr<bmfont::Font>font,
              TimeUnit initialWaitTime = 500,
              TimeUnit warpTimePerChar = 30);

/**
 * Updates the scroll text state
 * @param  currTime Requires the current time
 * @return          Returns true, if rendering is required. Otherwise false.
 */
    bool update(TimeUnit currTime);

/**
 * Draw the actual scroll text centered in the image.
 */
    void draw(Image& img);

/**
 * Set a new text. This resets the scroll state.
 */
    void setText(std::string text);

/**
 * Set text color
 * @param c RGB
 */
    void setColorRGB(ColorRGB c);

/**
 * Set text color
 * @param c PaletteIndex
 */
    void setColorPalette(uint8_t c);

protected:

/**
 * Reset the scroll state
 */
    void resetScrollState();
};
}
#endif // ifndef _H_MATRIX_SCROLL_TEXT

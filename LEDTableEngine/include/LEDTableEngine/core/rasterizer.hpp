#ifndef _H_RASTERIZER
#define _H_RASTERIZER

#include "types.hpp"

namespace led {
/**
 * This class can be used to raster geometric primitives
 * onto an RGB or palette image buffer.
 */

class Rasterizer {
public:

  Rasterizer() {}

  ~Rasterizer() {}

  /**
   * Set color as palette color index.
   * The color palette for the currently active
   * application will be used for rendering.
   * The image has to be of type Palette (not RGB)!
   */
  void setColor(uint8_t color);

  /**
   * Specify the color as RGB value.
   * The image has to be of type RGB (not Palette)!
   */
  void setColor(ColorRGB color);

  /**
   * Draws a circle by using the midpoint circle algorithm.
   * Set the color with the setColor function but pay
   * attention to the color buffer mode (RGB, Palette)!
   * This implementation uses the Midpoint Circle algorithm.
   */
  void drawCircle(Image& img,
                  Pointi center,
                  int    radius);

  /**
   * Draws a line by using the midpoint line algorithm.
   * Set the color with the setColor function but pay
   * attention to the color buffer mode (RGB, Palette)!
   * This implementation uses the Midpoint Line algorithm.
   */
  void drawLine(Image& img,
                Pointi a,
                Pointi b);

private:

  // Current color. If Palette color mode is used by an image,
  // only the first channel of this color will be used.
  ColorRGB m_color;
};
}
#endif // ifndef _H_RASTERIZER

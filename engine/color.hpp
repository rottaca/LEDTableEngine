#ifndef _H_COLOR_
#define _H_COLOR_

#include "types.hpp"

namespace led {
typedef std::array<uint8_t, 3>ColorRGB;
typedef std::vector<ColorRGB> Palette;

void    fillPalette(Palette& palette);

Palette createPaletteColorFade(ColorRGB c1,
                               ColorRGB c2,
                               int      steps,
                               bool     fillTo256 = true);
Palette createPaletteHSV(float hStart,
                         float hStop,
                         float S,
                         float V,
                         int   steps,
                         bool  fillTo256 = true);
void hsv2rgb(float  h,
             float  s,
             float  v,
             float& r,
             float& g,
             float& b);
}
#endif // ifndef _H_COLOR_

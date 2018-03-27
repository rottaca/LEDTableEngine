#ifndef _H_COLOR_
#define _H_COLOR_
/** @file */

#include "types.hpp"

namespace led {
/** Datatype for an RGB color */
typedef std::array<uint8_t, 3>ColorRGB;

/** Datatype for a Palette (vector of RGB colors) */
typedef std::vector<ColorRGB>Palette;

/**
 * Appends so many black colors to the palette, that the
 * palette size equals 256.
 */
void    fillPalette(Palette& palette);

/**
 * Returns a palette that represents a linear interpolation
 * between the two provided colors with a given number of steps.
 */
Palette createPaletteColorFade(ColorRGB c1,
                               ColorRGB c2,
                               int steps,
                               bool fillTo256 = true);

/**
 * Creates a palette with 22 predefined colors.
 */
Palette createPaletteDistinctColors(bool fillTo256 = true);

/**
 * Creates a HSV color palette.
 */
Palette createPaletteHSV(float hStart,
                         float hStop,
                         float S,
                         float V,
                         int steps,
                         bool fillTo256 = true);

/**
 * Converts an HSV color to RGB.
 */
void hsv2rgb(float h,
             float s,
             float v,
             float& r,
             float& g,
             float& b);
}
#endif // ifndef _H_COLOR_

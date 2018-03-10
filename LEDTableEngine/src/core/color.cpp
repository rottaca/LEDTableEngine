#include <LEDTableEngine/core/color.hpp>

namespace led {
void fillPalette(Palette& palette) {
  for (size_t i = palette.size(); i < 256; i++) {
    palette.push_back({ 0, 0, 0 });
  }
}

Palette createPaletteColorFade(ColorRGB c1, ColorRGB c2,
                               int steps, bool fillTo256) {
  Palette p;

  for (size_t i = 0; i < steps; i++) {
    float t = i / (steps - 1);
    p.push_back({
                  c1[0] * t + c2[0] * (1 - t),
                  c1[1] * t + c2[1] * (1 - t),
                  c1[2] * t + c2[2] * (1 - t)
                });
  }

  if (fillTo256) fillPalette(p);

  return p;
}

Palette createPaletteDistinctColors(bool fillTo256) {
  // Source: https://sashat.me/2017/01/11/list-of-20-simple-distinct-colors/
  Palette p = {
    {   0,    0,    0 }, //  0 Black
    { 255,  255,  255 }, //  1 White
    { 230,   25,   75 }, //  2 Red
    {  60,  180,   75 }, //  3 Green
    { 255,  225,   25 }, //  4 Yellow
    {   0,  130,  200 }, //  5 Blue
    { 245,  130,   48 }, //  6 Orange
    { 145,   30,  180 }, //  7 Purple
    {  70,  240,  240 }, //  8 Cyan
    { 240,   50,  230 }, //  9 Magenta
    { 210,  245,   60 }, // 10 Lime
    { 250,  190,  190 }, // 11 Pink
    {   0,  128,  128 }, // 12 Teal
    { 230,  190,  255 }, // 13 Lavender
    { 170,  110,   40 }, // 14 Brown
    { 255,  250,  200 }, // 15 Beige
    { 128,    0,    0 }, // 16 Maroon
    { 170,  255,  195 }, // 17 Mint
    { 128,  128,    0 }, // 18 Olive
    { 255,  215,  180 }, // 19 Coral
    {   0,    0,  128 }, // 20 Navy
    { 128,  128,  128 }  // 21 Grey
  };

  if (fillTo256) fillPalette(p);

  return p;
}

Palette createPaletteHSV(float hStart, float hStop, float S, float V, int steps, bool fillTo256) {
  Palette p;
  float   hRange = hStop - hStart;
  float   r, g, b;

  for (size_t i = 0; i < steps; i++) {
    float t = (float)i / (steps - 1);
    hsv2rgb(hStart + t * hRange, S, V, r, g, b);
    p.push_back({ 255 * r, 255 * g, 255 * b });
  }

  if (fillTo256) fillPalette(p);

  return p;
}

void hsv2rgb(float h, float s, float v, float& r, float& g, float& b) {
  double hh, p, q, t, ff;
  long   i;

  if (s <= 0.0) { // < is bogus, just shuts up warnings
    r = v;
    g = v;
    b = v;
    return;
  }
  hh = h;

  if (hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i   = (long)hh;
  ff  = hh - i;
  p   = v * (1.0 - s);
  q   = v * (1.0 - (s * ff));
  t   = v * (1.0 - (s * (1.0 - ff)));

  switch (i) {
  case 0:
    r = v;
    g = t;
    b = p;
    break;

  case 1:
    r = q;
    g = v;
    b = p;
    break;

  case 2:
    r = p;
    g = v;
    b = t;
    break;

  case 3:
    r = p;
    g = q;
    b = v;
    break;

  case 4:
    r = t;
    g = p;
    b = v;
    break;

  case 5:
  default:
    r = v;
    g = p;
    b = q;
    break;
  }
}
}

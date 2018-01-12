#include "color.hpp"

namespace led{

void fillPalette(Palette &palette){
  for (size_t i = palette.size(); i < 256; i++) {
    palette.push_back({0,0,0});
  }
}

Palette createPaletteColorFade(ColorRGB c1, ColorRGB c2,
                                               int steps, bool fillTo256){
  Palette p;
  for (size_t i = 0; i < steps; i++) {
    float t = i/(steps-1);
    p.push_back({
      c1[0]*t + c2[0]*(1-t),
      c1[1]*t + c2[1]*(1-t),
      c1[2]*t + c2[2]*(1-t)
    });
  }
  if (fillTo256)
    fillPalette(p);

  return p;
}
Palette createPaletteHSV(float hStart, float hStop, float S, float V, int steps, bool fillTo256){
  Palette p;
  float hRange = hStop-hStart;
  float r,g,b;
  for (size_t i = 0; i < steps; i++) {
    float t = (float)i/(steps-1);
    hsv2rgb(hStart + t*hRange, S, V, r, g, b);
    p.push_back({255*r,255*g,255*b});
  }
  if (fillTo256) {
    fillPalette(p);
  }
  return p;

}
void hsv2rgb(float h, float s, float v, float &r, float &g, float &b){
  double      hh, p, q, t, ff;
  long        i;

  if(s <= 0.0) {       // < is bogus, just shuts up warnings
      r = v;
      g = v;
      b = v;
      return;
  }
  hh = h;
  if(hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = v * (1.0 - s);
  q = v * (1.0 - (s * ff));
  t = v * (1.0 - (s * (1.0 - ff)));

  switch(i) {
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
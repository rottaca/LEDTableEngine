#include <LEDTableEngine/core/rasterizer.hpp>

/**
 * Set a pixel, if the pixel position is inside the image.
 */
#define SET_PX_IF_VALID(IM, Y, X, I, V) \
  { if (X >= 0 && X < IM.width &&       \
        Y >= 0 && Y < IM.height) { IM(Y, X, I) = V; } }

namespace led {
void Rasterizer::setColor(uint8_t color) {
  for (size_t i = 0; i < 3; i++) {
    m_color[i] = color;
  }
}

void Rasterizer::setColor(ColorRGB color) {
  m_color = color;
}

void Rasterizer::drawCircle(Image& img, Pointi center, int radius) {
  Pointi p(radius, 0);

  if ((center.x < 0) || (center.y < 0) ||
      (center.x >= img.width) || (center.y >= img.height)) return;

  int P = 1 - radius;

  for (size_t i = 0; i < img.channels; i++) {
        SET_PX_IF_VALID(img, radius + center.y,  center.x,           i, m_color[i]);

    if (radius > 0) {
        SET_PX_IF_VALID(img, center.y,           radius + center.x,  i, m_color[i]);
        SET_PX_IF_VALID(img, -radius + center.y, center.x,           i, m_color[i]);
        SET_PX_IF_VALID(img, center.y,           -radius + center.x, i, m_color[i]);
    }
  }

  while (p.x > p.y) {
    p.y++;

    if (P <= 0) {
      P = P + 2 * p.y + 1;
    }
    else {
      p.x--;
      P = P + 2 * p.y - 2 * p.x + 1;
    }

    if (p.x < p.y) break;

    for (size_t i = 0; i < img.channels; i++) {
        SET_PX_IF_VALID(img, p.y + center.y,  p.x + center.x,  i, m_color[i]);
        SET_PX_IF_VALID(img, p.y + center.y,  -p.x + center.x, i, m_color[i]);
        SET_PX_IF_VALID(img, -p.y + center.y,    p.x + center.x,  i, m_color[i]);
        SET_PX_IF_VALID(img, -p.y + center.y,    -p.x + center.x, i, m_color[i]);

      if (p.x != p.y) {
        SET_PX_IF_VALID(img, p.x + center.y,  p.y + center.x,  i, m_color[i]);
        SET_PX_IF_VALID(img, p.x + center.y,  -p.y + center.x, i, m_color[i]);
        SET_PX_IF_VALID(img, -p.x + center.y, p.y + center.x,  i, m_color[i]);
        SET_PX_IF_VALID(img, -p.x + center.y, -p.y + center.x, i, m_color[i]);
      }
    }
  }
}

void Rasterizer::drawLine(Image& img,
                          Pointi a,
                          Pointi b)
{
  Pointi p = a;
  Pointi delta;

  delta.x = abs(b.x - a.x);
  delta.y = -abs(b.y - a.y);
  Pointi s;
  s.x = a.x < b.x ? 1 : -1;
  s.y = a.y < b.y ? 1 : -1;
  int err = delta.x + delta.y;
  int e2;

  while (1) {
    for (size_t i = 0; i < img.channels; i++) {
        SET_PX_IF_VALID(img, p.y, p.x, i, m_color[i]);
    }

    if (p == b) break;
    e2 = 2 * err;

    if (e2 > delta.y) {
      err += delta.y;
      p.x += s.x;
    }

    if (e2 < delta.x) {
      err += delta.x;
      p.y += s.y;
    }
  }
}
}

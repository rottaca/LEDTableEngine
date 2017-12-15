#ifndef _H_TYPES_
#define _H_TYPES_
#include <chrono>
#include <vector>
#include <array>
#include <iostream>
#include <assert.h>
#include <memory>

typedef int64_t               TimeUnit;
typedef std::array<uint8_t, 3>ColorRGB;
typedef std::vector<ColorRGB> Palette;

enum BufferColorMode { RGB, PALETTE };

template<typename T>
struct Point {
  T x, y;
  Point() {
    x = 0;
    y = 0;
  }

  Point(T x,
        T y) {
    this->x = x;
    this->y = y;
  }

  bool operator==(const Point<T>& r) const
  {
    return x == r.x && y == r.y;
  }

  bool operator()(const Point<T>& r) const
  {
    return x == r.x && y == r.y;
  }
};
typedef Point<int>  Pointi;
typedef Point<float>Pointf;


struct Image {
  uint8_t *data;
  size_t   width;
  size_t   height;
  size_t   size;
  size_t   rowstride;
  uint8_t  channels;
  bool     ownsData;

  Image() {
    ownsData  = false;
    data      = nullptr;
    width     = 0;
    height    = 0;
    size      = 0;
    rowstride = 0;
    channels  = 0;
  }

  Image(size_t  h,
        size_t  w,
        uint8_t ch) {
    data      = new uint8_t[w * h * ch];
    width     = w;
    height    = h;
    rowstride = width * ch;
    size      = rowstride * height;
    channels  = ch;
    ownsData  = true;
  }

  Image(uint8_t *d,
        size_t   h,
        size_t   w,
        uint8_t  ch) {
    data      = d;
    width     = w;
    height    = h;
    rowstride = width * ch;
    size      = rowstride * height;
    channels  = ch;
    ownsData  = false;
  }

  void resize(size_t h, size_t w, uint8_t ch) {
    if ((h == height) && (w == width) && (ch == channels)) return;

    release();
    data      = new uint8_t[w * h * ch];
    width     = w;
    height    = h;
    rowstride = width * ch;
    size      = rowstride * height;
    channels  = ch;
    ownsData  = true;
  }

  void release() {
    if (ownsData && data) delete[] data;
    data     = nullptr;
    ownsData = false;
  }

  uint8_t& operator()(const size_t y, const size_t x, const size_t ch) {
    assert(y < height);
    assert(x < width);
    assert(ch < channels);
    assert(y * rowstride + x * channels + ch < size);
    return data[y * rowstride + x * channels + ch];
  }

  ~Image() {
    release();
  }
};
#endif // ifndef _H_TYPES_

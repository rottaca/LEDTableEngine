#include <LEDTableApps/boardGameApp.hpp>
#include <LEDTableEngine/core/baseController.hpp>

using namespace led;
BoardGameApp::BoardGameApp() {}

BoardGameApp::~BoardGameApp() {}

void BoardGameApp::initialize(BaseController *ctrl) {
  ShaderApplication::initialize(ctrl, 1);
}

void BoardGameApp::renderPixel(led::TimeUnit tm,
                               size_t        x,
                               size_t        y,
                               uint8_t     & r,
                               uint8_t     & g,
                               uint8_t     & b,
                               size_t        shaderId) {
  switch (shaderId) {
  case 0:
    chessboardShader(tm, x, y, r, g, b);
    break;
  }

  // Only draw once, if there is no user input
  m_requiresRedraw = false;
}

void BoardGameApp::chessboardShader(TimeUnit tm, size_t x, size_t y, uint8_t& r, uint8_t& g,
                                    uint8_t& b) {
  ColorRGB c1     = { 0, 0, 0 };
  ColorRGB c2     = { 255, 255, 255 };
  int tilesPerDim = 8;
  int borderSize  = 2;
  int padX        = (m_ctrl->getWidth() - tilesPerDim) / 2;
  int padY        = (m_ctrl->getHeight() - tilesPerDim) / 2;

  // Render the border of the chess board
  if ((x >= padX) && (x <= m_ctrl->getWidth() - 1 - padX) &&
      (y >= padY) && (y <= m_ctrl->getHeight() - 1 - padY)) {
    if ((x % 2 == 0) != (y % 2 == 0)) {
      r = c1[0];
      g = c1[1];
      b = c1[2];
    } else {
      r = c2[0];
      g = c2[1];
      b = c2[2];
    }
  }

  // Render the actual chess field
  else if (((x >= padX - borderSize) && (x <= m_ctrl->getWidth() - 1 + borderSize - padX)) &&
           ((y >= padY - borderSize) && (y <= m_ctrl->getHeight() - 1 + borderSize - padY))) {
    r = 210;
    g = 105;
    b = 30;
  } else {
    r = 0;
    g = 153;
    b = 204;
  }
}

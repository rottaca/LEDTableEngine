#include <LEDTableApps/boardGameApp.hpp>
#include <LEDTableEngine/core/baseController.hpp>

using namespace led;
BoardGameApp::BoardGameApp() {}

BoardGameApp::~BoardGameApp() {}

void BoardGameApp::initialize(BaseController *ctrl) {
  ShaderApplication::initialize(ctrl, 2);
}

void BoardGameApp::renderPixel(led::TimeUnit tm,
                               size_t        x,
                               size_t        y,
                               uint8_t     & r,
                               uint8_t     & g,
                               uint8_t     & b,
                               size_t        shaderId) {
}

void BoardGameApp::draw(led::Image& frame) {
  m_ctrl->clearFrame({0,0,0});

  switch (m_shaderCurrId) {
    case 0 :
      chessboard(frame);
    break;
    case 1:
      MADN(frame);
    break;
  }
  m_requiresRedraw = false;
}

void BoardGameApp::chessboard(led::Image& img) {

  size_t   xsize = img.width;
  size_t   ysize = img.height;
  uint8_t *p      = img.data;
  TimeUnit tm     = m_ctrl->getTimeMs();
  ColorRGB c1     = { 0, 0, 0 };
  ColorRGB c2     = { 255, 255, 255 };
  int tilesPerDim = 8;
  int borderSize  = 2;
  int padX        = (xsize - tilesPerDim) / 2;
  int padY        = (ysize - tilesPerDim) / 2;

  for (std::size_t y = 0; y < ysize; ++y)
  {
    for (std::size_t x = 0; x < xsize; ++x)
    {
      // Render the actual chess field
      if ((x >= padX) && (x <= xsize - 1 - padX) &&
          (y >= padY) && (y <= ysize - 1 - padY)) {
        if ((x % 2 == 0) != (y % 2 == 0)) {
          for (size_t i = 0; i < 3; i++)  img(y,x,i) = c1[i];
        } else {
          for (size_t i = 0; i < 3; i++)  img(y,x,i) = c2[i];
        }
      }

      // Render the border of the chess board
      else if (((x >= padX - borderSize) && (x <= xsize - 1 + borderSize - padX)) &&
               ((y >= padY - borderSize) && (y <= ysize - 1 + borderSize - padY))) {
        img(y,x,0) = 210;
        img(y,x,1) = 105;
        img(y,x,2) = 30;
      } else {
        img(y,x,0) = 0;
        img(y,x,1) = 153;
        img(y,x,2) = 204;
      }
    }
  }
}

void BoardGameApp::MADN(led::Image& img)
{
  size_t   xsize = img.width;
  size_t   ysize = img.height;
  int tilesPerDim = 11;
  int padXL        = ceil((xsize - tilesPerDim) / 2.0);
  int padYU        = ceil((ysize - tilesPerDim) / 2.0);

  // Draw all 4 corners
  for (size_t y = 0; y < 2; y++) {
    for (size_t x = 0; x < 2; x++) {
      // red
      img(padYU+y,padXL+x,0) = 255;
      // blue
      img(padYU+y,padXL + tilesPerDim - 1 - x,2) = 255;
      // green
      img(ysize-1-y-padYU,padXL + tilesPerDim - 1 - x,1) = 255;
      // yellow
      img(ysize-1-y-padYU,padXL+x,0) = 255;
      img(ysize-1-y-padYU,padXL+x,1) = 255;
    }
  }
  // Draw white area
  m_rasterizer.setColor({255,255,255});
  m_rasterizer.drawRectAA(img,Pointi(padXL,ysize/2.0-1), Pointi(padXL + tilesPerDim - 1,ysize/2.0+1));
  m_rasterizer.drawRectAA(img,Pointi(xsize/2.0-1,padYU), Pointi(xsize/2.0+1,padYU + tilesPerDim - 1));

  // Draw start points
  // red
  img(ysize/2.0-1,padXL,0) = 255;
  img(ysize/2.0-1,padXL,1) = 0;
  img(ysize/2.0-1,padXL,2) = 0;
  // green
  img(ysize/2.0+1,padXL + tilesPerDim - 1,0) = 0;
  img(ysize/2.0+1,padXL + tilesPerDim - 1,1) = 255;
  img(ysize/2.0+1,padXL + tilesPerDim - 1,2) = 0;
  // blue
  img(padYU,padXL + tilesPerDim/2.0+1,0) = 0;
  img(padYU,padXL + tilesPerDim/2.0+1,1) = 0;
  img(padYU,padXL + tilesPerDim/2.0+1,2) = 255;
  // yellow
  img(padYU + tilesPerDim - 1,xsize/2-1.0,0) = 255;
  img(padYU + tilesPerDim - 1,xsize/2-1,1) = 255;
  img(padYU + tilesPerDim - 1,xsize/2-1 ,2) = 0;

  // Draw goal areas
  m_rasterizer.setColor({255,0,0});
  m_rasterizer.drawLine(img, Pointi(padXL+1,padYU+tilesPerDim/2.0), Pointi(padXL+4,padYU+tilesPerDim/2.0));
  m_rasterizer.setColor({0,0,255});
  m_rasterizer.drawLine(img, Pointi(padXL+tilesPerDim/2.0,padYU+1), Pointi(padXL+tilesPerDim/2,padYU+4));
  m_rasterizer.setColor({0,255,0});
  m_rasterizer.drawLine(img, Pointi(padXL + tilesPerDim - 2,padYU+tilesPerDim/2.0), Pointi(padXL + tilesPerDim - 5,padYU+tilesPerDim/2.0));
  m_rasterizer.setColor({255,255,0});
  m_rasterizer.drawLine(img, Pointi(padXL+tilesPerDim/2.0,padYU + tilesPerDim -2), Pointi(padXL+tilesPerDim/2.0,padYU + tilesPerDim-5));

}

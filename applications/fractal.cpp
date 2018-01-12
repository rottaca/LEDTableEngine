#include "fractal.hpp"
#include "../engine/baseController.hpp"
using namespace led;
FractalApp::FractalApp() {}

FractalApp::~FractalApp() {}

void FractalApp::initialize(BaseController *ctrl) {
  BaseApplication::initialize(ctrl);

  // m_bufferColorMode = BufferColorMode::RGB;

  m_frac = FractalDef(-1.401155189, 0.0001,
                      5, 5.0 * ctrl->getHeight() / ctrl->getWidth(),
                      0.0001, 10000);

  m_colorPalette.resize(255);

  for (int i = 0; i <= 255; i++) {
    percentToRGB((float)i / 255,
                 m_colorPalette[i][0],
                 m_colorPalette[i][1],
                 m_colorPalette[i][2]);
  }
  m_colorPalette.insert(m_colorPalette.end(), { 255, 255, 255 });

  m_currZoom = m_frac.zoomMin;
  m_zoomDir  = 1;
}

void FractalApp::continueApp() {}

void FractalApp::processInput(const BaseInput::InputEvents& events,
                              TimeUnit                      deltaTime) {
  if (BaseInput::isPressed(events, BaseInput::InputEventName::EXIT)) {
    m_hasFinished = true;
    return;
  }

  m_currZoom += m_zoomDir * (1 + deltaTime / 2000.0);

  if (m_currZoom > m_frac.zoomMax)
  {
    m_currZoom = m_frac.zoomMax;
    m_zoomDir  = -m_zoomDir;
  } else if (m_currZoom < m_frac.zoomMin)
  {
    m_currZoom = m_frac.zoomMin;
    m_zoomDir  = -m_zoomDir;
  }
}

void FractalApp::draw(Image& frame) {
  draw_Mandelbrot(frame,
                  m_frac.center.x - m_frac.dim.x / m_currZoom,
                  m_frac.center.x + m_frac.dim.x / m_currZoom,
                  m_frac.center.y - m_frac.dim.y / m_currZoom,
                  m_frac.center.y + m_frac.dim.y / m_currZoom,
                  50 * pow(log10(frame.width / (m_frac.dim.x / m_currZoom)), 1.25));
}

void FractalApp::percentToRGB(float t, uint8_t& r, uint8_t& g, uint8_t& b) {
  if (t < 0.5) {
    r = (1 - t * 2) * 255;
    g = t * 2 * 255;
    b = 0;
  } else {
    r = 0;
    g = (1 - (t - 0.5) * 2) * 255;
    b = ((t - 0.5) * 2) * 255;
  }
}

void FractalApp::draw_Mandelbrot(Image      & frame,

// where to draw the image
                                 double       cxmin,
                                 double       cxmax,
                                 double       cymin,
                                 double       cymax,

                                                              // the rect to
                                                              // draw in the
                                                              // complex plane
                                 unsigned int max_iterations) // the maximum
                                                              // number of
                                                              // iterations
{
  size_t   ixsize = frame.width;
  size_t   iysize = frame.height;
  uint8_t *p      = frame.data;

  for (std::size_t iy = 0; iy < iysize; ++iy)
  {
    for (std::size_t ix = 0; ix < ixsize; ++ix)
    {
      std::complex<double> c(cxmin + ix / (ixsize - 1.0) * (cxmax - cxmin),
                             cymin + iy / (iysize - 1.0) * (cymax - cymin));
      std::complex<double> z = 0;
      unsigned int iterations;

      for (iterations = 0; iterations < max_iterations && std::abs(z) < 2.0;
           ++iterations) z = z * z + c;

      *p++ = 255 * iterations / max_iterations;
    }
  }
}

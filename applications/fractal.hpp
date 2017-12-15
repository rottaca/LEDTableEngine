#ifndef _H_FRACTAL_APP_
#define _H_FRACTAL_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>
#include <cstdlib>
#include <complex>

#include "../engine/baseApplication.hpp"

class FractalApp : public BaseApplication {
protected:

  struct FractalDef {
    Pointf center;
    Pointf dim;
    float  zoomMin, zoomMax;
    FractalDef() {}

    FractalDef(float cx,
               float cy,
               float sx,
               float sy,
               float fmin,
               float fmax) {
      center.x = cx;
      center.y = cy;
      dim.x    = sx;
      dim.y    = sy;
      zoomMin  = fmin;
      zoomMax  = fmax;
    }
  };

  float m_currZoom;
  float m_zoomDir;
  FractalDef m_frac;

public:

  FractalApp();
  virtual ~FractalApp();

  void initialize(BaseController *ctrl);

  void processInput(const BaseInput::InputEvents& events,
                    TimeUnit                      deltaTime);
  void draw(Image& frame);
  void continueApp();

private:

  void draw_Mandelbrot(Image      & frame, // where to draw the image
                       double       cxmin,
                       double       cxmax,
                       double       cymin,
                       double       cymax, // the rect to draw in the complex
                                           // plane
                       unsigned int max_iterations);

  void percentToRGB(float    t,
                    uint8_t& r,
                    uint8_t& g,
                    uint8_t& b);
};


#endif // ifndef _H_FRACTAL_APP_

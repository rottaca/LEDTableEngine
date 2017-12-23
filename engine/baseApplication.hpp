#ifndef _H_BASE_APP_
#define _H_BASE_APP_

#include <cstdlib>
#include <memory>
#include <vector>

#include "baseInput.hpp"
#include "types.hpp"
#include "audio.h"

class BaseController;

class BaseApplication {
protected:

  BaseController *m_ctrl;
  bool m_requiresRedraw;
  bool m_hasFinished;
  bool m_isPaused;
  Palette m_colorPalette;
  BufferColorMode m_bufferColorMode;

public:

  BaseApplication();
  virtual ~BaseApplication();

  virtual void            initialize(BaseController *ctrl);
  virtual void            deinitialize();
  virtual void            pauseApp();
  virtual void            continueApp();
  virtual bool            requiresRedraw();
  virtual bool            hasFinished();
  virtual const Palette&  getPalette();
  virtual BufferColorMode getBufferColorMode();

  virtual void            processInput(const BaseInput::InputEvents& events,
                                       TimeUnit                      deltaTime) = 0;
  virtual void            draw(Image& frame)                                    = 0;

protected:

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
};


#endif // ifndef _H_BASE_APP_

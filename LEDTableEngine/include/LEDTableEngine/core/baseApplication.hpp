#ifndef _H_BASE_APP_
#define _H_BASE_APP_

#include "types.hpp"
#include "audio.hpp"
#include "baseInput.hpp"

namespace led {
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

};
}

#endif // ifndef _H_BASE_APP_

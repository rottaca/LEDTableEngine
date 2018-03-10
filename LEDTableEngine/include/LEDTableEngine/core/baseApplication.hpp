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
  /**
   * Initialize the application.
   */
  virtual void            initialize(BaseController *ctrl);
  /**
   * Deinitialize the application.
   */
  virtual void            deinitialize();
  /**
   * Pause the application if e.g. a new app is launched ontop of the stack.
   */
  virtual void            pauseApp();
  /**
   * Continue the application if e.g. a app has been removed for the
   * top of the stack.
   */
  virtual void            continueApp();
  /**
   * Returns true, if the app has to redraw its image. Can be used to reduce
   * the number of required display updates.
   */
  virtual bool            requiresRedraw();
  /**
   * Returns true, if the application has finished. The deinitialize function
   * will be called soon if this function returns true.
   */
  virtual bool            hasFinished();
  /**
   * Returns the color palette for this application.
   */
  virtual const Palette&  getPalette();
  /**
   * Returns the required buffer color mode for this application.
   */
  virtual BufferColorMode getBufferColorMode();
  /**
   * Process incoming input events.
   * @param  events    List of translated input events
   * @param  deltaTime Time since the last update
   */
  virtual void            processInput(const BaseInput::InputEvents& events,
                                       TimeUnit                      deltaTime) = 0;
  /**
   * Draw a new frame to the provided image buffer (RGB or Palette)
   */
  virtual void            draw(Image& frame)                                    = 0;

protected:

};
}

#endif // ifndef _H_BASE_APP_

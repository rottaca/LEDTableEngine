#ifndef _H_BASE_CONTROLLER_
#define _H_BASE_CONTROLLER_

#include "types.hpp"

#include "baseApplication.hpp"

#include "keyboardDebouncer.hpp"
#include "font.hpp"
#include "audio.hpp"

namespace led {
class BaseInput;

class BaseController {
protected:

  // The width of the matrix in pixels
  size_t m_width;

  // The height of the matrix in pixels
  size_t m_height;

  // The size of the matrix (w*h)
  size_t m_size;

  // If debug is true, a short processing summary is printed
  // as well as the processed input events.
  bool m_debug;

  // True, if the engine is running
  bool m_isRunning;

  // Color mode for the frame buffer.
  // May be RGB or indexed image (with palette).
  BufferColorMode m_bufferMode;

  // Brightness of display. RGB Colors are multiplied by this value.
  // Ranges from 0 to 1
  float m_brightness;

  // Number of currently active players
  size_t m_playerCnt;

private:

  // Shared pointer to the input handler
  // Currently only a single input device is possible.
  std::shared_ptr<BaseInput> m_inputHandler;

  // Stack of applications: New apps are placed on top of the stack.
  // The lowest element is always the main menu application
  std::stack<std::shared_ptr<BaseApplication> > m_applicationStack;

  // Applications that have to be inserted after processing the currently
  // running Applications. This is required, if an app inserts a new
  // App in its own update function.
  std::vector<std::shared_ptr<BaseApplication> > m_queuedApplications;

  // The actual framebuffer that holds the current image (RGB or indexed)
  Image m_frameBuffer;

  // The debouncer is used to detect hold keys
  KeyboardDebouncer m_kdb;

  // Pointer to a bitmap font, used for text rendering
  std::shared_ptr<bmfont::Font> m_font;

  // Used as start time, so that getTimeMs() returns
  // the time relative to application start
  size_t m_refTimeStartUs;

public:

  BaseController();
  virtual ~BaseController();

  /**
   * Initializes the frambuffer, colormode, audio system add
   * the font rendering.
   * @param  width  Width in pixels
   * @param  height Height in pixels
   * @param  input  Pointer to the input handler
   * @param  debug  Debug mode
   * @return        False on error, otherwise true
   */
  virtual bool initialize(size_t                    width,
                          size_t                    height,
                          std::shared_ptr<BaseInput>input,
                          bool                      debug = false);

  /**
   * Adds a new application on the application stack
   * @param app          Shared pointer to the application
   * @param queuedInsert Delays the insertion. If this is true, the currently
   *                     active application is able to shutdown before the next
   *                     app is started. Alwys set this to true, if you launch a
   *                     new app from inside an other app!
   */
  void addApplication(std::shared_ptr<BaseApplication>app,
                      bool                            queuedInsert = false);

  /**
   * The actual main loop that processes events and renders the image
   * @param fps The update/processing rate of the engine
   */
  void run(size_t fps = 50);

  /**
   * Clear the image and set it to the specified color.
   * Use this function if the color buffer mode of your app is set to RGB.
   */
  void clearFrame(ColorRGB color);

  /**
   * Clear the image and set it to the specified color.
   * Use this function if the color buffer mode is set to palette.
   */
  void clearFrame(uint8_t paletteIdx);

  /**
   * Adjust the brightness of the rendered image.
   * @param b Values in the range of 0-1
   */
  void setBrightness(float b) {
    m_brightness = b;
  }

  /**
   * Returns the currently set brightness.
   */
  float getBrightness() {
    return m_brightness;
  }

  /**
   * Sets the number of players that are allowed to interact with the engine
   * (e.g. in games)
   */
  void setPlayerCount(size_t p) {
    m_playerCnt = p;
  }

  /**
   * Returns the number of players that are allowed to interact with the engine.
   */
  size_t getPlayerCount() {
    return m_playerCnt;
  }

  /**
   * Returns the time since engine start in milliseconds.
   */
  TimeUnit getTimeMs();

  /**
   * Returns the display width in pixels.
   */
  size_t   getWidth() {
    return m_width;
  }

  /**
   * Returns the display height in pixels.
   */
  size_t getHeight() {
    return m_height;
  }

  /**
   * Returns the number of pixels in the image.
   */
  size_t getSize() {
    return m_size;
  }

  /**
   * Returns true, if debugging is enabeled
   * @return [description]
   */
  bool isDebug() {
    return m_debug;
  }

  /**
   * Returns a pointer to the default font.
   */
  std::shared_ptr<bmfont::Font>getDefaultFont() {
    return m_font;
  }

protected:

  /**
   * Renders the provided frame to a implementation dependent device
   * (Computer Monitor, LED-Matrix,...)
   */
  virtual void           showFrame(const Image& frame) = 0;

  /**
   * Shutdown the engine and deinitialize all components
   */
  virtual void           shutdown();

  /**
   * Returns a const reference to the currently active palette
   */
  virtual const Palette& getCurrentPalette();

  /**
   * Recreates the frame if the currently active app requires a different
   * color mode.
   */
  void                   updateBufferColorMode();

private:

  /**
   * Create a new frame based on the image size and color mode.
   */
  void createFrame();

  /**
   * Directly add a new application to the application stack.
   */
  void addApplicationDirect(std::shared_ptr<BaseApplication>app);
};
}

#endif // ifndef _H_BASE_CONTROLLER_

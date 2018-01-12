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

  size_t m_width;
  size_t m_height;
  size_t m_size;
  bool   m_debug, m_isRunning;
  BufferColorMode m_bufferMode;
  float m_brightness;
  size_t m_playerCnt;

private:

  std::shared_ptr<BaseInput> m_inputHandler;
  std::stack<std::shared_ptr<BaseApplication> >  m_applicationStack;
  std::vector<std::shared_ptr<BaseApplication> > m_queuedApplications;
  Image m_frameBuffer;
  KeyboardDebouncer m_kdb;
  std::shared_ptr<bmfont::Font> m_font;

  size_t m_refTimeStartUs;

public:

  BaseController();
  virtual ~BaseController();

  virtual bool initialize(size_t                    width,
                          size_t                    height,
                          std::shared_ptr<BaseInput>input,
                          bool                      debug = false);

  void addApplication(std::shared_ptr<BaseApplication>app,
                      bool                            queuedInsert = false);
  void run(size_t fps = 50);
  void clearFrame(ColorRGB color);
  void clearFrame(uint8_t paletteIdx);

  void setBrightness(float b) {
    m_brightness = b;
  }

  float getBrightness() {
    return m_brightness;
  }

  void setPlayerCount(size_t p) {
    m_playerCnt = p;
  }

  size_t getPlayerCount() {
    return m_playerCnt;
  }

  TimeUnit getTimeMs();

  size_t   getWidth() {
    return m_width;
  }

  size_t getHeight() {
    return m_height;
  }

  size_t getSize() {
    return m_size;
  }

  bool isDebug() {
    return m_debug;
  }

  std::shared_ptr<bmfont::Font>getDefaultFont() {
    return m_font;
  }

protected:

  virtual void           showFrame(const Image& frame) = 0;
  virtual void           shutdown();
  virtual const Palette& getCurrentPalette();
  void                   updateBufferColorMode();

private:

  void createFrame();
  void addApplicationDirect(std::shared_ptr<BaseApplication>app);
};
}

#endif // ifndef _H_BASE_CONTROLLER_

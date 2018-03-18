#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/core/baseController.hpp>
namespace led {
BaseApplication::BaseApplication() {
  m_requiresRedraw = true;
  m_ctrl           = nullptr;
  m_requiresRedraw = true;
  m_hasFinished    = false;
  m_isPaused       = false;
  m_standbyAllowed = false;
}

BaseApplication::~BaseApplication() {}

void BaseApplication::initialize(BaseController *ctrl) {
  m_ctrl           = ctrl;
  m_requiresRedraw = true;
  m_hasFinished    = false;
  m_isPaused       = false;
  m_standbyAllowed = false;

  // Set default color
  m_colorPalette.push_back({ 0, 0, 0 });
  m_bufferColorMode = BufferColorMode::PALETTE;
}

void BaseApplication::deinitialize() {}

void BaseApplication::pauseApp() {
  m_isPaused = true;
}

const Palette& BaseApplication::getPalette() {
  return m_colorPalette;
}

void BaseApplication::continueApp() {
  m_isPaused = false;
}

bool BaseApplication::requiresRedraw() {
  return m_requiresRedraw;
}

bool BaseApplication::hasFinished() {
  return m_hasFinished;
}

bool BaseApplication::allowsStandby() {
  return m_standbyAllowed;
}

BufferColorMode BaseApplication::getBufferColorMode() {
  return m_bufferColorMode;
}
}

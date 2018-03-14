#include <LEDTableEngine/core/shaderApplication.hpp>
#include <LEDTableEngine/core/baseController.hpp>

namespace led {
ShaderApplication::ShaderApplication() {}

ShaderApplication::~ShaderApplication() {}

void ShaderApplication::initialize(BaseController *ctrl, size_t shaderCnt) {
  BaseApplication::initialize(ctrl);
  m_bufferColorMode = BufferColorMode::RGB;
  m_shaderCnt       = shaderCnt;
  m_shaderCurrId    = 0;
}

void ShaderApplication::processInput(const BaseInput::InputEvents& events,
                                     TimeUnit                      deltaTime) {
  if (BaseInput::isPressed(events, BaseInput::InputEventName::EXIT)) {
    m_hasFinished = true;
    return;
  }

  if (BaseInput::isPressed(events, BaseInput::InputEventName::ENTER)) {
    m_shaderCurrId   = (m_shaderCurrId + 1) % m_shaderCnt;
    m_requiresRedraw = true;
  }
}

void ShaderApplication::draw(Image& frame) {
  size_t   ixsize = frame.width;
  size_t   iysize = frame.height;
  uint8_t *p      = frame.data;
  TimeUnit tm     = m_ctrl->getTimeMs();

  for (std::size_t iy = 0; iy < iysize; ++iy)
  {
    for (std::size_t ix = 0; ix < ixsize; ++ix)
    {
      renderPixel(tm, ix, iy, *(p + 0), *(p + 1), *(p + 2), m_shaderCurrId);
      p += 3;
    }
  }
}
}

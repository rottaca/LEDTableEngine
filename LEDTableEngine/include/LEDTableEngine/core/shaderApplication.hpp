#ifndef _H_SHADER_BASE_APP_
#define _H_SHADER_BASE_APP_

#include <complex>

#include <LEDTableEngine/core/baseApplication.hpp>
namespace led {
class ShaderApplication : public led::BaseApplication {
protected:

  size_t m_shaderCnt;
  size_t m_shaderCurrId;

public:
  ShaderApplication();
  virtual ~ShaderApplication();

  virtual void initialize(led::BaseController *ctrl, size_t shaderCnt);

  virtual void processInput(const led::BaseInput::InputEvents& events,
                    led::TimeUnit                      deltaTime);
  virtual void draw(led::Image& frame);

protected:
  virtual void renderPixel(led::TimeUnit tm,
                   size_t        x,
                   size_t        y,
                   uint8_t     & r,
                   uint8_t     & g,
                   uint8_t     & b,
                   size_t shaderId) = 0;
};
}

#endif // ifndef _H_SHADER_BASE_APP_

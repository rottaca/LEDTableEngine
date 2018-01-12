#ifndef _H_SHADERS_APP_
#define _H_SHADERS_APP_

#include <complex>

#include "../engine/baseApplication.hpp"

class ShaderApp : public led::BaseApplication {
protected:

  enum ShaderType {
    WATER = 0,
    CHESS,
    WAVINGCOLOR,
    END
  };

  ShaderType m_shader;

public:

  ShaderApp();
  virtual ~ShaderApp();

  void initialize(led::BaseController *ctrl);

  void processInput(const led::BaseInput::InputEvents& events,
                    led::TimeUnit                      deltaTime);
  void draw(led::Image& frame);
  void continueApp();

private:

  void waterShader(led::TimeUnit tm,
                   size_t   x,
                   size_t   y,
                   uint8_t& r,
                   uint8_t& g,
                   uint8_t& b);
  void chessboardShader(led::TimeUnit tm,
                        size_t   x,
                        size_t   y,
                        uint8_t& r,
                        uint8_t& g,
                        uint8_t& b);
  void wavingColors(led::TimeUnit tm,
                    size_t   x,
                    size_t   y,
                    uint8_t& r,
                    uint8_t& g,
                    uint8_t& b);
};


#endif // ifndef _H_SHADERS_APP_

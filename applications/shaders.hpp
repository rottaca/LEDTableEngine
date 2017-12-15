#ifndef _H_SHADERS_APP_
#define _H_SHADERS_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>
#include <cstdlib>
#include <complex>

#include "../engine/baseApplication.hpp"

class ShaderApp : public BaseApplication {
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

  void initialize(BaseController *ctrl);

  void processInput(const BaseInput::InputEvents& events,
                    TimeUnit                      deltaTime);
  void draw(Image& frame);
  void continueApp();

private:

  void waterShader(TimeUnit tm,
                   size_t   x,
                   size_t   y,
                   uint8_t& r,
                   uint8_t& g,
                   uint8_t& b);
  void chessboardShader(TimeUnit tm,
                        size_t   x,
                        size_t   y,
                        uint8_t& r,
                        uint8_t& g,
                        uint8_t& b);
  void wavingColors(TimeUnit tm,
                    size_t   x,
                    size_t   y,
                    uint8_t& r,
                    uint8_t& g,
                    uint8_t& b);
};


#endif // ifndef _H_SHADERS_APP_

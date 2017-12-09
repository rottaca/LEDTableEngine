#ifndef _H_SHADERS_APP_
#define _H_SHADERS_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>
#include <cstdlib>
#include <complex>

#include "../LEDTableEngine/baseApplication.hpp"

class ShaderApp: public BaseApplication {
protected:
  enum ShaderType{
               WATER = 0,
               CHESS,    
               END};

  ShaderType m_shader;

public:
  ShaderApp();
  virtual ~ShaderApp ();

  void initialize(BaseController * ctrl);

  void processInput(const BaseInput::InputEvents &events,
                    const BaseInput::InputEvents &eventsDebounced,
                    TimeUnit deltaTime);
  void draw(Image &frame);
  void continueApp();

private:
  void waterShader(TimeUnit tm, size_t x, size_t y, uint8_t &r, uint8_t &g, uint8_t &b);
  void chessboardShader(TimeUnit tm, size_t x, size_t y, uint8_t &r, uint8_t &g, uint8_t &b);

};


#endif

#ifndef _H_ANIMATION_APP_
#define _H_ANIMATION_APP_

#include <LEDTableEngine/core/shaderApplication.hpp>
#include <LEDTableEngine/core/types.hpp>

class AnimationApp : public led::ShaderApplication {
protected:

  struct WaveDef{
    float amp;
    led::Pointf freq;
    led::Pointf shift;
  };

  std::vector<WaveDef> m_waveDefs;

public:

  AnimationApp();
  virtual ~AnimationApp();

  void initialize(led::BaseController *ctrl);
protected:
  void renderPixel(led::TimeUnit tm,
                   size_t        x,
                   size_t        y,
                   uint8_t     & r,
                   uint8_t     & g,
                   uint8_t     & b,
                   size_t shaderId);
private:

  void waterShader(led::TimeUnit tm,
                   size_t        x,
                   size_t        y,
                   uint8_t     & r,
                   uint8_t     & g,
                   uint8_t     & b);
  void wavingColors(led::TimeUnit tm,
                    size_t        x,
                    size_t        y,
                    uint8_t     & r,
                    uint8_t     & g,
                    uint8_t     & b);
   void wavingColors2(led::TimeUnit tm,
                     size_t        x,
                     size_t        y,
                     uint8_t     & r,
                     uint8_t     & g,
                     uint8_t     & b);
};


#endif // ifndef _H_ANIMATION_APP_

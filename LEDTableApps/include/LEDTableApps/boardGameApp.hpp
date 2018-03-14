#ifndef _H_BOARD_GAME_SHADER_APP_
#define _H_BOARD_GAME_SHADER_APP_

#include <LEDTableEngine/core/shaderApplication.hpp>

class BoardGameApp : public led::ShaderApplication {
public:

  BoardGameApp();
  virtual ~BoardGameApp();

  void initialize(led::BaseController *ctrl);

protected:

  void renderPixel(led::TimeUnit tm,
                   size_t        x,
                   size_t        y,
                   uint8_t     & r,
                   uint8_t     & g,
                   uint8_t     & b,
                   size_t        shaderId);

private:

  void chessboardShader(led::TimeUnit tm,
                        size_t        x,
                        size_t        y,
                        uint8_t     & r,
                        uint8_t     & g,
                        uint8_t     & b);
};


#endif // ifndef _H_BOARD_GAME_SHADER_APP_

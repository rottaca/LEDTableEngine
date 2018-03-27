#ifndef _H_BOARD_GAME_SHADER_APP_
#define _H_BOARD_GAME_SHADER_APP_

#include <LEDTableEngine/core/shaderApplication.hpp>
#include <LEDTableEngine/core/rasterizer.hpp>

namespace ledApps{
class BoardGameApp : public led::ShaderApplication {
protected:

    led::Rasterizer m_rasterizer;

public:

    BoardGameApp();
    virtual ~BoardGameApp();

    void initialize(led::BaseController *ctrl);
    void draw(led::Image& frame);

    const std::string getName() const{
      return "Board Games";
    }
protected:

    void renderPixel(led::TimeUnit tm,
                     size_t x,
                     size_t y,
                     uint8_t     & r,
                     uint8_t     & g,
                     uint8_t     & b,
                     size_t shaderId);

private:

/**
 * Chess shader
 */
    void chessboard(led::Image& img);

/**
 * German game, called 'Mensch ärgere dich nicht!'
 */
    void MADN(led::Image& img);
};
}

#endif // ifndef _H_BOARD_GAME_SHADER_APP_

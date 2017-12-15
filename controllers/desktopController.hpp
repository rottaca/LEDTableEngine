#ifndef _H_DESKTOP_CONTROLLER_
#define _H_DESKTOP_CONTROLLER_

#include <cstdlib>
#include <memory>
#include <stack>
#include <vector>
#include <chrono>

// SDL-Header einbinden
#ifdef WIN32
# include <SDL.h>
#else // ifdef WIN32
# include <SDL2/SDL.h>
#endif // ifdef WIN32

#include "../engine/baseController.hpp"

class DesktopController : public BaseController {
public:

  DesktopController();
  ~DesktopController();

  bool initialize(size_t                    width,
                  size_t                    height,
                  std::shared_ptr<BaseInput>input,
                  bool                      debug = false);

private:

  void showFrame(const Image& frame);
  void shutdown();
  void copyImageToBuffer(const Image& frame);

  int m_upscaleWidth, m_upscaleHeight;
  SDL_Window   *m_window;
  SDL_Surface  *m_windowSurf;
  SDL_Surface  *m_imageSurf;
  SDL_Renderer *m_renderer;
  float m_upscale;
};


#endif // ifndef _H_DESKTOP_CONTROLLER_

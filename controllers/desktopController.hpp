#ifndef _H_DESKTOP_CONTROLLER_
#define _H_DESKTOP_CONTROLLER_

#include <cstdlib>
#include <memory>
#include <stack>
#include <vector>
#include <chrono>

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "../LEDTableEngine/baseController.hpp"

class DesktopController : public BaseController {
public:

  DesktopController();
  ~DesktopController ();

  bool initialize(size_t width, size_t height,
                          std::shared_ptr<BaseInput> input, bool debug=false);

private:
  void showFrame(const std::vector<uint8_t>&frame);
  void shutdown();
  void copyImageToBuffer(const std::vector<uint8_t>&frame);

  int m_upscaleWidth, m_upscaleHeight;
  GtkWidget *m_gtkWindow;
  GtkWidget *m_gtkWidget;
  GdkPixbuf *m_gtkPixBuf;
  float m_upscale;

};


#endif

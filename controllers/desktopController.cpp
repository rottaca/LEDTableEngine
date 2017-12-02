#include "desktopController.hpp"
#include <cmath>

DesktopController::DesktopController(){

}
DesktopController::~DesktopController(){

}

bool DesktopController::initialize(size_t width, size_t height,
                          std::shared_ptr<BaseInput> input, bool debug){
   BaseController::initialize(width, height, input, debug);
   m_upscale = 30;
   m_upscaleWidth = width*m_upscale;
   m_upscaleHeight= height*m_upscale;

   gtk_init(nullptr,nullptr);
   m_gtkWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (m_gtkWindow), "Window");
   gtk_window_set_default_size (GTK_WINDOW (m_gtkWindow), m_upscaleWidth, m_upscaleHeight);

   m_gtkPixBuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB,false,8,m_upscaleWidth,m_upscaleHeight);
   m_gtkWidget = gtk_image_new_from_pixbuf (m_gtkPixBuf);
   gtk_container_add (GTK_CONTAINER (m_gtkWindow), m_gtkWidget);
   gtk_widget_show_all (GTK_WIDGET (m_gtkWindow));
}
void DesktopController::copyImageToBuffer(const std::vector<uint8_t>&frame){
   int rowstride = gdk_pixbuf_get_rowstride (m_gtkPixBuf);
   unsigned char* pixels = gdk_pixbuf_get_pixels (m_gtkPixBuf);
   const BaseApplication::Palette& palette = getCurrentPalette();

   for (size_t y = 0; y < m_upscaleHeight; y++) {
     unsigned char* p = pixels + y*rowstride;
     for (size_t x = 0; x < m_upscaleWidth; x++) {
       size_t realX, realY;
       realX = std::floor(x / m_upscale);
       realY = std::floor(y / m_upscale);

       assert(realX >= 0 && realX < m_width);
       assert(realY >= 0 && realY < m_height);
       assert(realX + realY*m_width < frame.size());

       int c = frame[realX + realY*m_width];
       assert(c*3 < palette.size());
       uint8_t r,g,b;
       r = palette[c*3];
       g = palette[c*3 + 1];
       b = palette[c*3 + 2];
       *p++ = r;
       *p++ = g;
       *p++ = b;
     }
   }
}

void DesktopController::showFrame(const std::vector<uint8_t>&frame){
    copyImageToBuffer(frame);
    gtk_widget_queue_draw(m_gtkWidget);
    while ( gtk_events_pending() ) gtk_main_iteration();
}

void DesktopController::shutdown(){
  gtk_main_quit ();
}

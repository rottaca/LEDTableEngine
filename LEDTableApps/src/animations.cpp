#include <LEDTableApps/animations.hpp>
#include <LEDTableEngine/core/baseController.hpp>

#include <complex>

#define RAND_FLOAT(A,B)(A + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/B)))

using namespace led;
AnimationApp::AnimationApp() {}

AnimationApp::~AnimationApp() {}

void AnimationApp::initialize(BaseController *ctrl) {
  ShaderApplication::initialize(ctrl,3);

  srand(ctrl->getTimeMs());
m_waveDefs.clear();
for (size_t i = 0; i < 5; i++) {
  WaveDef wd;
  wd.amp = RAND_FLOAT(10,50);
  wd.freq = led::Pointf(RAND_FLOAT(-0.4,0.4),RAND_FLOAT(-0.4,0.4));
  wd.shift = led::Pointf(RAND_FLOAT(-3,3),RAND_FLOAT(-3,3));
  m_waveDefs.push_back(wd);
}
}

void AnimationApp::renderPixel(led::TimeUnit tm,
                 size_t        x,
                 size_t        y,
                 uint8_t     & r,
                 uint8_t     & g,
                 uint8_t     & b,
                 size_t shaderId){
 switch (shaderId) {
   case 0:
     waterShader(tm,x,y,r,g,b);
   break;
   case 1:
     wavingColors(tm,x,y,r,g,b);
   break;
   case 2:
     wavingColors2(tm,x,y,r,g,b);
   break;
 }
}
#define TAU 6.28318530718
#define MAX_ITER 5

void AnimationApp::waterShader(TimeUnit tm, size_t x, size_t y, uint8_t& r, uint8_t& g, uint8_t& b) {
  double time = tm / 1000.0 * .5 + 23.0;
  Pointf uv((double)x / m_ctrl->getWidth(), (double)y / m_ctrl->getHeight());

  Pointf p(fmod(uv.x * TAU, TAU) - 250.0, fmod(uv.y * TAU, TAU) - 250.0);
  Pointf i     = p;
  double c     = 1.0;
  double inten = .005;

  for (int n = 0; n < MAX_ITER; n++)
  {
    double t = time * (1.0 - (3.5 / double(n + 1)));
    i.x = p.x + cos(t - i.x) + sin(t + i.y);
    i.y = p.y + sin(t - i.y) + cos(t + i.x);
    Pointf tmp(p.x / (sin(i.x + t) / inten), p.y / (cos(i.y + t) / inten));
    c += 1.0 / sqrt(tmp.x * tmp.x + tmp.y * tmp.y);
  }
  c /= double(MAX_ITER);
  c  = 1.17 - pow(c, 1.4);

  //   std::cout << c << std::endl;
  r = std::min(std::max(pow(fabs(c), 8.0) + 0.0, 0.0), 1.0) * 255.0;
  g = std::min(std::max(pow(fabs(c), 8.0) + 0.35, 0.0), 1.0) * 255.0;
  b = std::min(std::max(pow(fabs(c), 8.0) + 0.5, 0.0), 1.0) * 255.0;
}

void AnimationApp::wavingColors(TimeUnit tm, size_t x, size_t y, uint8_t& r, uint8_t& g, uint8_t& b) {
  float p = (float)x / m_ctrl->getWidth() + (float)y / m_ctrl->getHeight();
  float h = (cos(p * 1 + tm / 1500.0) + 1) * 180;
  float s = (sin(p * 2 + tm / 3000.0) + 1) * 0.25;

  float tr, tg, tb;

  hsv2rgb(h, 0.5 + s, 1, tr, tg, tb);
  r = tr * 255;
  g = tg * 255;
  b = tb * 255;
}

void AnimationApp::wavingColors2(TimeUnit tm, size_t x, size_t y, uint8_t& r, uint8_t& g, uint8_t& b) {

  double tm_float = tm / 1000.0;
  float h = 0;
  for(const WaveDef &w: m_waveDefs){
    h+=w.amp*sin(x*w.freq.x + y*w.freq.y + w.shift.x*tm_float + w.shift.y*tm_float);
  }
  // Clamp to valid range
  h = std::min(360.0f,std::max(h+180,0.0f));
  float s = 0.3 + (h/360.0f)*0.7f;

  float tr, tg, tb;
  hsv2rgb(h, s, 1, tr, tg, tb);
  r = tr * 255;
  g = tg * 255;
  b = tb * 255;
}

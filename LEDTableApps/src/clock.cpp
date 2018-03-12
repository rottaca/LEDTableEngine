#include <LEDTableApps/clock.hpp>
#include <algorithm>
#include <LEDTableEngine/core/baseController.hpp>
#include <LEDTableApps/textDisplay.hpp>

using namespace led;
ClockApp::ClockApp() {}

ClockApp::~ClockApp() {}

void ClockApp::initialize(BaseController *ctrl) {
  BaseApplication::initialize(ctrl);
  m_colorPalette = {
    {   0,   0, 0 }, // Background
    {   0, 255, 0 },
    { 255,   0, 0 },
    { 0,   0, 255 },
    { 255, 255,255 }
  };

  fillPalette(m_colorPalette);
  m_rasterizer.setColor(1);
}

void ClockApp::processInput(const BaseInput::InputEvents& events,
                            TimeUnit                      deltaTime) {
  for (const auto& e : events) {
    switch (e.name) {
    case BaseInput::InputEventName::EXIT :
      m_hasFinished = true;
      return;
    }
  }
}

void ClockApp::draw(Image& frame) {
  m_ctrl->clearFrame(0);

  auto now       = std::chrono::high_resolution_clock::now();
  std::time_t tt = std::chrono::system_clock::to_time_t(now);
  std::tm     tm = std::tm { 0 };
  gmtime_r(&tt, &tm);

  int radius = std::floor(std::min(frame.width,frame.height)/2.0);
  Pointi c = Pointi(frame.width / 2, frame.height / 2);
  Pointi p,p2;
  // tm.tm_sec = 15;
  // tm.tm_min = 30;
  // tm.tm_hour = 9;
  // Draw circle
  m_rasterizer.setColor(1);
  m_rasterizer.drawCircle(frame, c , radius);
  float angle;

  angle = 2*3.14159f*(tm.tm_min)/59.0f - 3.14159f/2.0f ;
  p.x = c.x + (radius-1) * cos(angle);
  p.y = c.y + (radius-1) * sin(angle);
  m_rasterizer.setColor(3);
  m_rasterizer.drawLine(frame,c,p);

  angle = 2*3.14159f*(tm.tm_hour % 12)/11.0f - 3.14159f/2.0f ;
  p.x = c.x + std::floor((radius/2.0f) * cos(angle));
  p.y = c.y + std::floor((radius/2.0f) * sin(angle));
  m_rasterizer.setColor(4);
  m_rasterizer.drawLine(frame,c,p);

  angle = 2*3.14159f*(tm.tm_sec)/59.0f - 3.14159f/2.0f ;
  p.x = c.x + (radius-1) * cos(angle);
  p.y = c.y + (radius-1) * sin(angle);
  frame(p.y,p.x,0) = 2;

}

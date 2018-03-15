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
    {   0,   0,   0 }, // Background
    {   0, 255,   0 }, // Circle
    { 255,   0,   0 }, // Seconds
    {   0,   0, 255 }, // Minutes
    { 255, 255, 255 } // Hours
  };
  m_displayMode = ANALOG;
  fillPalette(m_colorPalette);
}

void ClockApp::processInput(const BaseInput::InputEvents& events,
                            TimeUnit                      deltaTime) {
  for (const auto& e : events) {
    if(e.state != BaseInput::InputEventState::KEY_PRESSED) continue;
    switch (e.name) {
    case BaseInput::InputEventName::EXIT :
      m_hasFinished = true;
      return;
    case BaseInput::InputEventName::ENTER :
      if(m_displayMode==ANALOG) m_displayMode=DIGITAL;
      else m_displayMode=ANALOG;
      break;
    }
  }
}

void ClockApp::draw(Image& frame) {
  m_ctrl->clearFrame(0);

  auto now       = std::chrono::high_resolution_clock::now();
  std::time_t tt = std::chrono::system_clock::to_time_t(now);
  std::tm     tm = std::tm { 0 };
  gmtime_r(&tt, &tm);

  if(m_displayMode == ANALOG){

    int radius = std::floor(std::min(frame.width, frame.height) / 2.0);
    Pointi c   = Pointi(frame.width / 2, frame.height / 2);
    Pointi p, p2;
    // Draw circle
    m_rasterizer.setColor(1);
    m_rasterizer.drawCircle(frame, c, radius);
    float angle;

    // Draw minutes
    angle = 2 * 3.14159f * (tm.tm_min) / 59.0f - 3.14159f / 2.0f;
    p.x   = c.x + (radius - 1) * cos(angle);
    p.y   = c.y + (radius - 1) * sin(angle);
    m_rasterizer.setColor(3);
    m_rasterizer.drawLine(frame, c, p);

    // Draw hours
    angle = 2 * 3.14159f * (tm.tm_hour % 12) / 11.0f - 3.14159f / 2.0f;
    p.x   = c.x + std::floor(((radius - 1) / 2.0f) * cos(angle));
    p.y   = c.y + std::floor(((radius - 1) / 2.0f) * sin(angle));
    m_rasterizer.setColor(4);
    m_rasterizer.drawLine(frame, c, p);

    // Draw seconds
    angle = 2 * 3.14159f * (tm.tm_sec) / 59.0f - 3.14159f / 2.0f;
    p.x   = c.x + (radius - 1) * cos(angle);
    p.y   = c.y + (radius - 1) * sin(angle);
    m_rasterizer.setColor(2);
    m_rasterizer.drawLine(frame, c, p);
  }else{
    char txt[9];
    sprintf(txt,"%02d:%02d",tm.tm_hour+1,tm.tm_min+1);
    Pointi sz = m_ctrl->getDefaultFont()->getTextSize(txt);
    std::cout << txt << " " << sz.x << " " << sz.y << std::endl;
    m_ctrl->getDefaultFont()->draw(frame, txt, frame.width/2.0-sz.x/2.0, frame.height/2.0-sz.y/2.0, {4});
  }
}

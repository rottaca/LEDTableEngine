#ifndef _H_BASE_APP_
#define _H_BASE_APP_

#include <cstdlib>
#include <memory>
#include <vector>

#include "baseInput.hpp"

class BaseController;

class BaseApplication {
public:
  typedef std::vector<uint8_t> Palette;

protected:
  BaseController* m_ctrl;
  bool m_requiresRedraw;
  bool m_hasFinished;
  bool m_isPaused;
  Palette m_colorPalette;

public:
  BaseApplication();
  virtual ~BaseApplication ();

  virtual void initialize(BaseController * ctrl);

  virtual void pauseApp();
  virtual void continueApp();
  virtual bool requiresRedraw();
  virtual bool hasFinished();
  virtual const Palette& getPalette();

  virtual void processInput(const BaseInput::InputEvents &events,
                            const BaseInput::InputEvents &eventsDebounced)=0;
  virtual void draw(std::vector<uint8_t> &frame)=0;
};


#endif

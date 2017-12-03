#ifndef _H_BASE_CONTROLLER_
#define _H_BASE_CONTROLLER_

#include <cstdlib>
#include <memory>
#include <stack>
#include <vector>
#include <chrono>
#include <iostream>
#include <thread>
#include <assert.h>

#include "baseApplication.hpp"

#include "keyboardDebouncer.hpp"
#include "types.hpp"

class BaseInput;

class BaseController {

public:

protected:
  size_t m_width;
  size_t m_height;
  size_t m_size;
  bool m_debug, m_isRunning;

private:
  std::shared_ptr<BaseInput> m_inputHandler;
  std::stack<std::shared_ptr<BaseApplication> > m_applicationStack;
  std::vector<std::shared_ptr<BaseApplication> > m_queuedApplications;
  std::vector<uint8_t> m_frameBuffer;
  KeyboardDebouncer m_kdb;

public:
  BaseController();
  virtual ~BaseController ();

  virtual bool initialize(size_t width, size_t height,
                          std::shared_ptr<BaseInput> input, bool debug=false);

  void addApplication(std::shared_ptr<BaseApplication> app, bool queuedInsert = false);
  void run(size_t fps=50);

  TimeUnit getTimeMs();
  void clearFrame(uint8_t val = 0);

  size_t getWidth(){
    return m_width;
  }
  size_t getHeight(){
    return m_height;
  }
  size_t getSize(){
    return m_size;
  }
  bool isDebug(){
    return m_debug;
  }

protected:
  virtual void showFrame(const std::vector<uint8_t>&frame)=0;
  virtual void shutdown();
  virtual const BaseApplication::Palette& getCurrentPalette();

private:
  void createFrame();
  void addApplicationDirect(std::shared_ptr<BaseApplication> app);
};


#endif

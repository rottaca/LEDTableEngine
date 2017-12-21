#include "update.hpp"
#include "../engine/baseController.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

UpdateApp::UpdateApp() {

}

UpdateApp::~UpdateApp() {}

void UpdateApp::initialize(BaseController *ctrl) {
  BaseApplication::initialize(ctrl);
  m_colorPalette = {
    {   0,   0,   0 },
    { 255, 255, 255 },
    { 0, 255, 0 },
    { 255, 0, 0 },
  };
  for (int i = m_colorPalette.size(); i <= 255; i++) {
    m_colorPalette.push_back({ 0, 0, 0 });
  }

  m_scrollText.init(m_ctrl->getHeight(),
                    m_ctrl->getWidth(), m_ctrl->getDefaultFont());
  m_scrollText.setColorPalette(1);
  m_scrollText.setText("Update?");
  m_state = IDLE;
}

void UpdateApp::processInput(const BaseInput::InputEvents& events,
                                  TimeUnit                      deltaTime) {

  State newState = m_state;
  if(m_state == DO_UPDATE){
    newState = UPDATING;
    m_childPID = fork();
    if (m_childPID  < 0) {
      newState = ERROR;
      std::cerr<< "Fork failed" << std::endl;
    }
    // Child
    else if (m_childPID == 0) {
      std::cout<< "Starting update..." << std::endl;
      int res = system("./../update.sh");
      if(res > 0)
        exit(1);
      else
        exit(0);
    }
    // Parent
    else {}
  }else if(m_state == UPDATING){
    int returnStatus;
    // Check if child process terminated
    int ret = waitpid(m_childPID, &returnStatus, WNOHANG);
    if(ret == -1){
      newState = ERROR;
      std::cerr<< "waitpid error" << std::endl;
    }else if(ret == 0){
      // Not yet finished
    }else if(ret == m_childPID && WIFEXITED(returnStatus)){
      std::cout << "Child exit status: " << WEXITSTATUS(returnStatus) << std::endl;
      newState = WEXITSTATUS(returnStatus)==0?DONE:ERROR;
      m_childPID = 0;
    }
  }


  for (auto& e : events) {
    if (e.state != BaseInput::InputEventState::KEY_PRESSED) {
      continue;
    }

    switch (e.name) {
    case BaseInput::InputEventName::ENTER:
      if(m_state == IDLE){
        newState = DO_UPDATE;
      }else if(m_state == DONE){
        // Force restart through start script
        exit(1);
      }else if(m_state == ERROR){
        // Don't exit automatically
        m_hasFinished = true;
      }
      break;
    case BaseInput::InputEventName::EXIT: {
      m_hasFinished = true;
      break;
    }
    }
  }

  if(newState != m_state){
    switch (newState) {
      case IDLE:
      case DO_UPDATE:
        m_scrollText.setText("Updating...");
        break;
      case DONE:
        m_scrollText.setText("Done.");
        m_scrollText.setColorPalette(2);
        break;
      case ERROR:
        m_scrollText.setText("Error!");
        m_scrollText.setColorPalette(3);
        break;
    }
    m_state = newState;
  }

  m_requiresRedraw = m_scrollText.update(m_ctrl->getTimeMs());
}

void UpdateApp::draw(Image& frame) {
  m_requiresRedraw = false;
  m_ctrl->clearFrame(0);
  m_scrollText.draw(frame);
}

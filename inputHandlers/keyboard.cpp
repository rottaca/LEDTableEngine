#include "keyboard.hpp"

cKeyboard::cKeyboard() {
  active = false;
  keyboard_fd = 0;
  keyboard_ev = new input_event();
  keyboard_st = new keyboard_state();
}
bool cKeyboard::start(std::string keyboardDev){
  active = false;
  keyboard_fd = 0;
  keyboard_ev = new input_event();
  keyboard_st = new keyboard_state();
  keyboard_fd = open(keyboardDev.c_str(), O_RDONLY | O_NONBLOCK);
  if (keyboard_fd > 0) {
      ioctl(keyboard_fd, EVIOCGNAME(256), name);
      std::cout << "Opened Keyboard event file: " << name << std::endl;
      active = true;
      pthread_create(&thread, 0, &cKeyboard::loop, this);
  }else{
    std::cerr << "Couldn't open keyboard event file " << keyboardDev << "." << std::endl;
    return false;
  }
  return true;
}

void* cKeyboard::loop(void *obj) {
    while (reinterpret_cast<cKeyboard *>(obj)->active) reinterpret_cast<cKeyboard *>(obj)->readEv();
}

void cKeyboard::readEv() {
    int bytes = read(keyboard_fd, keyboard_ev, sizeof(*keyboard_ev));
    if (bytes > 0) {
        if (keyboard_ev->type & EV_KEY) {
            keyboard_st->keys[keyboard_ev->code] = keyboard_ev->value;
        }
    }
    usleep(10000);
}
short cKeyboard::getKeyState(short key) {
    return keyboard_st->keys[key];
}
cKeyboard::~cKeyboard() {
    if (keyboard_fd > 0) {
        active = false;
        pthread_join(thread, 0);
        close(keyboard_fd);
    }
    delete keyboard_st;
    delete keyboard_ev;
    keyboard_fd = 0;
}

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <iostream>
#include <fcntl.h>
#include <pthread.h>
#include <linux/input.h>
#include <unistd.h>

#define KEYBOARD_DEV "/dev/input/by-path/platform-i8042-serio-0-event-kbd"

struct keyboard_state {
    signed short keys[KEY_CNT];
};

class cKeyboard {
  private:
    pthread_t thread;
    bool active;
    int keyboard_fd;
    input_event *keyboard_ev;
    keyboard_state *keyboard_st;
    char name[256];

  protected:
  public:
    cKeyboard();
    ~cKeyboard();
    static void* loop(void* obj);
    void readEv();
    short getKeyState(short key);
};

#endif

#ifndef _H_MINESWEEPER_APP_
#define _H_MINESWEEPER_APP_

#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/core/matrixScrollText.hpp>

class MineSweeper : public led::BaseApplication {
protected:
  /**
   * Stores the state of each game field
   */
  struct FieldState{
    // 4 Bits for number of adjacent bombs (0 - 8)
    uint8_t neighborBombs : 4;
    // Marked by user?
    bool marked : 1;
    // Field opened?
    bool opened : 1;
    // Is bomb
    bool bomb: 1;
  };

  led::Pointi m_cursorPos;
  std::vector<FieldState> m_gamefield;
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_posDist;

  led::TimeUnit m_lastBlinkTime;
  bool m_cursorVisible;
  bool m_gameOver;
  size_t m_steps;

public:

  MineSweeper();
  virtual ~MineSweeper();

  void initialize(led::BaseController *ctrl);
  void deinitialize();

  void processInput(const led::BaseInput::InputEvents& events,
                    led::TimeUnit                      deltaTime);
  void draw(led::Image& frame);

private:
  void initGameField(size_t bombCnt, size_t w, size_t h);

  bool openField(int x, int y);

  bool gameWon();
};


#endif // ifndef _H_MineSweeper_APP_

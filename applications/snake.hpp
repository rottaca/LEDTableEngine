#ifndef _H_TESTPATTERN_APP_
#define _H_TESTPATTERN_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>
#inlude <list>

#include "../LEDTableEngine/baseApplication.hpp"

class Snake: public BaseApplication {
protected:
  struct Point{
	int x,y;  
	Point(){
		x = 0;
		y = 0;
	}
	Point(int x, int y){
		this->x = x;
		this->y = y;
	}
	
	bool operator==(const Point& l, const Point& r) const
	{
	  return l.x == r.x && l.y == r.y;
	}
  };
  
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_posDist;
  std::list<Point> m_snake;
  Point m_moveDir;
  Point m_foodPos;
  TimeUnit m_lastUpdateTime;
  TimeUnit m_deathTime;
  
public:
  Snake();
  virtual ~Snake ();

  void initialize(BaseController * ctrl);

  void processInput(const BaseInput::InputEvents &events,
                    const BaseInput::InputEvents &eventsDebounced,
                    TimeUnit deltaTime);
  void draw(std::vector<uint8_t> &frame);
};


#endif

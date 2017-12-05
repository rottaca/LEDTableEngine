#include "snake.hpp"
#include "../LEDTableEngine/baseController.hpp"
#include <algorithm>

Snake::Snake(){

}
Snake::~Snake (){

}

void Snake::initialize(BaseController * ctrl){
  BaseApplication::initialize(ctrl);
  m_colorPalette = {
    0,0,0,				// Background
    0,0,255,			// Food
    0,255,0,			// Snake
    255,0,0 			// Death
  };
  for(int i = m_colorPalette.size(); i <= 255; i++){
    m_colorPalette.push_back(0);
    m_colorPalette.push_back(0);
    m_colorPalette.push_back(0);
  }

  m_snake.clear();
  m_snake.push_front(Point(ctrl->getWidth()/2, ctrl->getHeight()/2));
  m_moveDir = Point(0, 1);
  m_foodPos = Point(ctrl->getWidth()/2, ctrl->getHeight()/2 + 5);
  m_lastUpdateTime = 0;
  m_deathTime = 0;
  m_generator = std::default_random_engine(m_ctrl->getTimeMs());
  m_posDist = std::uniform_int_distribution<int>(0,m_ctrl->getSize()-1);
}

void Snake::processInput(const BaseInput::InputEvents &events,
                          const BaseInput::InputEvents &eventsDebounced,
                          TimeUnit deltaTime){
  if(BaseInput::isPressed(eventsDebounced,BaseInput::InputEventName::EXIT)){
    m_hasFinished = true;
    return;
  }
	if(m_deathTime > 0){
      if(m_ctrl->getTimeMs() - m_deathTime > 3000){
        m_hasFinished = true;
      }
      return;
	}
	Point newDir = m_moveDir;
	for( const auto&e: eventsDebounced){
		if(e.state != BaseInput::InputEventState::KEY_PRESSED)
			continue;

		switch(e.name){
			case BaseInput::InputEventName::UP:
				newDir.x = 0;
				newDir.y = -1;
				break;
			case BaseInput::InputEventName::LEFT:
				newDir.x = -1;
				newDir.y = 0;
				break;
			case BaseInput::InputEventName::DOWN:
				newDir.x = 0;
				newDir.y = 1;
				break;
			case BaseInput::InputEventName::RIGHT:
				newDir.x = 1;
				newDir.y = 0;
				break;
			case BaseInput::InputEventName::EXIT:
				m_hasFinished = true;
				return;
		}
		if (newDir.x != -m_moveDir.x || newDir.y != -m_moveDir.y){
			m_moveDir = newDir;
		}
	}

	if(m_ctrl->getTimeMs() - m_lastUpdateTime < 1000.0/(0.2*m_snake.size()+1)){
		m_requiresRedraw = false;
		return;
	}

	m_lastUpdateTime = m_ctrl->getTimeMs();
	m_requiresRedraw = true;

	Point newPos(m_snake.front().x+m_moveDir.x,
				 m_snake.front().y+m_moveDir.y);

	if(newPos.x < 0 || newPos.x >= m_ctrl->getWidth() ||
	   newPos.y < 0 || newPos.y >= m_ctrl->getHeight() ||
	   (std::find(m_snake.begin(), m_snake.end(), newPos)!= m_snake.end())){
		   m_deathTime = m_ctrl->getTimeMs();
   }else{
	   m_snake.push_front(newPos);

	   if(newPos == m_foodPos){
       int idx = m_posDist(m_generator);
		   m_foodPos.x = idx % m_ctrl->getWidth();
		   m_foodPos.y = idx / m_ctrl->getWidth();
       std::cout << m_foodPos.x << "x" << m_foodPos.y << std::endl;
	   }else{
		   m_snake.pop_back();
	   }
   }
}
void Snake::draw(Image &frame){
	if(m_deathTime > 0){
		m_ctrl->clearFrame({3});
	}else{
		m_ctrl->clearFrame({0});
	}

	frame.data[m_foodPos.x + m_foodPos.y*frame.width] = 1;

	for(Point p: m_snake){
		frame.data[p.x + p.y*frame.width] = 2;
	}
}

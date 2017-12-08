#include "shaders.hpp"
#include "../LEDTableEngine/baseController.hpp"

ShaderApp::ShaderApp(){

}
ShaderApp::~ShaderApp (){

}

void ShaderApp::initialize(BaseController * ctrl){
  BaseApplication::initialize(ctrl);
  m_bufferColorMode = BufferColorMode::RGB;

}

void ShaderApp::continueApp(){
}
void ShaderApp::processInput(const BaseInput::InputEvents &events,
                          const BaseInput::InputEvents &eventsDebounced,
                          TimeUnit deltaTime){
    if(BaseInput::isPressed(eventsDebounced,BaseInput::InputEventName::EXIT)){
      m_hasFinished = true;
      return;
    }

    if(BaseInput::isPressed(eventsDebounced,BaseInput::InputEventName::ENTER)){
      m_shader = (ShaderType)((int)m_shader + 1);
      if(m_shader == END)
        m_shader = (ShaderType)0;
    }
}
void ShaderApp::draw(Image &frame){
   size_t ixsize = frame.width;
   size_t iysize = frame.height;
   uint8_t* p = frame.data;
   TimeUnit tm = m_ctrl->getTimeMs();
   for (std::size_t iy = 0; iy < iysize; ++iy)
   {
     for (std::size_t ix = 0; ix < ixsize; ++ix)
     {
       switch (m_shader) {
         case WATER:
         waterShader(tm, ix,iy,*(p+0),*(p+1),*(p+2));
         break;
       }
       p+=3;
     }
   }
}


#define TAU 6.28318530718
#define MAX_ITER 5

void ShaderApp::waterShader(TimeUnit tm, size_t x, size_t y, uint8_t &r, uint8_t &g, uint8_t &b){
  float time = tm/1000.0 * .5+23.0;
  Pointf uv((float)x/m_ctrl->getWidth(),(float)y/m_ctrl->getHeight());

  Pointf p(fmod(uv.x*TAU,TAU)-250.0,fmod(uv.y*TAU,TAU)-250.0 );
	Pointf i = p;
	float c = 1.0;
	float inten = .005;

	for (int n = 0; n < MAX_ITER; n++)
	{
		float t = time * (1.0 - (3.5 / float(n+1)));
		i.x = p.x + cos(t - i.x) + sin(t + i.y);
		i.y = p.y + sin(t - i.y) + cos(t + i.x);
    Pointf tmp(p.x / (sin(i.x+t)/inten), p.y / (cos(i.y+t)/inten));
		c += 1.0/sqrt(tmp.x*tmp.x + tmp.y*tmp.y);
	}
	c /= float(MAX_ITER);
	c = 1.17-pow(c, 1.4);

  r = std::min(std::max(pow(fabs(c), 8.0) + 0.0, 0.0),1.0)*255.0;
  g = std::min(std::max(pow(fabs(c), 8.0) + 0.35, 0.0),1.0)*255.0;
  b = std::min(std::max(pow(fabs(c), 8.0) + 0.5, 0.0),1.0)*255.0;
}

#include "matrixController.hpp"
#include <cmath>

MatrixController::MatrixController(){

}
MatrixController::~MatrixController(){

}

bool MatrixController::initialize(size_t width, size_t height,
                          std::shared_ptr<BaseInput> input, bool debug){
   BaseController::initialize(width, height, input, debug);
   m_tpm2 = std::make_shared<TPM2>();
   if(!m_tpm2->connect("/dev/pts/20")){
     return false;
   }
   m_frameDataPacket = m_tpm2->createTPM2DataPacket(width*height*3);
   m_tpm2->sendPacket(m_frameDataPacket);
   return true;

}
void MatrixController::copyImageToBuffer(const std::vector<uint8_t>&frame){
   const BaseApplication::Palette& palette = getCurrentPalette();
   size_t idx = 0;
   uint8_t* p = m_frameDataPacket.dataPtr;
   uint8_t r,g,b;
   for (size_t y = 0; y < m_height; y++) {
     for (size_t x = 0; x < m_width; x++) {

       int c = frame[idx++];
       r = palette[c*3];
       g = palette[c*3 + 1];
       b = palette[c*3 + 2];
       *p++ = r;
       *p++ = g;
       *p++ = b;
     }
   }
}

void MatrixController::showFrame(const std::vector<uint8_t>&frame){
  copyImageToBuffer(frame);
  m_tpm2->sendPacket(m_frameDataPacket);
}

void MatrixController::shutdown(){

}

#include "matrixController.hpp"
#include <cmath>

MatrixController::MatrixController() {}

MatrixController::~MatrixController() {}

bool MatrixController::initialize(size_t width, size_t height,
                                  std::shared_ptr<BaseInput>input, bool debug) {
  if(!BaseController::initialize(width, height, input, debug))
    return false;
  m_tpm2 = std::make_shared<TPM2>();

  if (!m_tpm2->connect("/dev/ttyACM0")) {
    return false;
  }
  m_frameDataPacket = m_tpm2->createTPM2DataPacket(width * height * 3);
  m_tpm2->sendPacket(m_frameDataPacket);
  return true;
}

void MatrixController::copyImageToBuffer(const Image& frame) {
  const Palette& palette = getCurrentPalette();
  uint8_t *pixels                         = m_frameDataPacket.dataPtr;
  uint8_t *p;
  size_t   idx = 0;
	ColorRGB c;

  for (size_t y = 0; y < m_height; y++) {
    p = pixels + 3 * y;

    for (size_t x = 0; x < m_width; x++) {
      if(x%2==0)
      	p = pixels + 3*(y + x*m_height);
      else
        p = pixels + 3*(m_height- 1 - y + x*m_height);

      // Palette mode
      if (m_bufferMode == BufferColorMode::PALETTE) {
          int p = frame.data[idx++];
          c = palette[p];
      }
      // RGB Mode
 	     else{
   		   c[0] = frame.data[idx++];
   		   c[1] = frame.data[idx++];
   		   c[2] = frame.data[idx++];
 	     }
 	     *p++ = c[0];
 	     *p++ = c[1];
 	     *p++ = c[2];
    }
  }
}

void MatrixController::showFrame(const Image& frame) {
  copyImageToBuffer(frame);
  m_tpm2->sendPacket(m_frameDataPacket);
}

void MatrixController::shutdown() {}

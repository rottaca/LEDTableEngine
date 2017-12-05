#ifndef _H_MATRIX_CONTROLLER_
#define _H_MATRIX_CONTROLLER_

#include <cstdlib>
#include <memory>
#include <stack>
#include <vector>
#include <chrono>

#include "../LEDTableEngine/baseController.hpp"
#include "TPM2.hpp"

class MatrixController : public BaseController {
public:

  MatrixController();
  ~MatrixController ();

  bool initialize(size_t width, size_t height,
                          std::shared_ptr<BaseInput> input, bool debug=false);

private:
  void showFrame(const Image&frame);
  void shutdown();
  void copyImageToBuffer(const Image&frame);

  TPM2::Packet m_frameDataPacket;
  std::shared_ptr<TPM2> m_tpm2;

};


#endif

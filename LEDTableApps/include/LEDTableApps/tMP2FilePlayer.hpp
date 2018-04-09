#ifndef _H_TMP2FILEPLAYER_
#define _H_TMP2FILEPLAYER_

#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/controllers/TMP2.hpp>

class TMP2FilePlayer : public led::BaseApplication {
private:
  std::vector<std::string> m_fileNames;
  size_t m_fileIdx;
  TMP2File m_tmp2File;
public:

    TMP2FilePlayer();
    virtual ~TMP2FilePlayer();

    void initialize(led::BaseController *ctrl);
    void deinitialize();

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);

    void continueApp();
    void pauseApp();

    const std::string getName()  const{
      return "TMP2 File Player";
    }
};


#endif // ifndef _H_TMP2FILEPLAYER_

#ifndef _H_BASE_INPUT_
#define _H_BASE_INPUT_

#include "types.hpp"

namespace led {
class BaseController;

class BaseInput {
public:

/**
 * A enum of all possible key states
 */
    enum InputEventState {
        KEY_PRESSED,         // Key pressed initially
        KEY_RELEASED,         // Key released
        KEY_HOLD,         // Key is still pressed
        KEY_NONE,         // Key has been released the last time
        TYPE_CNT         // Used to query the number of possible states
    };

/**
 * Array of strings to convert the enums to printable text
 */
    static const char *inputEventState2Str[];

/**
 * Names of all possible keys
 */
    enum InputEventName {
        UP,
        LEFT,
        DOWN,
        RIGHT,
        ENTER,
        EXIT,
        A,
        B,
        NAME_CNT         // Used to query the numer of possible keys
    };

/**
 * Array of strings to convert the enums to printable text
 */
    static const char *inputEventName2Str[];

  /**
   * Player IDs
   */
  enum PlayerID {
    PLAYER_0 = 0,
    PLAYER_1,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4,
    PLAYER_5,
    PLAYER_6,
    PLAYER_7,
    PLAYER_MAX_CNT
   };

/**
 * Stores the state, player id and name of a key event
 */
    struct InputEvent {
        InputEventState state;
        InputEventName name;
        PlayerID playerId;

        InputEvent() {
            state    = InputEventState::KEY_NONE;
            name     = InputEventName::UP;
            playerId = PLAYER_0;
        }

        InputEvent(InputEventState state,
                   InputEventName name,
                   PlayerID playerId) {
            this->name     = name;
            this->state    = state;
            this->playerId = playerId;
        }
    };

// List of individual input events
    typedef std::vector<InputEvent>InputEvents;

private:

    BaseController *m_ctrl;

public:

    BaseInput();
    virtual ~BaseInput();

    virtual void                   setController(BaseController *ctrl);
    virtual bool                   initialize();
    virtual void                   deinitialize();

    static bool                    isReleased(const InputEvents& events,
                                              InputEventName name);
    static bool                    isPressed(const InputEvents& events,
                                             InputEventName name);
    static bool                    isHold(const InputEvents& events,
                                          InputEventName name);

    virtual BaseInput::InputEvents getInputEvents() = 0;
    virtual size_t                 getSupportedPlayerCnt() = 0;
};
}

#endif // ifndef _H_BASE_INPUT_

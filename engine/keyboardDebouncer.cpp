#include "keyboardDebouncer.hpp"
#include <algorithm>
#include <iostream>

namespace led {
bool operator<(const BaseInput::InputEvent& a,  const BaseInput::InputEvent& b)
{
  return a.name < b.name;
}

bool operator==(const BaseInput::InputEvent& a,  const BaseInput::InputEvent& b)
{
  return a.name == b.name && a.playerId == b.playerId;
}

KeyboardDebouncer::KeyboardDebouncer() {}

KeyboardDebouncer::~KeyboardDebouncer() {}

void KeyboardDebouncer::processInput(const BaseInput::InputEvents& events) {
  // std::cout << "RawInputs:" << std::endl;
  // for(const auto& e: events){
  //   std::cout << BaseInput::inputEventName2Str[e.name] <<
  //             "(" << BaseInput::inputEventState2Str[e.state] << "), ";
  // }
  // std::cout << std::endl;
  BaseInput::InputEvents newEvents, removedEvents, updatedEvents;

  for (const auto& e : events) {
    auto lower = std::lower_bound(m_events.begin(), m_events.end(), e);

    if ((lower != m_events.end()) &&
        (lower->name == e.name) &&
        (lower->playerId == e.playerId)) {
      lower->state = BaseInput::InputEventState::KEY_HOLD;
      updatedEvents.push_back(*lower);
    } else {
      newEvents.push_back(e);
    }
  }

  for (auto e = m_events.begin(); e != m_events.end();) {
    if (std::find(updatedEvents.begin(), updatedEvents.end(),
                  *e) == updatedEvents.end()) {
      if (e->state == BaseInput::InputEventState::KEY_RELEASED) {
        e = m_events.erase(e);
      }
      else {
        e->state = BaseInput::InputEventState::KEY_RELEASED;
        e++;
      }
    } else {
      e++;
    }
  }

  for (auto e : newEvents) m_events.push_back(e);

  std::sort(m_events.begin(), m_events.end());

  // std::cout << "SortedInputs:" << std::endl;
  // for(const auto& e: m_events){
  //   std::cout << BaseInput::inputEventName2Str[e.name] <<
  //             "(" << BaseInput::inputEventState2Str[e.state] << "), ";
  // }
  // std::cout << std::endl;
}

const BaseInput::InputEvents& KeyboardDebouncer::getDebouncedEvents() {
  return m_events;
}
}

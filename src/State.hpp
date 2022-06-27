#ifndef STATE_HPP
#define STATE_HPP

#include <memory>

struct StateManager;

struct State {
  virtual ~State() {}
  virtual auto update(StateManager& sm) -> void = 0;
  virtual auto draw() -> void = 0;
  virtual auto draw_previous() -> bool { return false; }
};

using StatePtr = std::unique_ptr<State>;

#endif /* STATE_HPP */

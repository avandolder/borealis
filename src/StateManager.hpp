#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <cassert>
#include <memory>
#include <vector>

#include "State.hpp"

template <class T>
struct StateManager final {
 private:
  using StatePtr = std::unique_ptr<State<T>>;
  std::vector<StatePtr> states_;

 public:
  auto push(StatePtr&& st) -> void { states_.push_back(std::move(st)); }
  auto pop() -> void { states_.pop_back(); }
  auto replace(StatePtr&& st) {
    pop();
    push(std::forward<StatePtr>(st));
  }

  auto update(T& t) -> void {
    assert(!states_.empty());
    states_.back()->update(t);
  }

  auto draw() -> void {
    auto itr = states_.cend();
    while (itr != states_.cbegin() && (*--itr)->draw_previous())
      ;
    for (; itr != states_.cend(); ++itr)
      (*itr)->draw();
  }

  inline auto empty() const -> bool { return states_.empty(); }
};

#endif /* STATE_MANAGER_HPP */

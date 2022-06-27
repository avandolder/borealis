#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <cassert>
#include <memory>
#include <ranges>
#include <vector>

#include "State.hpp"

struct StateManager final {
 private:
  using StatePtr = std::unique_ptr<State<StateManager>>;
  std::vector<StatePtr> states_;

 public:
  auto push(StatePtr&& st) -> void { states_.push_back(std::move(st)); }

  auto pop() -> void { states_.pop_back(); }

  auto replace(StatePtr&& st) {
    pop();
    push(std::forward<StatePtr>(st));
  }

  auto update() -> void {
    assert(!states_.empty());
    states_.back()->update(*this);
  }

  auto draw() -> void {
    for (auto& st : states_ | std::views::reverse) {
      st->draw();
      if (!st->draw_previous())
        break;
    }
  }

  inline auto empty() const -> bool { return states_.empty(); }
};

#endif /* STATE_MANAGER_HPP */

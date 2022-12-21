#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <cassert>
#include <memory>
#include <vector>

#include "state.hpp"

template <class T>
struct StateManager final {
 private:
  using StatePtr = std::unique_ptr<State<T>>;
  std::vector<StatePtr> states_;

 public:
  void push(StatePtr&& st) { states_.push_back(std::move(st)); }
  void pop() { states_.pop_back(); }
  void replace(StatePtr&& st) {
    pop();
    push(std::forward<StatePtr>(st));
  }

  void update(T t) {
    assert(!states_.empty());
    states_.back()->update(t);
  }

  void draw() {
    auto itr = states_.cend();
    while (itr != states_.cbegin() && (*--itr)->draw_previous()) {
    }
    for (; itr != states_.cend(); ++itr) {
      (*itr)->draw();
    }
  }

  [[nodiscard]] auto empty() const -> bool { return states_.empty(); }
};

#endif /* STATE_MANAGER_HPP */

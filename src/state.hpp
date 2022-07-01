#ifndef STATE_HPP
#define STATE_HPP

template <class T>
struct State {
  virtual ~State() = default;
  virtual auto update(T) -> void = 0;
  virtual auto draw() -> void = 0;
  virtual auto draw_previous() -> bool { return false; }
};

#endif /* STATE_HPP */

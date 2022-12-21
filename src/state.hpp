#ifndef STATE_HPP
#define STATE_HPP

template <class T>
struct State {
  virtual ~State() = default;
  virtual void update(T) = 0;
  virtual void draw() = 0;
  virtual auto draw_previous() -> bool { return false; }
};

#endif /* STATE_HPP */

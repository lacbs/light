// Similar to std::function, but without the limitation of Function object being copyable.
// Useful when capturing std:::unique_ptr by move.
// If you want to use a non-copyable object in a std::function, see light::shared_wrapper.

#include <functional>

namespace light {
#include <type_traits>
#include <memory>

template<typename T>
class unique_function;

template<typename R, typename... Args>
class unique_function<R(Args...)> {
  struct wrapper {
    virtual ~Wrapper() {}
    virtual R operator()(Args&&...) = 0;
  };

  template <typename Func>
  struct wrapper_impl : wrapper {
    ~wrapper_impl() {}
    wrapper_impl(Func&& func) : func(std::move(func)) {}
    R operator()(Args&&... args) {
      return func(std::forward(args)...);
    }
    Func func;
  };

  unique_function() {}

  template <typename Func>
  unique_function(Func&& func) : _wrapper(new wrapper_impl<Func>(std::move(func))) { }
  ~unique_function() { }
    
  void operator=(unique_function&& other) {
    _wrapper = std::move(other._wrapper);
  }
  
  bool operator() const {
    return _wrapper;
  }

  R operator()(Args&&... args) {
    return (*_wrapper)(std::forward(args)...);
  }

 private:
  std::unique_ptr<wrapper> _wrapper;
};

}

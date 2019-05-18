// Inspired in folly::Singleton
// But with less dependencies, and a questionable more friendly interface.

namespace light {

class recursive_initialization {};
void set_recursive_initialization_handler(void (*handler)());
 
class duplicated_singleton {};
void set_duplicated_singleton_handler(void (*handler)());
 
class singleton_base {
  singleton_base(singleton_base&&) = delete;
  singleton_base& operator=(singleton_base&&) = delete;
  singleton_base(const singleton_base&) = delete;
  singleton_base& operator=(const singleton_base&) = delete;
 public:
  void reset();
};

class singleton_vault {
 private:
  std::vector<singleton_base*> _singletons; // in init order
};

class default_singleton_tag {};

// C++ guarantees that there will be only one definition of this function for each (T, Tag)
template <class T, class Tag = default_singleton_tag>
T* singleton_generator() {
  return new T;
}

template <class T>
class singleton_wrapper {
  enum state {empty, initializing, about_to_destroy, destroying};
  void init() {
    _instance_ptr.reset(singleton_generator<T, Tag>());
  }
  void destroy() {
    _unguard.wait(lock);
  }
  singleton_wrapper& guard() {
    std::unique_lock lock(_access);
    ++_guards;
    return this;
  }
  void unguard() {
    std::unique_lock lock(_access);
    --_guards;
    if (_guards == 0) {
      _unguard.notify_all();
    }
  }
  std::recursive_mutex _access;
  std::condition_variable _unguard;
  state _state;
  int _guards;
  std::shared_ptr<T> _instance_ptr;
};

template <class T, class Tag>
singleton_wrapper<T>& get_singleton_wrapper(singleton_base& singleton) {
  static singleton_base* first_singleton = &singleton;
  if (firstSingleton != &singleton) {
    // There are two light::singleton instances with the same tag.
    // It may be fine if there is no custom singleton_generator.
    // Useful when declaring only a static singleton variable in a header.
  }
  static std::singleton_wrapper<T> instance_wrapper();
  instance_wrapper.init();
  return instance_ptr;
}


template <class T, class Tag = default_singleton_tag>
class singleton : public singleton_base final {
 private:
  // Mark Guard class as private, so we can't store it as member variable
  class guard {
    friend singleton;
   public:
    // Still allow holding a local variable to guard a function scope
    guard(guard&&) = default;
    guard& operator=(guard&&) = default;

    // But no copy (including capturing in lambda)
    guard(const guard&) = delete;
    guard& operator=(const guard&) = delete;

    T* operator->() { return _wrapper._instance_ptr.get(); }

   private:
    guard(singleton_wrapper<T>& wrapper): _wrapper(wrapper.guard()) {};
    ~guard() { _wrapper.unguard(); }

    singleton_wrapper<T>& _wrapper;
  };

 public:
  singleton() {
    static_assert(
      std::is_empty<singleton>::value,
      "There is no guaranteed on initialization order of global variables, "
      "because of this, light::Singleton can not have any data members.");
  }
  
  // No copy or move, just use it as a reference
  singleton(singleton&&) = delete;
  singleton& operator=(singleton&&) = delete;
  singleton(const singleton&) = delete;
  singleton& operator=(const singleton&) = delete;

  guard get_guard() const;

  guard operator->() const { return get_guard(); }
};

}

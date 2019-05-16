// Inspired in folly::Singleton
// But with less dependencies, and a questionable more friendly interface.

namespace light {

class singleton_vault {

};

class default_singleton_tag {};

template <typename T, typename Tag = default_singleton_tag>
class singleton {
 private:
  class real {

  };
 public:
  singleton() {}

  singleton(singleton& other) = delete;
  singleton(const singleton& other) = delete;
  singleton(singleton&& other) = delete;

  singleton& operator=(singleton& other) = delete;
  singleton& operator=(const singleton& other) = delete;
  void operator=(singleton&& other) = delete;
};

}

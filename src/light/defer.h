// Similar to defer used in Swift.
// Register a function to be executed at the end of the scope.
// Making:
// {
//   code A
//   light::defer d1 = []{
//     code B
//   };
//   code C
//   LIGHT_DEFER(code D)
//   code E
// }
// Equivalent to:
// try {
//   code A
//   code C
//   try {
//     code E
//   } finally {
//     code D
//   }
// } finally {
//   code B
// }
// Notes:
// - There is no "finally" in C++, 
// - Use extra caution when throwing exception from the defer code, see:
//   - [can't find a way to implement this] defer_oe: Override Exception (ignore previously uncaught exception if throwing, similar to what python does)
//   - defer_ie: Ignore own Exception (simply add a try/catch block to the code, "safest")
//   - defer_iu: Ignore own excpetion if there is an Uncaught one
//   - defer_te: Throw Exception (default, aborts if there is an uncaught exception and is thowning)
//   - defer_ne: No exception (aborts if there is an exception, similar to noexcept(true) declaration)

#ifdef __COUNTER__
#define LIGHT_DEFER_WITH(type, code) light::defer ## type __light_defer_ ## __COUNTER__ ([&] { code; });
#else
#define LIGHT_DEFER_WITH(type, code) light::defer ## type __light_defer_ ## __LINE__ ([&] { code; }); // TODO: stringfy line
#endif
#define LIGHT_DEFER_OE(code) LIGHT_DEFER_WITH(oe, code)
#define LIGHT_DEFER_IE(code) LIGHT_DEFER_WITH(ie, code)
#define LIGHT_DEFER_IU(code) LIGHT_DEFER_WITH(iu, code)
#define LIGHT_DEFER_TE(code) LIGHT_DEFER_WITH(te, code)
#define LIGHT_DEFER_NE(code) LIGHT_DEFER_WITH(ne, code)
#define LIGHT_DEFER(code) LIGHT_DEFER_TE(code)

namespace light {

bool uncaught_exception() {
#if __cplusplus >= 201703
  return std::uncaught_exceptions() > 0;
#else
  return std::uncaught_exception();
#endif
}
 
 class sentinel
{
    int prev_uncaught;

public:
    sentinel() : prev_uncaught(std::uncaught_exceptions()) {}

    ~sentinel()
    {
        int cur_uncaught = std::uncaught_exceptions();
        if (cur_uncaught > prev_uncaught)
        {
        }
    }
};
 
 // try { throw; }
// Use first exception
class defer_te {
 public:
  template <typename Func>
  defer_ufe(Func func) {}
 private:
};

using defer = defer_te;

}

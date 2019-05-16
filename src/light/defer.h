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
//   - defer_ufe: Use first exception (default, use first uncaught exception, C++17 behavior)
//   - defer_ig: Ignore own exception (simply add a try/catch block to the code)
//   - defer_ule: Use last exception (similar to what python does, use last uncaught exception)
//   - defer_ne: No exception (aborts if there is an exception, similar to noexcept(true) declaration)

#define LIGHT_DEFER_WITH(type, code) light::defer ## type __light_defer_ ## __COUNTER__ ([&] { code; });
#define LIGHT_DEFER_UFE(code) LIGHT_DEFER_WITH(ufe, code)
#define LIGHT_DEFER_IG(code) LIGHT_DEFER_WITH(ig, code)
#define LIGHT_DEFER_ULE(code) LIGHT_DEFER_WITH(ule, code)
#define LIGHT_DEFER_NE(code) LIGHT_DEFER_WITH(ne, code)
#define LIGHT_DEFER(code) LIGHT_DEFER_UFE(code)

namespace light {

// Use first exception
class defer_ufe {
 public:
  template <typename Func>
  defer_ufe(Func func) {}
 private:
};

using defer = defer_ufe;

}

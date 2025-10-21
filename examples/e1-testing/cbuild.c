#include "../../cbuild.h"
#include "../../extra/cbuild_tests.h"

CB_TEST(t1) {
  cb_assert(!"🎂" && "The cake is a lie");
}

CB_TEST(t2) {
  int res = 0;
  for (int i = 1; i <= 100; ++i) { res += i; }
  cb_assert(res == (100 * 101)/2 && "You shouldn't see this message");
}

int32_t main(int32_t argc, char **argv) {
  cb_rebuild_self(argc, argv);
  cb_test_run();
}

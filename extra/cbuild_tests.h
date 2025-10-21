#ifndef CBUILD_EXTRA_TESTS_H
#define CBUILD_EXTRA_TESTS_H

#include <setjmp.h>

typedef void cb_test_func(void);

struct CB_Test {
  const char *name;
  cb_test_func *function;
};

#if COMPILER_CL
#pragma section(".CRT$XCU", read)
typedef void cb_w32_test_func_init(void);
#define CB_TEST(FUNC_NAME)                                       \
  static void FUNC_NAME(void);                                   \
  internal void register_##FUNC_NAME(void) {                     \
    cb_test_register((struct CB_Test) {                          \
      .function = &FUNC_NAME,                                    \
      .name = #FUNC_NAME,                                        \
    });                                                          \
  }                                                              \
  __declspec(allocate(".CRT$XCU"))                               \
  cb_w32_test_func_init *FUNC_NAME##_reg = register_##FUNC_NAME; \
  static void FUNC_NAME(void)
#else
#define CB_TEST(FUNC_NAME)                   \
  static void FUNC_NAME(void);               \
  __attribute__((constructor))               \
  internal void register_##FUNC_NAME(void) { \
    cb_test_register((struct CB_Test) {      \
      .function = &FUNC_NAME,                \
      .name = #FUNC_NAME,                    \
    });                                      \
  }                                          \
  static void FUNC_NAME(void)
#endif

static void cb_test_run(void);

internal void cb_test_register(struct CB_Test test);
internal void cb_assertion_test(const char *condition,
                                const char *file,
                                int32_t line);


// ======================================================================
// Implementations
internal jmp_buf test_runner_jmpbuf = {0};

internal struct {
  struct CB_Test *tests;
  size_t count;
  size_t capacity;
} cb_tests = {0};

static void cb_test_run(void) {
  int32_t test_passed_count = 0;
  cb_assertion_handler = cb_assertion_test;
  for (size_t i = 0; i < cb_tests.count; ++i) {
    printf("test %s ... ", cb_tests.tests[i].name);
    if (setjmp(test_runner_jmpbuf) == 0) {
      cb_tests.tests[i].function();
      printf(ANSI_COLOR_GREEN "ok\n" ANSI_COLOR_RESET);
      test_passed_count += 1;
    }
  }
  cb_assertion_handler = cb_assertion_break;
  cb_println(CB_LogLevel_Info, "Summary: %d/%d tests passed",
             test_passed_count, cb_tests.count);
}

internal void cb_test_register(struct CB_Test test) {
  cb_dyn_push_custom(&cb_tests, test,
                     tests, count, capacity);
}

internal void cb_assertion_test(const char *condition,
                                const char *file, int32_t line) {
  printf(ANSI_COLOR_RED "FAILED" ANSI_COLOR_RESET
         ": `%s` (%s:%d)\n", condition, file, line);
  longjmp(test_runner_jmpbuf, 1);
}

#endif

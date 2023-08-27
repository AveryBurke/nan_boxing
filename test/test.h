#define run_test(test) do { \
  int result = test(); \
  if (!result) { return result; } \
  printf(": pass\n"); \
} while (0)

#define assert_eq_type(a, b, type) do { \
  int a_ = a; \
  int b_ = b; \
  if (a_ != b_) { \
    printf("\n  ***%s: expected %d to equal %d***", type, a_, b_); \
    return 0; \
  } \
} while (0)

#define begin_test { printf("%s", __func__); increment_tests_run; }
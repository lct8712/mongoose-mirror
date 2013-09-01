#ifndef TEST_LIB_A_H_
#define TEST_LIB_A_H_

namespace test_lib {

class EmptyClass {
 public:
  static int Add(int a, int b) { return a + b; }
  static int Sum(int a, int b);
};

int Max(int a, int b) {
  return a > b ? a : b;
}

}

#endif  // TEST_LIB_A_H_

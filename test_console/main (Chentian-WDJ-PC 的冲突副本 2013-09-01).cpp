#include "..\test_dll\a.h"
#include "..\test_lib\a.h"
#include <iostream>

enum ENUM_TYPE {
  ENUM_TYPE_A,
};

int sum(int a, int b) {
  return a + b;
}

int main() {
  printf("1 + 2 = %d\n", test_lib::EmptyClass::Add(1, 2));
  printf("4 + 5 = %d\n", test_lib::EmptyClass::Add(4, 5));
  printf("max of 3, 4: %d\n", test_lib::Max(3, 4));

  printf("1 + 2 = %d\n", test_dll::EmptyClass::Add(1, 2));
  printf("4 + 5 = %d\n", test_dll::EmptyClass::Add(4, 5));
  printf("max of 3, 4: %d\n", test_dll::Max(3, 4));

  return 0;
}

#include "..\test_dll\a.h"
#include "..\test_lib\a.h"
#include <iostream>
#include <cassert>

bool is_character(char c) {
  return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A');
}

bool is_leagl_package_name(const char* name) {
  if (name == NULL || name[0] == '\0') {
    return false;
  }

  const char *p = name;
  while (*p != '\0') {
    if (*p != '.' && !is_character(*p)) {
      return false;
    }
    p++;
  }
  return true;
}

int main() {
  //printf("1 + 2 = %d\n", test_lib::EmptyClass::Add(1, 2));
  //printf("4 + 5 = %d\n", test_lib::EmptyClass::Add(4, 5));
  //printf("max of 3, 4: %d\n", test_lib::Max(3, 4));

  //printf("1 + 2 = %d\n", test_dll::EmptyClass::Add(1, 2));
  //printf("4 + 5 = %d\n", test_dll::EmptyClass::Add(4, 5));
  //printf("max of 3, 4: %d\n", test_dll::Max(3, 4));
  
  assert(is_leagl_package_name("abcAAA.aaa"));
  assert(!is_leagl_package_name("abc1AAA.aaa"));

  return 0;
}

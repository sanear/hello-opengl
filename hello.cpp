#include <iostream>
using namespace std;

int doMath(int x, int y) {
  int result = 1;
  for (int i = 0; i < y; i++) {
    result *= x;
  }
  return result;
}

int main() {
  int base = 2;
  int exp = 10;

  printf("%i**%i = %i\n", base, exp, doMath(base, exp));
  return 0;
}

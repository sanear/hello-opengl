#include <iostream>
#include "../lib/models.h"  // TODO: impossible to imagine this being correct
#include <GLFW/glfw3.h>

using namespace std;

int main() {
  OnScreenObject *oso = new OnScreenObject(0, 0);
  cout << "This shouldn't work! " << oso->info() << endl;

  ALilGuy *guy = new ALilGuy("Ava", 0, 0);

  cout << endl;
  cout << "There's a guy! " << guy->info() << endl;
  int delX = 16;
  int delY = 32;
  cout << "They're moving " << delX << " units right and " << delY << " units up!" << endl;
  float dist = guy->move(delX, delY);
  cout << "They moved " << dist << " units!" << endl;
  cout << guy->info() << endl;
  return 0;
}

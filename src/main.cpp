#include <iostream>

#include <stdlib.h>

#include "hello-opengl.h"
#include "../lib/models.h"  // TODO: impossible to imagine this being correct

using namespace std;

void lilGuyPoc() {
  ALilGuy *guy = new ALilGuy("Ava", 0, 0);

  cout << endl;
  cout << "There's a guy! " << guy->info() << endl;
  int delX = 16;
  int delY = 32;
  cout << "They're moving " << delX << " units right and " << delY << " units up!" << endl;
  float dist = guy->move(delX, delY);
  cout << "They moved " << dist << " units!" << endl;
  cout << guy->info() << endl;
}


int main() {
  // OO hello world
  lilGuyPoc();
  do_triangle();

  exit(EXIT_SUCCESS);
}
